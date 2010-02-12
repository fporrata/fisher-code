p#ifndef TIMER_H
#define TIMER_H


extern "C" {
#include "sys/time.h"
}

#ifdef MULTITHREADED
#include <OpenThreads/Mutex>
#include <OpenThreads/ScopedLock>
#endif

class Timer {
public:
	Timer() {}
	~Timer() {}
	void start(void);
	void stop(void);
	double secs(void);
	unsigned long int usecs(void);
private:
	struct timeval tv_start;
	struct timeval tv_stop;
#ifdef MULTITHREADED
	OpenThreads::Mutex mutex;
#endif
};

inline void Timer::start(void)
{
	gettimeofday(&tv_start, 0);
}


inline void Timer::stop(void)
{
	gettimeofday(&tv_stop, 0);
}


inline unsigned long int Timer::usecs(void)
{
#ifdef MULTITHREADED
	OpenThreads::ScopedLock<OpenThreads::Mutex> serializer(mutex);
#endif
	stop();
	unsigned long int startTime =
        ( (unsigned long int)(tv_start.tv_sec) * 1000000uL +
          (unsigned long int)(tv_start.tv_usec) );
	unsigned long int stopTime  =
        ( (unsigned long int)(tv_stop.tv_sec) * 1000000uL +
          (unsigned long int)(tv_stop.tv_usec) );
	assert(stopTime >= startTime);
	unsigned long int elapsedTime = stopTime - startTime;
	return elapsedTime;
}


inline double Timer::secs(void)
{
#ifdef MULTITHREADED
	OpenThreads::ScopedLock<OpenThreads::Mutex> serializer(mutex);
#endif
	stop();
	double const startTime =
        ( (double)(tv_start.tv_sec) +
          (double)(tv_start.tv_usec) * 1.0e-6 );
	double const stopTime  =
        ( (double)(tv_stop.tv_sec) +
          (double)(tv_stop.tv_usec) * 1.0e-6 );
	assert(stopTime >= startTime);
	double const elapsedTime = stopTime - startTime;
	return elapsedTime;
}


#endif // TIMER_H

