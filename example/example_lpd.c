#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "lpd.h"


int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;
	jl_port_t port;
	jl_lpd_config_t lpd_config = {
		/* TODO a random MAC Address */
		.src_mac.octet = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55},
		.eth_type = 0x9988,
		.op_code = 0x1,
		.ttl = 15,
	};


	/* init sdk */
	jl_switch_init();

	/* 1. set loop frame format */
	jl_lpd_config_set(&lpd_config);

	/* 2. enable loop detection */
	jl_lpd_enable();

	/* 3. trigger loop detection periodically */
	while (1) {
		JL_FOR_EACH_PHY_PORT(port)
			jl_lpd_trigger_once(port);
		sleep(1);
	}

	return 0;
}
