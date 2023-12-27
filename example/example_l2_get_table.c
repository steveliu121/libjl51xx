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

	/* dump the 3th(0 - 2063) index of the L2 table */
	int i;
	jl_l2_fib_key_t key;
	jl_l2_fib_config_t config;

	i = 3;
	memset(&key, 0, sizeof(jl_l2_fib_key_t));
	memset(&config, 0, sizeof(jl_l2_fib_config_t));

	/* init sdk */
	jl_switch_init();

	jl_l2_fib_config_get_by_index(i, &key, &config);
	/* dump all the L2 table entires whose dest port equals UTP_PORT0 */

	for (i = 0; i < JL_L2_DA_TABLE_SIZE; i++) {
		jl_l2_fib_config_get_by_index(i, &key, &config);

		if (config.dest.port == UTP_PORT0) {
			/* do some thing */
		}
	}

	return 0;
}
