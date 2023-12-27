#include "jl_switch.h"

#include "jl51xx/jl51xx_drv_cpu.h"
#include "cpu.h"


static jl_ret_t _jl_cpu_allow_bpdu_enable_set(jl_enable_t enable)
{
	return jl51xx_cpu_allow_bpdu_enable_set(enable);
}

static jl_ret_t _jl_cpu_allow_bpdu_enable_get(jl_enable_t *penable)
{
	return jl51xx_cpu_allow_bpdu_enable_get(penable);
}

static jl_ret_t _jl_cpu_allow_rst_bpdu_enable_set(jl_enable_t enable)
{
	return jl51xx_cpu_allow_rst_bpdu_enable_set(enable);
}

static jl_ret_t _jl_cpu_allow_rst_bpdu_enable_get(jl_enable_t *penable)
{
	return jl51xx_cpu_allow_rst_bpdu_enable_get(penable);
}

static jl_ret_t _jl_to_cpu_tag_enable_get(jl_enable_t *penable)
{
	return jl51xx_to_cpu_tag_enable_get(penable);
}

static jl_ret_t _jl_to_cpu_tag_enable_set(jl_enable_t enable)
{
	return jl51xx_to_cpu_tag_enable_set(enable);
}

static jl_ret_t _jl_cpu_allow_lldp_enable_set(jl_enable_t enable)
{
	return jl51xx_cpu_allow_lldp_enable_set(enable);
}

static jl_ret_t _jl_cpu_allow_lldp_enable_get(jl_enable_t *penable)
{
	return jl51xx_cpu_allow_lldp_enable_get(penable);
}

jl_api_ret_t jl_cpu_allow_bpdu_enable_set(jl_enable_t enable)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	if (enable >= JL_ENABLE_END)
		return JL_ERR_PARAM;

	JL_API_MUTEX_LOCK();
	ret = _jl_cpu_allow_bpdu_enable_set(enable);
	JL_API_MUTEX_UNLOCK();

	return ret;
}

jl_api_ret_t jl_cpu_allow_bpdu_enable_get(jl_enable_t *penable)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	JL_CHECK_POINTER(penable);

	JL_API_MUTEX_LOCK();
	ret = _jl_cpu_allow_bpdu_enable_get(penable);
	JL_API_MUTEX_UNLOCK();

	return ret;
}

jl_api_ret_t jl_cpu_allow_rst_bpdu_enable_set(jl_enable_t enable)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	if (enable >= JL_ENABLE_END)
		return JL_ERR_PARAM;

	JL_API_MUTEX_LOCK();
	ret = _jl_cpu_allow_rst_bpdu_enable_set(enable);
	JL_API_MUTEX_UNLOCK();

	return ret;
}

jl_api_ret_t jl_cpu_allow_rst_bpdu_enable_get(jl_enable_t *penable)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	JL_CHECK_POINTER(penable);

	JL_API_MUTEX_LOCK();
	ret = _jl_cpu_allow_rst_bpdu_enable_get(penable);
	JL_API_MUTEX_UNLOCK();

	return ret;
}

jl_api_ret_t jl_cpu_tag_enable_set(jl_enable_t enable)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	if (enable >= JL_ENABLE_END)
		return JL_ERR_PARAM;

	JL_API_MUTEX_LOCK();
	ret = _jl_to_cpu_tag_enable_set(enable);
	JL_API_MUTEX_UNLOCK();

	return ret;
}

jl_api_ret_t jl_cpu_tag_enable_get(jl_enable_t *penable)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	JL_CHECK_POINTER(penable);

	JL_API_MUTEX_LOCK();
	ret = _jl_to_cpu_tag_enable_get(penable);
	JL_API_MUTEX_UNLOCK();

	return ret;
}

jl_api_ret_t jl_cpu_allow_lldp_enable_set(jl_enable_t enable)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	if (enable >= JL_ENABLE_END)
		return JL_ERR_PARAM;

	JL_API_MUTEX_LOCK();
	ret = _jl_cpu_allow_lldp_enable_set(enable);
	JL_API_MUTEX_UNLOCK();

	return ret;
}

jl_api_ret_t jl_cpu_allow_lldp_enable_get(jl_enable_t *penable)
{
	jl_api_ret_t ret;

	/* Check System state */
	JL_CHECK_STAT();

	JL_CHECK_POINTER(penable);

	JL_API_MUTEX_LOCK();
	ret = _jl_cpu_allow_lldp_enable_get(penable);
	JL_API_MUTEX_UNLOCK();

	return ret;
}

