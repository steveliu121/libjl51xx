#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

#include "vlan.h"

#define ARRAY_SIZE(xs)	(sizeof(xs) / sizeof(xs[0]))

static int g_exit;

static void sig_handle(int sig)
{
	(void) sig;
	g_exit = 1;
}

static void clear_window(void)
{
	fprintf(stdout, "\033c");
}


static void mtu_vlan_test(void)
{

	/* 1. set Port VID: port0_pvid=100, port1_pvid=200 */
	jl_port_vlan_set(UTP_PORT0, 100);
	jl_port_vlan_set(UTP_PORT1, 200);

	/* 2. set VLAN Assignment: port0=port based; port1=port based; port2=packet based */
	jl_port_vlan_assignment_set(UTP_PORT0, VLAN_ASSIGN_PORT);
	jl_port_vlan_assignment_set(UTP_PORT1, VLAN_ASSIGN_PORT);
	jl_port_vlan_assignment_set(UTP_PORT2, VLAN_ASSIGN_PKT);

	/* 3. only accept untagged frames on port0 & port1, tagged frames from port0 & port1 will be dropped. port2 accepts all type frames */
	jl_vlan_acpt_frmt_set(UTP_PORT0, VLAN_ACCEPT_FRAME_TYPE_UNTAG_ONLY);
	jl_vlan_acpt_frmt_set(UTP_PORT1, VLAN_ACCEPT_FRAME_TYPE_UNTAG_ONLY);
	jl_vlan_acpt_frmt_set(UTP_PORT2, VLAN_ACCEPT_FRAME_TYPE_ALL);

	/* 4. set port ingress direction VLAN Operation: port0=push; port1=push; port2=pop */
	jl_port_vlan_operation_set(UTP_PORT0, PORT_DIR_INGRESS, VLAN_OPERATION_PUSH);
	jl_port_vlan_operation_set(UTP_PORT1, PORT_DIR_INGRESS, VLAN_OPERATION_PUSH);
	jl_port_vlan_operation_set(UTP_PORT2, PORT_DIR_INGRESS, VLAN_OPERATION_POP);

	/* 5. set port egress direction VLAN Operation: port0=remove all; port1=remove all;*/
	jl_port_vlan_operation_set(UTP_PORT0, PORT_DIR_EGRESS, VLAN_OPERATION_REMOVE_ALL);
	jl_port_vlan_operation_set(UTP_PORT1, PORT_DIR_EGRESS, VLAN_OPERATION_REMOVE_ALL);

	/* 6. create VLAN 100 for port0 & port2; create VLAN 200 for port1 & port2; both are SVL */
	struct vlan_config vlan_cfg0 = {0};
	struct vlan_config vlan_cfg1 = {0};

	vlan_cfg0.valid = 1;
	vlan_cfg0.vid = 100;
	vlan_cfg0.port_mask = (1 << UTP_PORT0) | (1 << UTP_PORT2);
	vlan_cfg0.fid = 0;

	vlan_cfg1.valid = 1;
	vlan_cfg1.vid = 200;
	vlan_cfg1.port_mask = (1 << UTP_PORT1) | (1 << UTP_PORT2);
	vlan_cfg1.fid = 0;

	jl_vlan_set(1, &vlan_cfg0);
	jl_vlan_set(2, &vlan_cfg1);
}

int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;
	char console_input[128] = {0};
	int cmd_index = -1;
	jl_error_code_t ret;

	signal(SIGINT, sig_handle);
	signal(SIGTERM, sig_handle);

	ret = jl_switch_init();
	if (ret) {
		printf("JL SWITCH init fail\n");
		return -1;
	}
	/* init vlan */
	ret = jl_vlan_init();
	if (ret) {
		printf("JL vlan init fail\n");
		return -1;
		}

	while (!g_exit) {
		fprintf(stdout, "\n======JL SWITCH: %s=======\n", argv[0]);
		fprintf(stdout, "1. MTU Vlan test\n");
		fprintf(stdout, "Type a number or 'q' to exit\n");

		fgets(console_input, 127, stdin);
		if (!strncmp(console_input, "q", 1))
			goto exit;
		else
			cmd_index = atoi(console_input);

		clear_window();

		switch (cmd_index) {
		case 1:
			mtu_vlan_test();
			break;
		default:
			fprintf(stdout, "Unknown input message\n");
			break;
		}

		getchar();

		clear_window();
	};

exit:
	jl_switch_deinit();

	return 0;
}

