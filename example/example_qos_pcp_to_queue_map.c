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
	jl_uint32 i = 0;
	jl_uint32 pcp[8] = {7, 6, 5, 4, 3, 2, 1, 0};
	jl_uint32 queue[8] = {0, 1, 2, 3, 3, 2, 1, 0};
	jl_port_t port[8] = {UTP_PORT0, UTP_PORT1, UTP_PORT2, UTP_PORT3, UTP_PORT4, UTP_PORT5, UTP_PORT6, UTP_PORT7};

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

	for (i = 0; i < 8; i++) {
		ret = jl_qos_l2_port_pcp_set(port[i], pcp[i]);
		if (ret) {
			/*throw exception*/
			return ret;
		}

		ret = jl_qos_l2_pcp2queue_map_set(pcp[i], queue[i]);
		if (ret) {
			/*throw exception*/
			return ret;
		}
	}
	return 0;
}


