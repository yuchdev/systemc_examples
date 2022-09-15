#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>

#include <string>
#include <vector>
#include <queue>
#include <random>

#include "component.h"

class Memory;

class Transaction
{
public:
	Transaction(bool write, unsigned address, std::vector<uint8_t> data)
		: m_Write(write),
		  m_Address(address),
		  m_Data(std::move(data))
	{}

	bool GetWrite() { return m_Write; }
	unsigned GetAddress() { return m_Address; }
	std::vector<uint8_t>& GetData() { return m_Data; }

private:
	bool m_Write;
	unsigned m_Address;
	std::vector<uint8_t> m_Data;
};

class Processor : public Component
{
public:
	Processor(const sc_core::sc_module_name name, const double cyclePeriod);
	~Processor();
	void Bind(tlm_utils::simple_target_socket<Memory>& target);
	bool ExecuteCycle() override;
	tlm::tlm_sync_enum Callback(tlm::tlm_generic_payload& p, tlm::tlm_phase& phase, sc_core::sc_time& delay);
private:
	const bool READ = false;
	const bool WRITE = true;

	void GenerateTransactions();
	bool DataPhase(Transaction& trans);
	void AddressPhase(Transaction& trans);

	std::queue<Transaction> m_Transactions;
	tlm_utils::simple_initiator_socket<Processor> m_Socket;
	bool m_DataDone;
	bool m_DataValid;
	std::vector<uint8_t> m_Data;
	bool m_Error;
};

#endif
