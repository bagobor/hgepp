/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * Core functions implementation: power status
 */
#include "hge_impl.h"

namespace hge {

void HGE_Impl::_InitPowerStatus()
{
	hKrnl32 = LoadLibrary("kernel32.dll");

	if(hKrnl32 != NULL)
		lpfnGetSystemPowerStatus = (GetSystemPowerStatusFunc)GetProcAddress(hKrnl32, "GetSystemPowerStatus");

	_UpdatePowerStatus();
}


void HGE_Impl::_UpdatePowerStatus()
{
	SYSTEM_POWER_STATUS ps;

	if(lpfnGetSystemPowerStatus != NULL && lpfnGetSystemPowerStatus(&ps))
	{
		if(ps.ACLineStatus == 1)
		{
			nPowerStatus = HGEPWR_AC;
		}
		else if(ps.BatteryFlag < 128)
		{
			nPowerStatus = ps.BatteryLifePercent;
		}
		else
		{
			nPowerStatus = HGEPWR_UNSUPPORTED;
		}
	}
	else
	{
		nPowerStatus = HGEPWR_UNSUPPORTED;
	}
}


void HGE_Impl::_DonePowerStatus()
{
	if(hKrnl32 != NULL) FreeLibrary(hKrnl32);
}

} // namespace hge
