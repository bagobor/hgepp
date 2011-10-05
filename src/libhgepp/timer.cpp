/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * Core functions implementation: timer
 */
#include "hge_impl.h"

namespace hge {

float HGE_CALL HGE_Impl::Timer_GetTime()
{
	return fTime;
}

float HGE_CALL HGE_Impl::Timer_GetDelta()
{
	return fDeltaTime;
}


int HGE_CALL HGE_Impl::Timer_GetFPS()
{
	return nFPS;
}


} // namespace hge
