#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

#include "jl_base.h"
#include "storm.h"

int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;

	/* Enable Strom control on port 0 for all kind of packet.*/
	/* Set the control rate of broadcast storm on port 0 to 5Mbps */
	/* Set the control rate of multicast storm on port 0 to 2000pps */
	jl_switch_init();

	jl_storm_control_enable_set(UTP_PORT0, STORM_TYPE_BROADCAST, 1);

	jl_storm_control_enable_set(UTP_PORT0, STORM_TYPE_MULTICAST, 1);

	jl_storm_control_enable_set(UTP_PORT0, STORM_TYPE_UNKNOWN, 1);

	jl_storm_control_rate_set(UTP_PORT0, STORM_TYPE_BROADCAST, RATE_UNIT_KBPS, 5000);

	jl_storm_control_rate_set(UTP_PORT0, STORM_TYPE_MULTICAST, RATE_UNIT_PPS, 2000);

	return 0;
}
