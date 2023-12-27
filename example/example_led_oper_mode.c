#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

#include "led.h"

int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;

	/* Use group 1, and the port is Port 0 ~ Port 7*/
	/* Set blinking rate to 128ms*/
	/* Set LED mode group 1 to indicate operation in 10M or not */
	jl_switch_init();

	jl_led_enable_set(LED_GROUP1, ((1 << UTP_PORT0) | (1 << UTP_PORT1) | (1 << UTP_PORT2) \
		| (1 << UTP_PORT3) | (1 << UTP_PORT4) | (1 << UTP_PORT5) | (1 << UTP_PORT6) | (1 << UTP_PORT7)));

	jl_led_blink_rate_set(LED_BLINKRATE_128MS);

	jl_led_group_config_set(LED_GROUP1, LED_CONFIG_SPD10);

	return 0;
}
