#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

#include "jl_base.h"
#include "mib.h"

int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;

	/* Set port 0 mib counter mode as read-clear */
	/* Get number of received valid unicast packets of port 0 */
	/* Clear all types of mib counter of port 0 */
	jl_uint64 counter = 0;
	jl_ret_t ret = JL_ERR_OK;
	jl_mib_swc_glb_drop_t port_cnt = {0};
	jl_port_t port = 0;
	jl_mib_swc_port_drop_t swc_port_cnt = {0};

	jl_switch_init();

	jl_mib_port_rc_mode_set(UTP_PORT0, 1);

	jl_mib_port_get(UTP_PORT0, PORT_MIB_TX_PKT_UC_CNT, &counter);

	jl_mib_port_clear_all(UTP_PORT0);

	ret = jl_mib_swc_global_drop_get(&port_cnt);
	if (ret)
		printf("jl_mib_swc_global_drop_get return err 0x%x\n", ret);
	printf("buf_overflow_drop:%d\n", port_cnt.buf_overflow_drop);
	printf("empty_mask_drop:%d\n", port_cnt.empty_mask_drop);
	printf("epp_pm_drop:%d\n", port_cnt.epp_pm_drop);
	printf("ingress_config_acl_drop:%d\n", port_cnt.ingress_config_acl_drop);
	printf("ingress_res_manager_drop:%d\n", port_cnt.ingress_res_manager_drop);
	printf("ingress_stp_block_drop:%d\n", port_cnt.ingress_stp_block_drop);
	printf("ingress_stp_learn_drop:%d\n", port_cnt.ingress_stp_learn_drop);
	printf("ingress_stp_listen_drop:%d\n", port_cnt.ingress_stp_listen_drop);
	printf("ipp_broken_pkt:%d\n", port_cnt.ipp_broken_pkt);
	printf("ipp_pm_drop:%d\n", port_cnt.ipp_pm_drop);
	printf("l2_flag_drop:%d\n", port_cnt.l2_lookup_drop);
	printf("max_allow_vlan_drop:%d\n", port_cnt.max_allow_vlan_drop);
	printf("mini_allow_vlan_drop:%d\n", port_cnt.mini_allow_vlan_drop);
	printf("re_queue_overflow_drop:%d\n", port_cnt.re_queue_overflow_drop);
	printf("unknown_ingress_drop:%d\n", port_cnt.unknown_ingress_drop);
	printf("vlan_member_drop:%d\n", port_cnt.vlan_member_drop);

	for (port = 0; port < JL_PORT_MAX; port++) {
		printf("=========port %d:==========\n", port);
		memset(&swc_port_cnt, 0, sizeof(swc_port_cnt));
		ret = jl_mib_swc_port_drop_get(port, &swc_port_cnt);
		if (ret)
			printf("jl_mib_swc_port_drop_get return err 0x%x\n", ret);
		printf("drain_port_drop:%d\n", swc_port_cnt.drain_port_drop);
		printf("egress_port_dis_drop:%d\n", swc_port_cnt.egress_port_dis_drop);
		printf("egress_res_manage_drop:%d\n", swc_port_cnt.egress_res_manage_drop);
		printf("egress_stp_drop:%d\n", swc_port_cnt.egress_stp_drop);
		printf("mac_rx_broken_pkt:%d\n", swc_port_cnt.mac_rx_broken_pkt);
		printf("mac_rx_short_pkt_drop:%d\n", swc_port_cnt.mac_rx_short_pkt_drop);
		printf("mbsc_drop:%d\n", swc_port_cnt.mbsc_drop);
		printf("ps_err_overflow:%d\n", swc_port_cnt.ps_err_overflow);
		printf("ps_err_underrun:%d\n", swc_port_cnt.ps_err_underrun);
		printf("queue_off_drop:%d\n", swc_port_cnt.queue_off_drop);
		printf("sp_overflow_drop:%d\n", swc_port_cnt.sp_overflow_drop);
		printf("unknown_egress_drop:%d\n", swc_port_cnt.unknown_egress_drop);
		printf("=========end==========\n");
	}

	ret = jl_mib_swc_global_drop_clear();
	if (ret)
		printf("jl_mib_swc_global_drop_clear return err 0x%x\n", ret);
	for (port = 0; port < JL_PORT_MAX; port++) {
		printf("=========port %d:==========\n", port);
		ret = jl_mib_swc_port_drop_clear(port);
		if (ret)
			printf("jl_mib_swc_port_drop_clear return err 0x%x\n", ret);
	}
	return 0;
}
