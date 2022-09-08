#include "Clock.h"

Clock::Clock()
{
	counter = -1;
}

Clock::~Clock()
{

}

void Clock::step()
{
	counter++;
}

void Clock::delay_real()
{
	Sleep(interval);
}

void Clock::evolve(Agent *agent)
{
	agent->setStep(counter);
}