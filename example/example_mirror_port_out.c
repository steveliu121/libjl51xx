#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

#include "mirror.h"

int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;

	/* init sdk */
	jl_switch_init();

	/* 1. set port output mirror */
	jl_mirror_output_mr_set(UTP_PORT2, UTP_PORT3);

	return 0;
}
