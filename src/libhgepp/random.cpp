/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * Core functions implementation: random number generation
 */
#include <hgepp/hge_impl.h>
extern "C" {
	#include "sfmt/SFMT.h"
}

namespace hge
{

//unsigned int g_seed = 0;

void HGE_CALL HGE_Impl::Random_Seed(int seed)
{
	init_gen_rand( (uint32_t)seed );
// 	if (!seed)
// 		g_seed = timeGetTime();
// 	else
// 		g_seed = seed;
}

int HGE_CALL HGE_Impl::Random_Int(int min, int max)
{
// 	g_seed = 214013 * g_seed + 2531011;
// 	return min + (g_seed ^ g_seed >> 15) % (max - min + 1);
	return min + gen_rand32() % (max - min + 1);
}

float HGE_CALL HGE_Impl::Random_Float(float min, float max)
{
// 	g_seed = 214013 * g_seed + 2531011;
// 	//return min+g_seed*(1.0f/4294967295.0f)*(max-min);
// 	return min + (g_seed >> 16) * (1.0f / 65535.0f) * (max - min);
	return min + (float)genrand_real1() * (max - min);
}

} // namespace hge
