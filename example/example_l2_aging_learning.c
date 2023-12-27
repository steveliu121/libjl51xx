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

	/* turn off auto-learning and aging */
	/* init sdk */
	jl_switch_init();

	jl_l2_aging_learning_set(0, 0);

	return 0;
}
