#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

#include "acl.h"

int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;

	jl_api_ret_t ret;
	jl_acl_rule_t acl_rule;
	jl_acl_action_t acl_action;

	/* init sdk */
	jl_switch_init();

	/* set entry 0 rule*/
	memset(&acl_rule, 0, sizeof(jl_acl_rule_t));
	acl_rule.compare_sa = 1;
	acl_rule.sa_mac = 0x11;
	acl_rule.type_of_comparison_sa = 1;
	acl_rule.ports = 1 << UTP_PORT0;

	memset(&acl_action, 0, sizeof(jl_acl_action_t));
	acl_action.force_queue_enable = ENABLED;
	acl_action.queue_id = 0;
	ret = jl_acl_set(0, &acl_rule, &acl_action);

	if (ret) {
		/*throw exception*/
		return ret;
	}

	/* set entry 1 rule */
	memset(&acl_rule, 0, sizeof(jl_acl_rule_t));
	acl_rule.compare_sa = 1;
	acl_rule.sa_mac = 0x22;
	acl_rule.type_of_comparison_sa = 1;
	acl_rule.ports = 1 << UTP_PORT1;

	memset(&acl_action, 0, sizeof(jl_acl_action_t));
	acl_action.force_queue_enable = ENABLED;
	acl_action.queue_id = 1;
	ret = jl_acl_set(1, &acl_rule, &acl_action);

	if (ret) {
		/*throw exception*/
		return ret;
	}

	/* set entry 2 rule */
	memset(&acl_rule, 0, sizeof(jl_acl_rule_t));
	acl_rule.compare_da = 1;
	acl_rule.da_mac = 0x33;
	acl_rule.type_of_comparison_da = 1;
	acl_rule.compare_vid = 1;
	acl_rule.vid = 100;
	acl_rule.type_of_comparison_vid = 1;
	acl_rule.ports = (1 << UTP_PORT2) + (1 << UTP_PORT3);

	memset(&acl_action, 0, sizeof(jl_acl_action_t));
	acl_action.force_queue_enable = ENABLED;
	acl_action.queue_id = 0;
	acl_action.send_to_port_enable = ENABLED;
	acl_action.dest_port = UTP_PORT4;
	ret = jl_acl_set(2, &acl_rule, &acl_action);

	if (ret) {
		/*throw exception*/
		return ret;
	}

	return 0;
}
