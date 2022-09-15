#include "randomizer.h"
#include "global.h"

Randomizer* Randomizer::m_Instance = NULL;

unsigned Randomizer::Get()
{
	return GetInstance()->Generate();
}

void Randomizer::Cleanup()
{
	if (m_Instance != NULL)
	{
		delete m_Instance;
	}
}

unsigned Randomizer::Generate()
{
	return m_Dist(m_Mt);
}

Randomizer::Randomizer()
	: m_Rd(),
	  m_Mt(m_Rd()),
	  m_Dist(0, MAX_DELTA_CYCLES)
{
	m_Mt.seed(SEED);
}

Randomizer* Randomizer::GetInstance()
{
	if (m_Instance == NULL)
	{
		m_Instance = new Randomizer();
	}
	return m_Instance;
}
