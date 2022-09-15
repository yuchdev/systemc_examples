#ifndef PAYLOAD_MANAGER_H
#define PAYLOAD_MANAGER_H

#include <tlm>

class PayloadManager : public tlm::tlm_mm_interface
{
public:
	static tlm::tlm_generic_payload& Acquire();
	void free(tlm::tlm_generic_payload* p) override;
	static void Cleanup();

private:
	tlm::tlm_generic_payload& AcquireImpl();
	static PayloadManager* m_Instance;
	static PayloadManager* GetInstance();

};

#endif
