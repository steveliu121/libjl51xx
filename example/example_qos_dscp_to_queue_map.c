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
	jl_uint32 dscp[4] = {0, 10, 20, 63};
	jl_uint32 queue[4] = {0, 1, 2, 3};
	jl_port_t port[4] = {UTP_PORT0, UTP_PORT1, UTP_PORT2, UTP_PORT3};
	jl_uint32 i = 0;

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

	for (i = 0; i < 4; i++) {
		ret = jl_qos_l3_queue_assign_enable_set(port[i], ENABLED);
		if (ret) {
			/*throw exception*/
			return ret;
		}

		ret = jl_qos_l3_dscp2queue_map_set(dscp[i], queue[i]);
		if (ret) {
			/*throw exception*/
			return ret;
		}
	}
	return 0;
}


