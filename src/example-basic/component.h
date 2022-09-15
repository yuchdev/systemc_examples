#ifndef COMPONENT_H
#define COMPONENT_H

#include <systemc>
#include <fstream>

class Component : public sc_core::sc_module
{
public:
	SC_HAS_PROCESS(Component);
	Component(const sc_core::sc_module_name name, const double clockPeriod);
	~Component();
	void Run();
	double GetClockPeriod();
protected:
	virtual bool ExecuteCycle() = 0;
	std::ostream& LOG();
	void SetError();
private:
	double m_ClockPeriod;
	unsigned m_CycleCounter;
	bool m_Error;
};

#endif
