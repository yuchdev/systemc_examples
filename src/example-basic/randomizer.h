#ifndef RANDOMIZER_H
#define RANDOMIZER_H

#include <random>

class Randomizer
{
public:
	static unsigned Get();
	static void Cleanup();

private:
	Randomizer();
	static Randomizer* m_Instance;
	static Randomizer* GetInstance();

	unsigned Generate();

	std::random_device m_Rd;
	std::mt19937 m_Mt;
	std::uniform_int_distribution<unsigned> m_Dist;
};

#endif
