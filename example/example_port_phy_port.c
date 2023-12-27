#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

#include "jl_base.h"
#include "port.h"

int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;
	int ret = 0;
	jl_port_t port;

	jl_port_phy_ability_t ability;
	jl_port_speed_t speed;
	jl_port_duplex_t duplex;
	jl_port_link_status_t linkst;

	jl_switch_init();

	/* set PHY 1 with Auto negotiation, 100F,10F */
	/* and Symmetric PAUSE flow control abilities */
	memset(&ability, 0x00, sizeof(jl_port_phy_ability_t));
	ability.full_duplex_100 = 1;
	ability.full_duplex_10 = 1;
	ability.flow_control = 1;
	ability.asym_flow_control = 1;
	ret = jl_port_phy_autoneg_ability_set(UTP_PORT1, &ability);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, " UTP_PORT1 jl_port_phy_autoneg_ability_set ret err %d\n", ret);

	/* set PHY 2 with Auto negotiation, 100h, without flow control */
	memset(&ability, 0x00, sizeof(jl_port_phy_ability_t));
	ability.half_duplex_100 = 1;
	ret = jl_port_phy_autoneg_ability_set(UTP_PORT2, &ability);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "UTP_PORT2 jl_port_phy_autoneg_ability_set ret err %d\n", ret);

	/* set PHY 3 with Auto negotiation, 10h, without flow control */
	memset(&ability, 0x00, sizeof(jl_port_phy_ability_t));
	ability.half_duplex_10 = 1;
	ret = jl_port_phy_autoneg_ability_set(UTP_PORT3, &ability);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "UTP_PORT3 jl_port_phy_autoneg_ability_set ret err %d\n", ret);

	JL_FOR_EACH_PHY_PORT(port) {
		/* Get ability of PHY port */
		memset(&ability, 0x00, sizeof(jl_port_phy_ability_t));
		ret = jl_port_phy_autoneg_ability_get(port, &ability);
		if (ret)
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_phy_autoneg_ability_get ret err %d\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_phy_autoneg_ability_get  port %d\n", port);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "auto_negotiation is %d\n", ability.auto_negotiation);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "half_duplex_10 is %d\n", ability.half_duplex_10);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "full_duplex_10 is %d\n", ability.full_duplex_10);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "half_duplex_100 is %d\n", ability.half_duplex_100);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "full_duplex_100 is %d\n", ability.full_duplex_100);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "flow_control is %d\n", ability.flow_control);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "asym_flow_control is %d\n", ability.asym_flow_control);
	}
	/* Get PHY link status of UTP PORT  */

	port_udelay(2000000); //wait 2s
	JL_FOR_EACH_PHY_PORT(port) {
		speed = 0;
		duplex = 0;
		linkst = 0;
		ret = jl_port_phy_link_status_get(port, &linkst, &duplex, &speed);
		if (ret)
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_phy_link_status_get ret err %d\n", ret);

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port%d linkst is %d,duplex is %d,speed is %d\n", port, linkst, duplex, speed);
	}

	jl_port_mac_ability_t cur_ability = {0};

	JL_FOR_EACH_PHY_PORT(port) {
		memset(&cur_ability, 0x00, sizeof(jl_port_mac_ability_t));
		ret = jl_port_mac_status_get(port, &cur_ability);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port%d force_mode is %d\n", port, cur_ability.force_mode);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port%d speed is %d\n", port, cur_ability.speed);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port%d duplex is %d\n", port, cur_ability.duplex);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port%d link is %d\n", port, cur_ability.link);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port%d force_fc_en is %d\n", port, cur_ability.force_fc_en);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port%d tx_pause is %d\n", port, cur_ability.tx_pause);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port%d rx_pause is %d\n", port, cur_ability.rx_pause);
	}
	return 0;
}
