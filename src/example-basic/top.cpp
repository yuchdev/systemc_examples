#include <systemc>
#include <tlm>

#include "processor.h"
#include "memory.h"
#include "randomizer.h"
#include "payload_manager.h"
#include "global.h"

int sc_main(int argc, char** argv)
{
	Processor procesor("ASIP", ASIP_CLK_PERIOD);
	Memory memory("MEM", MEM_CLK_PERIOD, MEM_LATENCY, MEM_SIZE);

	procesor.Bind(memory.GetSocket());

	sc_core::sc_start();

	Randomizer::Cleanup();
	PayloadManager::Cleanup();

	return 0;
}
