/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * Core functions implementation: power status
 */
#include "hge_impl.h"

namespace hge
{

void HGE_Impl::_InitPowerStatus()
{
	m_kernel32 = LoadLibrary("kernel32.dll");

	if (m_kernel32 != NULL)
		m_getsystempowerstatus_func = (GetSystemPowerStatusFunc) GetProcAddress(m_kernel32,
				"GetSystemPowerStatus");

	_UpdatePowerStatus();
}

void HGE_Impl::_UpdatePowerStatus()
{
	SYSTEM_POWER_STATUS ps;

	if (m_getsystempowerstatus_func != NULL && m_getsystempowerstatus_func(&ps))
	{
		if (ps.ACLineStatus == 1)
		{
			m_power_status = HGEPWR_AC;
		}
		else if (ps.BatteryFlag < 128)
		{
			m_power_status = ps.BatteryLifePercent;
		}
		else
		{
			m_power_status = HGEPWR_UNSUPPORTED;
		}
	}
	else
	{
		m_power_status = HGEPWR_UNSUPPORTED;
	}
}

void HGE_Impl::_DonePowerStatus()
{
	if (m_kernel32 != NULL)
		FreeLibrary(m_kernel32);
}

} // namespace hge
