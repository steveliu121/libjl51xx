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

	/* add a multicast member port 0,1,2 to the L2 Multicast Table index 2.	*/
	/* add a multicast static entry with MAC address 01:00:5E:01:01:01, fid 5 and member port 0,1,2	*/
	jl_uint32 portmask;
	jl_l2_table_entry_idx_t index;

	portmask = (1 << UTP_PORT0) | (1 << UTP_PORT1) | (1 << UTP_PORT2);
	index = 2;

	jl_l2_fib_key_t key = {
		.is_collision = 0, //ignored
		.mac_addr.val = 0x01005E010101, //mac
		.fid = 5, //fid
		.entry_index = 0, //ignored
	};

	jl_l2_fib_config_t config = {
		.aging = {
			.is_valid = 1, //valid
			.is_static = 1, //static
		},
		.dest = {
			.is_unicast = 0, //multicast
			.port = index, //pointer to multicast table index
			.pkt_drop = 0,
		},
	};

	/* init sdk */
	jl_switch_init();

	/* set L2 multicast entry */
	jl_l2_mc_port_mask_set(index, portmask);

	jl_l2_fib_config_set_auto(&key, &config);

	return 0;
}
