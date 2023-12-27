#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

#include "jl_base.h"
#include "qos.h"

int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;
	jl_api_ret_t ret = JL_ERR_OK;
	jl_port_t port[4] = {UTP_PORT0, UTP_PORT1, UTP_PORT2, EXT_PORT0};
	jl_uint32 pcp[4] = {0, 1, 2, 3};
	jl_uint32 queue[4] = {0, 1, 2, 3};
	jl_uint32 queue_weight[4] = {1, 2, 4, 8};
	jl_qos_schedule_t t_sche;
	jl_uint8 i;

	ret = jl_switch_init();
	if (ret) {
		/*throw exception*/
		return ret;
	}

	ret = jl_qos_init();
	if (ret) {
		/*throw exception*/
		return ret;
	}

	/*DWRR scheduling based on PCP assigned queue*/
	for (i = 0; i < 4;  i++) {
		/*set port pcp*/
		ret = jl_qos_l2_port_pcp_set(port[i], pcp[i]);
		if (ret) {
			/*throw exception*/
			return ret;
		}
		/*map pcp to queue*/
		ret = jl_qos_l2_pcp2queue_map_set(pcp[i], queue[i]);
		if (ret) {
			/*throw exception*/
			return ret;
		}
		/*set queue dwrr weight*/
		/*only when the queue has joined in DWRR scheduler it's weight value can be effective*/
		ret = jl_qos_queue_dwrr_set(port[i], queue[i], queue_weight[i]);
		if (ret) {
			/*throw exception*/
			return ret;
		}
	}
	/*queue 0、3 join in SP, queue 1、2 join in DWRR*/
	t_sche.type[0] = QOS_SCHE_SP;
	t_sche.type[1] = QOS_SCHE_DWRR;
	t_sche.type[2] = QOS_SCHE_DWRR;
	t_sche.type[3] = QOS_SCHE_SP;

	for (i = 0; i < 4; i++)
	{
		/*set schedule type*/
		ret = jl_qos_schedule_set(port[i], &t_sche);
		if (ret)
			return ret;
	}

	for (i = 0; i < 4; i++)
	{
		ret = jl_qos_schedule_get(port[i], &t_sche);
		if (ret)
			return ret;
	}
	
	return 0;
}


