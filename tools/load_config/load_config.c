#include <stdlib.h>

#include "portable/jl_portable.h"
#include "driver/jl_reg_io.h"
#include "config.h"

#define ARRAY_SIZE(x)		(sizeof(x)/sizeof((x)[0]))
#define PHY_ID(x)		(((x) >> 0x15U) & 0x1FU)
#define PAGE_ID(x)		(((x) >> 0x5U) & 0xFFFFU)
#define REG_ID(x)		((x) & 0x1FU)
#define MAX_BURST_SIZE		8U

#define CONFIG_DEBUG_ON
#ifdef CONFIG_DEBUG_ON
#define LOG		printf
#else
#define LOG(...)
#endif


static int __do_write(const struct cmd_line_s *cmd_line)
{
	jl_api_ret_t ret = JL_ERR_OK;

	if ((cmd_line->flag != JL_CMD_FLAG_DA) &&
			((cmd_line->flag != JL_CMD_FLAG_IDA))) {
		LOG("Abort, Unknown WRITE command flag!!!\n");
		return -1;
	}

	if (cmd_line->flag == JL_CMD_FLAG_DA) {
		jl_phy_reg_write_ext(PHY_ID(cmd_line->addr),
						PAGE_ID(cmd_line->addr),
						REG_ID(cmd_line->addr),
						(jl_uint16)cmd_line->val);
	} else if (cmd_line->flag == JL_CMD_FLAG_IDA) {
		ret = jl_apb_reg_write(cmd_line->addr, cmd_line->val);
		if (ret)
			return -1;
	}

	return 0;
}

static int __do_burst_write(const struct cmd_line_s *cmd_line)
{
	jl_api_ret_t ret = JL_ERR_OK;
	jl_uint32 burst_val[8] = {0};
	jl_uint32 size = (jl_uint32)cmd_line->flag;
	unsigned int i = 0;

	if ((size == 0) || (size > MAX_BURST_SIZE)) {
		LOG("Abort, BURST WRITE command with wrong size!!!\n");
		return -1;
	}

	for (i = 0; i < size; i++) {
		if ((i > 0)  && (cmd_line[i].cmd ||
				cmd_line[i].flag ||
				cmd_line[i].addr)) {
			LOG("Abort, BRUST WRITE with wrong burst value format!!!\n");
			return -1;
		}

		burst_val[i] = cmd_line[i].val;
	}

	ret = jl_apb_reg_burst_write(cmd_line->addr, &burst_val[0], size);
	if (ret)
		return -1;

	return (int)size;
}

static int load_config(void)
{
	unsigned int size = ARRAY_SIZE(cmd_line_set);
	unsigned int i = 0;
	const struct cmd_line_s *cmd_line = NULL;
	int ret = 0;

	while (size) {
		cmd_line = &cmd_line_set[i];

		switch (cmd_line->cmd) {
		case (JL_CMD_WRITE):
			ret = __do_write(cmd_line);
			if (ret)
				goto error;

			size--;
			i++;
			break;
		case (JL_CMD_BWRITE):
			ret = __do_burst_write(cmd_line);
			if (ret < 0)
				goto error;

			size -= ret;
			i += ret;
			break;
		case (JL_CMD_DELAY):
			port_udelay(cmd_line->val);

			size--;
			i++;
			break;
		default:
			LOG("Abort, Unknown command!!!\n");
			goto error;
		}
	}

	return 0;

error:
	LOG("Load configuration fail!!!\n");
	LOG("CMD Line: CMD:[%d], FLAG:[%d], ADDR:[0x%8x], VALUE:[0x%8x]\n",
						cmd_line->cmd, cmd_line->flag,
						cmd_line->addr, cmd_line->val);

	return -1;
}

int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;
	jl_api_ret_t ret;

	ret = jl_reg_io_init();
	if (ret)
		return -1;

	load_config();

	jl_reg_io_deinit();

	return ret;
}
