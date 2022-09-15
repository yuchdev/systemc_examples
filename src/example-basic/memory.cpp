#include <systemc>
#include <tlm>
#include <cstdlib>

#include <string>
#include <vector>

#include "memory.h"
#include "payload_manager.h"
#include "global.h"

Memory::Memory(const sc_core::sc_module_name name, const double clockPeriod, const unsigned latency, const unsigned size)
	: Component(name, clockPeriod),
	  m_Latency(latency),
	  m_Queue("queue")
{
	InitializeMemory(size);
	m_Socket.register_nb_transport_fw(this, Callback);
}

Memory::~Memory()
{}

tlm_utils::simple_target_socket<Memory>& Memory::GetSocket()
{
	return m_Socket;
}

bool Memory::ExecuteCycle()
{
	tlm::tlm_generic_payload* trans = m_Queue.get_next_transaction();
	const bool valid = trans != NULL;
	tlm::tlm_generic_payload& p = (valid ? (*trans) : PayloadManager::Acquire());

	sc_core::sc_time delay = sc_core::SC_ZERO_TIME;
	tlm::tlm_phase phase = tlm::BEGIN_RESP;
	p.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

	if (valid)
	{
		LOG() << "sending valid data" << std::endl;
		p.set_response_status(tlm::TLM_OK_RESPONSE);
		if (p.is_write())
		{
			p.set_data_ptr(new unsigned char [4]);
		}
		else
		{
			LOG() << "setting payload data pointer" << std::endl;
			p.set_data_ptr(m_Data.data() + p.get_address());
		}
	}
	else
	{
		LOG() << "sending latency cycle" << std::endl;
	}

	LOG() << "initializing data phase transport" << std::endl;
	tlm::tlm_sync_enum response = m_Socket->nb_transport_bw(p, phase, delay);
	if (response != tlm::TLM_COMPLETED)
	{
		LOG() << "unexpected response of nb_transport_bw" << std::endl;
		throw 0;
	}

	if (p.is_write())
	{
		if (valid)
		{
			LOG() << "writing data from write operation" << std::endl;
			for (unsigned i = 0; i < 4; i++)
			{
				m_Data.at(p.get_address() + i) = p.get_data_ptr()[i];
			}
		}
		delete[] p.get_data_ptr();
	}

	p.release();

	return true;
}

tlm::tlm_sync_enum Memory::Callback(tlm::tlm_generic_payload& p, tlm::tlm_phase& phase, sc_core::sc_time& delay)
{
	LOG() << "new transaction detected " << (p.is_write() ? "WRITE" : "READ") << " 0x" << std::hex << p.get_address() << std::dec << std::endl;

	if (phase != tlm::BEGIN_REQ)
	{
		LOG() << "Transaction isn't of type BEGIN_REQ" << std::endl;
		throw 0;
	}

	p.acquire();
	sc_core::sc_time timeout = sc_core::sc_time(Component::GetClockPeriod() * m_Latency, TIME_UNIT);
	LOG() << "transaction data phase scheduled at " << timeout << std::endl;
	m_Queue.notify(p, timeout);

	return tlm::TLM_COMPLETED;
}

void Memory::InitializeMemory(const unsigned size)
{
	for (unsigned i = 0; i < size; i++)
	{
		m_Data.push_back(i);
	}
}
