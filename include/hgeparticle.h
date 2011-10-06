/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * hgeParticleSystem helper class header
 */

#pragma once

#include <hge.h>
#include <hgesprite.h>
#include <hgevector.h>
#include <hgecolor.h>
#include <hgerect.h>

namespace hge {

#define MAX_PARTICLES	500
#define MAX_PSYSTEMS	100

struct hgeParticle
{
	hgeVector	vecLocation;
	hgeVector	vecVelocity;

	float		fGravity;
	float		fRadialAccel;
	float		fTangentialAccel;

	float		fSpin;
	float		fSpinDelta;

	float		fSize;
	float		fSizeDelta;

	hgeColor	colColor;		// + alpha
	hgeColor	colColorDelta;

	float		fAge;
	float		fTerminalAge;
};

struct hgeParticleSystemInfo
{
	hgeSprite*	sprite;    // texture + blend mode
	int			nEmission; // particles per sec
	float		fLifetime;

	float		fParticleLifeMin;
	float		fParticleLifeMax;

	float		fDirection;
	float		fSpread;
	bool		bRelative;

	float		fSpeedMin;
	float		fSpeedMax;

	float		fGravityMin;
	float		fGravityMax;

	float		fRadialAccelMin;
	float		fRadialAccelMax;

	float		fTangentialAccelMin;
	float		fTangentialAccelMax;

	float		fSizeStart;
	float		fSizeEnd;
	float		fSizeVar;

	float		fSpinStart;
	float		fSpinEnd;
	float		fSpinVar;

	hgeColor	colColorStart; // + alpha
	hgeColor	colColorEnd;
	float		fColorVar;
	float		fAlphaVar;
};

class HGE_EXPORT hgeParticleSystem
{
public:
	hgeParticleSystemInfo info;

	hgeParticleSystem(const char *filename, hgeSprite *sprite);
	hgeParticleSystem(hgeParticleSystemInfo *psi);
	hgeParticleSystem(const hgeParticleSystem &ps);
	~hgeParticleSystem();

	hgeParticleSystem& operator=(const hgeParticleSystem &ps);

	void Render();
	void FireAt(float x, float y);
	void Fire();
	void Stop(bool bKillParticles = false);
	void Update(float fDeltaTime);
	void MoveTo(float x, float y, bool bMoveParticles = false);
	void Transpose(float x, float y);
	void SetScale(float scale);
	void TrackBoundingBox(bool bTrack);

	int GetParticlesAlive() const;
	float GetAge() const;
	void GetPosition(float *x, float *y) const;
	void GetTransposition(float *x, float *y) const;
	float GetScale();
	hgeRect* GetBoundingBox(hgeRect *rect) const;

private:
	hgeParticleSystem();

	//static HGE			*g_hgeparticlesys_hge;
	static HGE * get_hge();

	float m_age;
	float m_emission_residue;

	hgeVector m_prev_location;
	hgeVector m_location;
	float m_tx, m_ty;
	float m_scale;

	int m_alive_count;
	hgeRect m_bounding_box;
	bool m_update_bbox;

	// TODO: replace with pool or something resizable and fast
	hgeParticle m_particles[MAX_PARTICLES];
};


class HGE_EXPORT hgeParticleManager
{
public:
	hgeParticleManager();
	~hgeParticleManager();

	void Update(float dt);
	void Render();

	hgeParticleSystem* SpawnPS(hgeParticleSystemInfo *psi, float x, float y);
	bool IsPSAlive(hgeParticleSystem *ps) const;
	void Transpose(float x, float y);
	void GetTransposition(float *dx, float *dy) const;
	void KillPS(hgeParticleSystem *ps);
	void KillAll();

private:
	hgeParticleManager(const hgeParticleManager &);
	hgeParticleManager& operator=(const hgeParticleManager &);

	int m_systems_count;
	float m_tx;
	float m_ty;

	// TODO: replace with STL vector or list
	hgeParticleSystem* m_systems[MAX_PSYSTEMS];
};

} // namespace hge
