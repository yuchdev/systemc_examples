#include "component.h"
#include "randomizer.h"
#include "global.h"

Component::Component(const sc_core::sc_module_name name, const double clockPeriod)
	: sc_core::sc_module(name),
	  m_ClockPeriod(clockPeriod),
	  m_CycleCounter(0)
{
	SC_THREAD(Run);
}

Component::~Component()
{}

void Component::Run()
{
	LOG() << "running" << std::endl;
	
	while (true)
	{
		m_CycleCounter++;
		LOG() << "new cycle " << m_CycleCounter << std::endl;
		unsigned deltaCycles = Randomizer::Get();
		LOG() << "delay for " << deltaCycles << " delta cycles" << std::endl;
		for (unsigned i = 0; i < deltaCycles; i++)
		{
			wait(sc_core::SC_ZERO_TIME);
		}
	
		if (!ExecuteCycle())
		{
			break;
		}

		LOG() << "cycle done" << std::endl;
		wait(m_ClockPeriod, TIME_UNIT);
	}

	LOG() << "finished " << (m_Error ? "ERROR" : "OK") << " " << m_CycleCounter << std::endl;
	sc_core::sc_stop();
}

double Component::GetClockPeriod()
{
	return m_ClockPeriod;
}

std::ostream& Component::LOG()
{
	return std::cerr << sc_core::sc_time_stamp() << "\t" << this->name() << "\t: ";
}

void Component::SetError()
{
	m_Error = true;
}
