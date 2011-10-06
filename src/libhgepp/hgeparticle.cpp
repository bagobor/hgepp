/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * hgeParticleSystem helper class implementation
 */


#include <hgeparticle.h>

namespace hge {


HGE	* g_hgeparticlesys_hge=0;


hgeParticleSystem::hgeParticleSystem(const char *filename, hgeSprite *sprite)
{
	void *psi;

	g_hgeparticlesys_hge=hgeCreate(HGE_VERSION);

	psi=g_hgeparticlesys_hge->Resource_Load(filename);
	if(!psi) return;
	memcpy(&info, psi, sizeof(hgeParticleSystemInfo));
	g_hgeparticlesys_hge->Resource_Free(psi);
	info.sprite=sprite;

	m_location.x=m_prev_location.x=0.0f;
	m_location.y=m_prev_location.y=0.0f;
	m_tx=m_ty=0;
	m_scale = 1.0f;

	m_emission_residue=0.0f;
	m_alive_count=0;
	m_age=-2.0;

	m_bounding_box.Clear();
	m_update_bbox=false;
}

hgeParticleSystem::hgeParticleSystem(hgeParticleSystemInfo *psi)
{
	g_hgeparticlesys_hge=hgeCreate(HGE_VERSION);

	memcpy(&info, psi, sizeof(hgeParticleSystemInfo));

	m_location.x=m_prev_location.x=0.0f;
	m_location.y=m_prev_location.y=0.0f;
	m_tx=m_ty=0;
	m_scale = 1.0f;

	m_emission_residue=0.0f;
	m_alive_count=0;
	m_age=-2.0;

	m_bounding_box.Clear();
	m_update_bbox=false;
}

hgeParticleSystem::hgeParticleSystem(const hgeParticleSystem &ps)
{
	memcpy(this, &ps, sizeof(hgeParticleSystem));
	g_hgeparticlesys_hge=hgeCreate(HGE_VERSION);
}

void hgeParticleSystem::Update(float fDeltaTime)
{
	int i;
	float ang;
	hgeParticle *par;
	hgeVector vecAccel, vecAccel2;

	if(m_age >= 0)
	{
		m_age += fDeltaTime;
		if(m_age >= info.fLifetime) m_age = -2.0f;
	}

	// update all alive particles

	if(m_update_bbox) m_bounding_box.Clear();
	par=m_particles;

	for(i=0; i<m_alive_count; i++)
	{
		par->fAge += fDeltaTime;
		if(par->fAge >= par->fTerminalAge)
		{
			m_alive_count--;
			memcpy(par, &m_particles[m_alive_count], sizeof(hgeParticle));
			i--;
			continue;
		}

		vecAccel = par->vecLocation-m_location;
		vecAccel.Normalize();
		vecAccel2 = vecAccel;
		vecAccel *= par->fRadialAccel;

		// vecAccel2.Rotate(M_PI_2);
		// the following is faster
		ang = vecAccel2.x;
		vecAccel2.x = -vecAccel2.y;
		vecAccel2.y = ang;

		vecAccel2 *= par->fTangentialAccel;
		par->vecVelocity += (vecAccel+vecAccel2)*fDeltaTime;
		par->vecVelocity.y += par->fGravity*fDeltaTime;

		par->vecLocation += par->vecVelocity*fDeltaTime;

		par->fSpin += par->fSpinDelta*fDeltaTime;
		par->fSize += par->fSizeDelta*fDeltaTime;
		par->colColor += par->colColorDelta*fDeltaTime;

		if(m_update_bbox) m_bounding_box.Encapsulate(par->vecLocation.x, par->vecLocation.y);

		par++;
	}

	// generate new particles

	if(m_age != -2.0f)
	{
		float fParticlesNeeded = info.nEmission*fDeltaTime + m_emission_residue;
		int nParticlesCreated = (unsigned int)fParticlesNeeded;
		m_emission_residue=fParticlesNeeded-nParticlesCreated;

		par=&m_particles[m_alive_count];

		for(i=0; i<nParticlesCreated; i++)
		{
			if(m_alive_count>=MAX_PARTICLES) break;

			par->fAge = 0.0f;
			par->fTerminalAge = g_hgeparticlesys_hge->Random_Float(info.fParticleLifeMin, info.fParticleLifeMax);

			par->vecLocation = m_prev_location+(m_location-m_prev_location)*g_hgeparticlesys_hge->Random_Float(0.0f, 1.0f);
			par->vecLocation.x += g_hgeparticlesys_hge->Random_Float(-2.0f, 2.0f);
			par->vecLocation.y += g_hgeparticlesys_hge->Random_Float(-2.0f, 2.0f);

			ang=info.fDirection-M_PI_2+g_hgeparticlesys_hge->Random_Float(0,info.fSpread)-info.fSpread/2.0f;
			if(info.bRelative) ang += (m_prev_location-m_location).Angle()+M_PI_2;
			par->vecVelocity.x = cosf(ang);
			par->vecVelocity.y = sinf(ang);
			par->vecVelocity *= g_hgeparticlesys_hge->Random_Float(info.fSpeedMin, info.fSpeedMax);

			par->fGravity = g_hgeparticlesys_hge->Random_Float(info.fGravityMin, info.fGravityMax);
			par->fRadialAccel = g_hgeparticlesys_hge->Random_Float(info.fRadialAccelMin, info.fRadialAccelMax);
			par->fTangentialAccel = g_hgeparticlesys_hge->Random_Float(info.fTangentialAccelMin, info.fTangentialAccelMax);

			par->fSize = g_hgeparticlesys_hge->Random_Float(info.fSizeStart, info.fSizeStart+(info.fSizeEnd-info.fSizeStart)*info.fSizeVar);
			par->fSizeDelta = (info.fSizeEnd-par->fSize) / par->fTerminalAge;

			par->fSpin = g_hgeparticlesys_hge->Random_Float(info.fSpinStart, info.fSpinStart+(info.fSpinEnd-info.fSpinStart)*info.fSpinVar);
			par->fSpinDelta = (info.fSpinEnd-par->fSpin) / par->fTerminalAge;

			par->colColor.r = g_hgeparticlesys_hge->Random_Float(info.colColorStart.r, info.colColorStart.r+(info.colColorEnd.r-info.colColorStart.r)*info.fColorVar);
			par->colColor.g = g_hgeparticlesys_hge->Random_Float(info.colColorStart.g, info.colColorStart.g+(info.colColorEnd.g-info.colColorStart.g)*info.fColorVar);
			par->colColor.b = g_hgeparticlesys_hge->Random_Float(info.colColorStart.b, info.colColorStart.b+(info.colColorEnd.b-info.colColorStart.b)*info.fColorVar);
			par->colColor.a = g_hgeparticlesys_hge->Random_Float(info.colColorStart.a, info.colColorStart.a+(info.colColorEnd.a-info.colColorStart.a)*info.fAlphaVar);

			par->colColorDelta.r = (info.colColorEnd.r-par->colColor.r) / par->fTerminalAge;
			par->colColorDelta.g = (info.colColorEnd.g-par->colColor.g) / par->fTerminalAge;
			par->colColorDelta.b = (info.colColorEnd.b-par->colColor.b) / par->fTerminalAge;
			par->colColorDelta.a = (info.colColorEnd.a-par->colColor.a) / par->fTerminalAge;

			if(m_update_bbox) m_bounding_box.Encapsulate(par->vecLocation.x, par->vecLocation.y);

			m_alive_count++;
			par++;
		}
	}

	m_prev_location=m_location;
}

void hgeParticleSystem::MoveTo(float x, float y, bool bMoveParticles)
{
	int i;
	float dx,dy;
	
	if(bMoveParticles)
	{
		dx=x-m_location.x;
		dy=y-m_location.y;

		for(i=0;i<m_alive_count;i++)
		{
			m_particles[i].vecLocation.x += dx;
			m_particles[i].vecLocation.y += dy;
		}

		m_prev_location.x=m_prev_location.x + dx;
		m_prev_location.y=m_prev_location.y + dy;
	}
	else
	{
		if(m_age==-2.0) { m_prev_location.x=x; m_prev_location.y=y; }
		else { m_prev_location.x=m_location.x;	m_prev_location.y=m_location.y; }
	}

	m_location.x=x;
	m_location.y=y;
}

void hgeParticleSystem::FireAt(float x, float y)
{
	Stop();
	MoveTo(x,y);
	Fire();
}

void hgeParticleSystem::Fire()
{
	if(info.fLifetime==-1.0f) m_age=-1.0f;
	else m_age=0.0f;
}

void hgeParticleSystem::Stop(bool bKillParticles)
{
	m_age=-2.0f;
	if(bKillParticles) 
	{
		m_alive_count=0;
		m_bounding_box.Clear();
	}
}

void hgeParticleSystem::Render()
{
	int i;
	uint32_t col;
	hgeParticle *par=m_particles;

	col=info.sprite->GetColor();

	for(i=0; i<m_alive_count; i++)
	{
		if(info.colColorStart.r < 0)
			info.sprite->SetColor(SETA(info.sprite->GetColor(),par->colColor.a*255));
		else
			info.sprite->SetColor(par->colColor.GetHWColor());
		info.sprite->RenderEx(par->vecLocation.x*m_scale+m_tx, par->vecLocation.y*m_scale+m_ty, par->fSpin*par->fAge, par->fSize*m_scale);
		par++;
	}

	info.sprite->SetColor(col);
}


hgeRect *hgeParticleSystem::GetBoundingBox(hgeRect *rect) const
{
	*rect = m_bounding_box;

	rect->x1 *= m_scale;
	rect->y1 *= m_scale;
	rect->x2 *= m_scale;
	rect->y2 *= m_scale;

	return rect;
}

HGE * hgeParticleSystem::get_hge()
{
	return g_hgeparticlesys_hge;
}

hgeParticleSystem::~hgeParticleSystem()
{
	g_hgeparticlesys_hge->Release();
}

void hgeParticleSystem::Transpose( float x, float y )
{
	m_tx=x; m_ty=y;
}

void hgeParticleSystem::TrackBoundingBox( bool bTrack )
{
	m_update_bbox=bTrack;
}

void hgeParticleSystem::SetScale( float scale )
{
	m_scale = scale;
}

int hgeParticleSystem::GetParticlesAlive() const
{
	return m_alive_count;
}

float hgeParticleSystem::GetAge() const
{
	return m_age;
}

void hgeParticleSystem::GetPosition( float *x, float *y ) const
{
	*x=m_location.x; *y=m_location.y;
}

void hgeParticleSystem::GetTransposition( float *x, float *y ) const
{
	*x=m_tx; *y=m_ty;
}

float hgeParticleSystem::GetScale()
{
	return m_scale;
}

void hgeParticleManager::GetTransposition( float *dx, float *dy ) const
{
	*dx=m_tx; *dy=m_ty;
}

} // namespace hge
