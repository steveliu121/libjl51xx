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

	/* add a unicast static entry with MAC address 00:11:22:33:44:55, untagged, destport 5 */
	jl_l2_fib_key_t key = {
		.is_collision = 0,
		.mac_addr.val = 0x1122334455,	//mac
		.fid = 1,	//fid
		.entry_index = 0,
	};

	jl_l2_fib_config_t config = {
		.aging = {
			.is_valid = 1, //valid
			.is_static = 1, //static
		},
		.dest = {
			.is_unicast = 1, //unicast
			.port = UTP_PORT5, //dest port
			.pkt_drop = 0,
		},
	};

	/* init sdk */
	jl_switch_init();

	/* set aging time to 300s */
	jl_l2_aging_time_set(300, L2_AGING_S);

	jl_l2_fib_config_set_auto(&key, &config);

	return 0;
}
