#include "payload_manager.h"

PayloadManager* PayloadManager::m_Instance = NULL;

tlm::tlm_generic_payload& PayloadManager::Acquire()
{
	return GetInstance()->AcquireImpl();
}

void PayloadManager::free(tlm::tlm_generic_payload* p)
{
	delete p;
}

tlm::tlm_generic_payload& PayloadManager::AcquireImpl()
{
	tlm::tlm_generic_payload* result = new tlm::tlm_generic_payload();
	result->set_mm(this);
	result->acquire();
	return *result;
}

PayloadManager* PayloadManager::GetInstance()
{
	if (m_Instance == NULL)
	{
		m_Instance = new PayloadManager();
	}
	return m_Instance;
}

void PayloadManager::Cleanup()
{
	if (m_Instance != NULL)
	{
		delete m_Instance;
	}
}
