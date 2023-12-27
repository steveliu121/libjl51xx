#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include <getopt.h>

#include "driver/jl_reg_io.h"

#if (RAND_MAX < 256)
#error "RAND_MAX is expected bigger than 256"
#endif

struct option longopts[] = {
	{"round", required_argument, NULL, 'r'},
	{"help", no_argument, NULL, 'h'},
	{0, 0, 0, 0}
};

void print_usage(char *app)
{
	printf("======IO over SMI Pressure Test======\n");
	printf("Generate a random register value, write, read, compare\n");
	printf("Using LLDP Configuration register by default\n");
	printf("Usage: %s -r <test_round>\n", app);
	printf("Example:\n");
	printf("\t%s -r 1000\n", app);
}

uint32_t _rand_uint32(void)
{
	uint32_t r0, r1, r2, r3;
	struct timeval tv;

	gettimeofday(&tv, NULL);
	srand(tv.tv_usec);
	r0 = rand() % 256;
	gettimeofday(&tv, NULL);
	srand(tv.tv_usec);
	r1 = rand() % 256;
	gettimeofday(&tv, NULL);
	srand(tv.tv_usec);
	r2 = rand() % 256;
	gettimeofday(&tv, NULL);
	srand(tv.tv_usec);
	r3 = rand() % 256;

	return (r0 << 24| r1 << 16 | r2 << 8 | r3);
}

int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;
	int opt;
	uint32_t buf[8] = {0};
	uint32_t buf_0[8] = {0};
	uint32_t count = 0;
	uint32_t round = 0;
	uint8_t i = 0;
	jl_api_ret_t ret;

	while ((opt = getopt_long(argc, argv,
		"r:h", longopts, NULL)) != -1) {
		switch (opt) {
		case 'r':
			round = strtoul(optarg, NULL, 10);
			break;
		case 'h':
			print_usage(argv[0]);
			return 0;
		case ':':
			fprintf(stdout, "required argument : -%c\n", optopt);
			return -1;
		case '?':
			fprintf(stdout, "invalid param: -%c\n", optopt);
			print_usage(argv[0]);
			return -1;
		}
	}

	ret = jl_reg_io_init();
	if (ret) {
		printf("smi init fail\n");
		return -1;
	}

	while (count++ <= round) {
		buf_0[0] = _rand_uint32();
		buf_0[1] = _rand_uint32();
		buf_0[2] = _rand_uint32();
		buf_0[3] = _rand_uint32();
		buf_0[4] = _rand_uint32();
		buf_0[5] = _rand_uint32() % 0x3ff;

		ret = jl_apb_reg_burst_write(0x102aee, &buf_0[0], 8);
		if (ret) {
			printf("####error[%d]int func[%s] line[%d]\n", ret, __func__, __LINE__);
			goto exit;
		}
		ret = jl_apb_reg_burst_read(0x102aee, &buf[0], 8);
		if (ret) {
			printf("####error[%d]int func[%s] line[%d]\n", ret, __func__, __LINE__);
			goto exit;
		}

		for (i = 0; i < 8; i++)
			if (buf[i] != buf_0[i]) {
				printf("ERROR write != read, count:%d\n", count);
				printf("read index: %d, value: 0x%08x, write: 0x%08x\n", i, buf[i], buf_0[i]);
			}
	}


exit:
	jl_reg_io_deinit();

	return 0;
}
