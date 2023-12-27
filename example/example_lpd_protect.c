#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

#include "lpd.h"
#include "stp.h"
#include "port.h"

typedef struct lpd_status_s {
	jl_uint8 is_blocked;
	jl_uint8 is_in_loop;
	jl_int16 ports_in_loop[SWITCH_PORT_NUM - 1];
	jl_uint8 ports_in_loop_count;
} lpd_status_t;

lpd_status_t lpd_status_array[SWITCH_PORT_NUM];


int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;
	jl_port_t port;
	jl_port_t port_m;
	jl_port_t port_in_loop;
	jl_portmask_t port_mask = 0;
	jl_port_duplex_t duplex;
	jl_port_speed_t speed;
	jl_port_link_status_t link_status;
	jl_lpd_config_t lpd_config = {
		/* TODO a random MAC Address */
		.src_mac.octet = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55},
		.eth_type = 0x9988,
		.op_code = 0x1,
		.ttl = 15
};


	/* init sdk */
	jl_switch_init();

	/* clear lpd_status for all ports */
	memset(lpd_status_array, 0, (sizeof(lpd_status_t)*SWITCH_PORT_NUM));


	/* 1. set loop frame format */
	jl_lpd_config_set(&lpd_config);

	/* 2. enable loop detection */
	jl_lpd_enable();

	/* 3. trigger loop detection and block loop ports periodically */
	while (1) {
		/*clear when one loop start*/
		JL_FOR_EACH_PHY_PORT(port) {
			lpd_status_array[port].is_in_loop = FALSE;
			lpd_status_array[port].ports_in_loop_count = 0;
			memset(lpd_status_array[port].ports_in_loop, 0, sizeof(jl_int16)*(SWITCH_PORT_NUM - 1));
		}
		/* get port's lpd staus, before that ,the port should be is_in_loop=false and link up */
		JL_FOR_EACH_PHY_PORT(port) {
			if (lpd_status_array[port].is_in_loop)
				continue;
			 jl_port_phy_link_status_get(port, &link_status, &duplex, &speed);

			 /*use time out err presents port link down, it is not accurately*/
			if (-10 == jl_lpd_trigger_once(port)) {
				 if (link_status == PORT_LINKUP && port < 8) {
					printf("Port %d trigger loop frame time out, pls fix this issue at first.", port);
					return 0;
				 }
				continue;
			}

			jl_lpd_lp_ports_get(&port_mask);

			/*get ports in loop based on port_mask */
			JL_FOR_EACH_PHY_PORT(port_m) {
				if (((port_mask >> port_m) & 0x01) == 1) {
					lpd_status_array[port_m].is_in_loop = TRUE;
					lpd_status_array[port].ports_in_loop[lpd_status_array[port].ports_in_loop_count] = port_m;
					lpd_status_array[port].ports_in_loop_count++;
				}
			}
		}
		/* block port based on lpd staus */
		JL_FOR_EACH_PHY_PORT(port) {
			if (lpd_status_array[port].is_in_loop)
				continue;
			//it means port is not one port in ports__in_loop, need to clear the blocked status
			else if (lpd_status_array[port].is_blocked) {		
				jl_stp_port_egress_state_set(port, STP_STATE_FORWARDING);
				jl_stp_port_ingress_state_set(port, STP_STATE_FORWARDING);
				lpd_status_array[port].is_blocked = FALSE;
			}
			if (lpd_status_array[port].ports_in_loop_count > 0) {

				/* go through the ports in loop */
				for (int i = 0; i < lpd_status_array[port].ports_in_loop_count; i++) {

					port_in_loop = lpd_status_array[port].ports_in_loop[i];

					if (!lpd_status_array[port_in_loop].is_blocked) {			
						jl_stp_port_egress_state_set(port_in_loop, STP_STATE_BLOCKING);
						jl_stp_port_ingress_state_set(port_in_loop,  STP_STATE_BLOCKING);
						lpd_status_array[port_in_loop].is_blocked = TRUE;
					}
				}	
			} 		
		}
		//sleep(100);
	}
	return 0;
}


