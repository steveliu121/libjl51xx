#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

#include "jl_base.h"
#include "port.h"

int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;

	jl_port_ext_mac_ability_t ability;
	jl_port_mac_ability_t cur_ability;

	jl_switch_init();

	/* Force the MAC of EXT_PORT 1 working with 100F */
	/* and Symmetric PAUSE flow control abilities */
	memset(&ability, 0x00, sizeof(jl_port_ext_mac_ability_t));
	jl_port_mac_force_link_ext_get(EXT_PORT1, &ability);
	ability.force_mode = 1;
	ability.speed = 1;
	ability.duplex = 1;
	ability.link = 1;
	ability.tx_pause = 1;
	ability.rx_pause = 1;
	jl_port_mac_force_link_ext_set(EXT_PORT1, &ability);

	/* Get MAC link status of UTP PORT 1 */
	memset(&cur_ability, 0x00, sizeof(jl_port_mac_ability_t));
	jl_port_mac_status_get(UTP_PORT1, &cur_ability);

	return 0;
}
