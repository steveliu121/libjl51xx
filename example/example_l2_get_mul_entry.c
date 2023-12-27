#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

#include "jl_base.h"
#include "l2.h"

int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;

	/* get a multicast entry with MAC address 01:00:5E:01:01:01, fid 1, and its multicast port mask */
	jl_uint32 portmask;
	jl_l2_fib_config_t config;

	jl_l2_fib_key_t key = {
		.is_collision = 0,
		.mac_addr.val = 0x01005E010101, //mac
		.fid = 1, //fid
		.entry_index = 0,
	};
	portmask = 0;
	memset(&config, 0, sizeof(jl_l2_fib_config_t));

	/* init sdk */
	jl_switch_init();

	jl_l2_fib_config_get(&key, &config);

	/* get the multicast member port mask */
	jl_l2_mc_port_mask_get(config.dest.port, &portmask);

	return 0;
}
