#include "Timer.h"

// STATIC
std::set<Timer*> Timer::timers_;

// STATIC
void Timer::updateAll(units::MS elapsed_time)
{
	for (std::set<Timer*>::iterator iter = timers_.begin(); iter != timers_.end(); iter++)
	{
		(*iter)->update(elapsed_time);
	}
}
