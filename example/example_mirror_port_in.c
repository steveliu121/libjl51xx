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

	/* 1. set port input mirror */
	jl_mirror_input_mr_set(UTP_PORT0, UTP_PORT2);

	return 0;
}
