#include <systemc>
#include <tlm>

#include <string>
#include <vector>

#include "processor.h"
#include "payload_manager.h"

Processor::Processor(const sc_core::sc_module_name name, const double clockPeriod)
	: Component(name, clockPeriod),
	  m_DataDone(false),
	  m_DataValid(false),
	  m_Error(false)
{
	GenerateTransactions();
	m_Socket.register_nb_transport_bw(this, Callback);
}

Processor::~Processor()
{}

void Processor::Bind(tlm_utils::simple_target_socket<Memory>& target)
{
	m_Socket.bind(target);
}

bool Processor::ExecuteCycle()
{
	if (DataPhase(m_Transactions.front()))
	{
		LOG() << "next transaction" << std::endl;
		m_Transactions.pop();
		if (m_Transactions.empty())
		{
			return false;
		}
	}
	AddressPhase(m_Transactions.front());
	return true;
}

bool Processor::DataPhase(Transaction& trans)
{
	while (!m_DataDone)
	{
		LOG() << "waiting for data phase" << std::endl;
		wait(sc_core::SC_ZERO_TIME);
	}
	m_DataDone = false;

	if (m_DataValid)
	{
		if (trans.GetWrite())
		{
			LOG() << "write operation completed" << std::endl;
		}
		else
		{
			for (unsigned i = 0; i < 4; i++)
			{
				if (trans.GetData().at(i) != m_Data.at(i))
				{
					LOG() << "read data missmatch 0x" << std::hex << (int)m_Data.at(i) << " - 0x" << (int)trans.GetData().at(i) << std::dec << std::endl;
					SetError();
				}
			}
			LOG() << "data read correctly" << std::endl;
		}
		return true;
	}
	else
	{
		LOG() << "data not ready yet" << std::endl;
	}
	return false;
}

void Processor::AddressPhase(Transaction& trans)
{
	tlm::tlm_generic_payload& p = PayloadManager::Acquire();
	sc_core::sc_time delay = sc_core::SC_ZERO_TIME;
	tlm::tlm_phase phase = tlm::BEGIN_REQ;

	p.set_command(trans.GetWrite() ? tlm::TLM_WRITE_COMMAND : tlm::TLM_READ_COMMAND);
	p.set_address(trans.GetAddress());

	LOG() << "sending address pahse " << (trans.GetWrite() ? "WRITE" : "READ") << " 0x" << std::hex << trans.GetAddress() << std::dec << std::endl;

	tlm::tlm_sync_enum response = m_Socket->nb_transport_fw(p, phase, delay);
	p.release();

	if (response != tlm::TLM_COMPLETED)
	{
		LOG() << "unexpected response of nb_transport_fw" << std::endl;
		SetError();
	}
	LOG() << "address phase send" << std::endl;
}

tlm::tlm_sync_enum Processor::Callback(tlm::tlm_generic_payload& p, tlm::tlm_phase& phase, sc_core::sc_time& delay)
{
	LOG() << "processing transport callback" << std::endl;
	m_DataDone = true;
	if (p.get_response_status() == tlm::TLM_OK_RESPONSE)
	{
		LOG() << "data is ready" << std::endl;
		m_DataValid = true;
		if (m_Transactions.front().GetWrite())
		{
			for (unsigned i = 0; i < 4; i++)
			{
				p.get_data_ptr()[i] = m_Transactions.front().GetData().at(i);
			}		
		}
		else
		{
			m_Data.clear();
			for (unsigned i = 0; i < 4; i++)
			{
				m_Data.push_back(p.get_data_ptr()[i]);
			}
		}
	}
	else if (p.get_response_status() == tlm::TLM_INCOMPLETE_RESPONSE)
	{
		LOG() << "data not ready" << std::endl;
		m_DataValid = false;
	}
	else
	{
		LOG() << "error in data phase" << std::endl;
		SetError();
	}

	LOG() << "transport callback done" << std::endl;
	return tlm::TLM_COMPLETED;
}

void Processor::GenerateTransactions()
{
	m_Transactions.push(Transaction(READ,  0x00, {0x00, 0x01, 0x02, 0x03}));
	m_Transactions.push(Transaction(READ,  0x08, {0x08, 0x09, 0x0a, 0x0b}));
	m_Transactions.push(Transaction(WRITE, 0x04, {0xf0, 0xf1, 0xf2, 0xf3}));
	m_Transactions.push(Transaction(READ,  0x04, {0xf0, 0xf1, 0xf2, 0xf3}));
}
