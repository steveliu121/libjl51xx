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


static void vlan_table_test(void)
{
	struct vlan_config vlan_cfg = {0};
	struct vlan_config vlan_cfg_tmp = {0};
	jl_uint32 vlan_table_index = 0;
	jl_api_ret_t ret = 0;
    
	/* init vlan */
	ret = jl_vlan_init();
	if (ret != JL_ERR_OK) {
		printf("JL vlan init fail\n");
		return;
	}

	for (vlan_table_index = 1; vlan_table_index <= 5; vlan_table_index++)
	{
		memset(&vlan_cfg, 0, sizeof(vlan_cfg));
		vlan_cfg.valid = 1;
		vlan_cfg.vid = 100 + vlan_table_index;
		vlan_cfg.port_mask = (1 << UTP_PORT0) | (1 << UTP_PORT1) | (1 << UTP_PORT2);
		vlan_cfg.fid = 0;
		vlan_cfg.operation = vlan_table_index;
		if (vlan_cfg.operation == VLAN_OPERATION_SWAP) {
			vlan_cfg.vid_sel = VLANID_INHERIT_FROM_TABLE_VALUE;
			vlan_cfg.new_vid =  200 + vlan_table_index;
		} else if (vlan_cfg.operation == VLAN_OPERATION_PUSH) {
			vlan_cfg.vid_sel = VLANID_INHERIT_FROM_ORIGINAL_OUTERMOST;
			vlan_cfg.new_vid =  200 + vlan_table_index;
		}

		ret = jl_vlan_set(vlan_table_index, &vlan_cfg);
		if (ret != JL_ERR_OK) {
			printf("jl_vlan_set fail, index %d return  %d!\n", vlan_table_index, ret);
		}

		memset(&vlan_cfg_tmp, 0, sizeof(vlan_cfg_tmp));
		ret = jl_vlan_get(vlan_table_index, &vlan_cfg_tmp);
		if (ret != JL_ERR_OK) {
			printf("jl_vlan_get fail, index %d return  %d!\n", vlan_table_index, ret);
		}
		else {
			printf("vlan table entry %d: valid %d, vlanid %d, portmask 0x%x, fid %d, operation %d, vid_sel %d, new_vid %d.\n",\
		  		vlan_table_index, vlan_cfg_tmp.valid, vlan_cfg_tmp.vid, vlan_cfg_tmp.port_mask, \
		  		vlan_cfg_tmp.fid, vlan_cfg_tmp.operation, vlan_cfg_tmp.vid_sel, vlan_cfg_tmp.new_vid);
		}
	}
	return;
}

/* example for trunk port 
if pkt vlan tag == default vid, then need to remove vlan tag to transmit; 
if pkt vlan tag != default vid, then transmit directly.*/
static void vlan_trunk_tx_test(void)
{
    struct vlan_config vlan_cfg0 = {0};
	struct vlan_config vlan_cfg1 = {0};
	struct vlan_config vlan_cfg_tmp = {0};
	jl_uint32 vlan_table_index = 0;
	jl_api_ret_t ret = 0;

	ret = jl_vlan_init();
	if (ret != JL_ERR_OK) {
		printf("JL vlan init fail\n");
		return;
	}

	/* 1. set utp_port0 as vlan trunk port, default vid=3  */ 
	jl_port_vlan_set(UTP_PORT0, 3);
	jl_port_vlan_assignment_set(UTP_PORT0, VLAN_ASSIGN_PKT);
	jl_vlan_acpt_frmt_set(UTP_PORT0, VLAN_ACCEPT_FRAME_TYPE_ALL);
	
	/* 2. set vlan 3 members {UTP_PORT0, UTP_PORT1, UTP_PORT2} and vlan operation = remove all*/
	vlan_cfg0.valid = 1; 
	vlan_cfg0.vid = 3;
	vlan_cfg0.port_mask = (1 << UTP_PORT1) | (1 << UTP_PORT2) | (1 << UTP_PORT0);
	vlan_cfg0.operation = VLAN_OPERATION_REMOVE_ALL;
	vlan_table_index = 1;
	ret = jl_vlan_set(vlan_table_index, &vlan_cfg0);
	if (ret != JL_ERR_OK) {
		printf("jl_vlan_set fail, index %d return  %d!\n", vlan_table_index, ret);
	}

	memset(&vlan_cfg_tmp, 0, sizeof(vlan_cfg_tmp));
	ret = jl_vlan_get(vlan_table_index, &vlan_cfg_tmp);
	if (ret == JL_ERR_OK) {
		printf("vlan table entry %d: valid %d, vlanid %d, portmask 0x%x, fid %d, operation %d, vid_sel %d, new_vid %d.\n",\
		  		vlan_table_index, vlan_cfg_tmp.valid, vlan_cfg_tmp.vid, vlan_cfg_tmp.port_mask, \
		  		vlan_cfg_tmp.fid, vlan_cfg_tmp.operation, vlan_cfg_tmp.vid_sel, vlan_cfg_tmp.new_vid);
	} else {
		printf("jl_vlan_get fail, index %d return  %d!\n", vlan_table_index, ret);
	}

	/* 3. set vlan 4 members {UTP_PORT0, UTP_PORT1}*/
	vlan_cfg1.valid = 1;
	vlan_cfg1.vid = 4;
	vlan_cfg1.port_mask = (1 << UTP_PORT0) | (1 << UTP_PORT1);
	vlan_table_index = 2;
	ret = jl_vlan_set(vlan_table_index, &vlan_cfg1);
	if (ret != JL_ERR_OK) {
		printf("jl_vlan_set fail, index %d return  %d!\n", vlan_table_index, ret);
	}
    
	memset(&vlan_cfg_tmp, 0, sizeof(vlan_cfg_tmp));
	ret = jl_vlan_get(vlan_table_index, &vlan_cfg_tmp);
	if (ret == JL_ERR_OK) {
		printf("vlan table entry %d: valid %d, vlanid %d, portmask 0x%x, fid %d, operation %d, vid_sel %d, new_vid %d.\n",\
		  		vlan_table_index, vlan_cfg_tmp.valid, vlan_cfg_tmp.vid, vlan_cfg_tmp.port_mask, \
		  		vlan_cfg_tmp.fid, vlan_cfg_tmp.operation, vlan_cfg_tmp.vid_sel, vlan_cfg_tmp.new_vid);
	} else {
		printf("jl_vlan_get fail, index %d return  %d!\n", vlan_table_index, ret);
	}
	return;
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
		fprintf(stdout, "1. vlan_table_test \n"); 
		fprintf(stdout, "2. vlan_trunk_tx_test \n"); 
		fprintf(stdout, "Type a number or 'q' to exit\n");

		fgets(console_input, 127, stdin);
		if (!strncmp(console_input, "q", 1))
			goto exit;
		else
			cmd_index = atoi(console_input);

		clear_window();

		switch (cmd_index) {
		case 1:
			vlan_table_test();
			break;
		case 2:
			vlan_trunk_tx_test();
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

