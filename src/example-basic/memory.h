#ifndef MEMORY_H
#define MEMORY_H

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/peq_with_get.h>

#include <string>
#include <vector>

#include "component.h"

class Memory : public Component
{
public:
	Memory(const sc_core::sc_module_name name, const double clockPeriod, const unsigned latency, const unsigned size);
	~Memory();
	tlm_utils::simple_target_socket<Memory>& GetSocket();
	bool ExecuteCycle() override;
	tlm::tlm_sync_enum Callback(tlm::tlm_generic_payload& p, tlm::tlm_phase& phase, sc_core::sc_time& delay);
private:
	void InitializeMemory(const unsigned size);

	std::vector<uint8_t> m_Data;
	unsigned m_Latency;
	tlm_utils::simple_target_socket<Memory> m_Socket;
	tlm_utils::peq_with_get<tlm::tlm_generic_payload> m_Queue;
};

#endif
