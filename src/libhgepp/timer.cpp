/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * Core functions implementation: timer
 */
#include <hgepp/hge_impl.h>

namespace hge
{

float HGE_CALL HGE_Impl::Timer_GetTime()
{
	return m_time;
}

float HGE_CALL HGE_Impl::Timer_GetDelta()
{
	return m_time_delta;
}

int HGE_CALL HGE_Impl::Timer_GetFPS()
{
	return m_time_fps;
}

} // namespace hge
