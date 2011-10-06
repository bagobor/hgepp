/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * hgeParticleManager helper class implementation
 */
#include <hgeparticle.h>

namespace hge
{

hgeParticleManager::hgeParticleManager()
{
	m_systems_count = 0;
	m_tx = m_ty = 0.0f;
}

hgeParticleManager::~hgeParticleManager()
{
	int i;
	for (i = 0; i < m_systems_count; i++)
		delete m_systems[i];
}

void hgeParticleManager::Update(float dt)
{
	int i;
	for (i = 0; i < m_systems_count; i++)
	{
		m_systems[i]->Update(dt);
		if (m_systems[i]->GetAge() == -2.0f && m_systems[i]->GetParticlesAlive() == 0)
		{
			delete m_systems[i];
			m_systems[i] = m_systems[m_systems_count - 1];
			m_systems_count--;
			i--;
		}
	}
}

void hgeParticleManager::Render()
{
	int i;
	for (i = 0; i < m_systems_count; i++)
		m_systems[i]->Render();
}

hgeParticleSystem* hgeParticleManager::SpawnPS(hgeParticleSystemInfo *psi, float x, float y)
{
	if (m_systems_count == MAX_PSYSTEMS)
		return 0;
	m_systems[m_systems_count] = new hgeParticleSystem(psi);
	m_systems[m_systems_count]->FireAt(x, y);
	m_systems[m_systems_count]->Transpose(m_tx, m_ty);
	m_systems_count++;
	return m_systems[m_systems_count - 1];
}

bool hgeParticleManager::IsPSAlive(hgeParticleSystem *ps) const
{
	int i;
	for (i = 0; i < m_systems_count; i++)
		if (m_systems[i] == ps)
			return true;
	return false;
}

void hgeParticleManager::Transpose(float x, float y)
{
	int i;
	for (i = 0; i < m_systems_count; i++)
		m_systems[i]->Transpose(x, y);
	m_tx = x;
	m_ty = y;
}

void hgeParticleManager::KillPS(hgeParticleSystem *ps)
{
	int i;
	for (i = 0; i < m_systems_count; i++)
	{
		if (m_systems[i] == ps)
		{
			delete m_systems[i];
			m_systems[i] = m_systems[m_systems_count - 1];
			m_systems_count--;
			return;
		}
	}
}

void hgeParticleManager::KillAll()
{
	int i;
	for (i = 0; i < m_systems_count; i++)
		delete m_systems[i];
	m_systems_count = 0;
}

} // namespace hge
