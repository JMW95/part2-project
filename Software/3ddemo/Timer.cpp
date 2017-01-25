#include "Timer.h"

timespec start;
timespec end;
long timediff;

Timer::Timer(){
    timediff = 0;
}

void Timer::start(){
    clock_gettime(CLOCK_MONOTONIC, &stime);
}

void Timer::stop(){
    clock_gettime(CLOCK_MONOTONIC, &etime);
    
    long t1 = (stime.tv_sec * 1000) + (stime.tv_nsec / 1e06);
    long t2 = (etime.tv_sec * 1000) + (etime.tv_nsec / 1e06);
    
    timediff = t2 - t1;
}

float Timer::get_seconds(){
    return timediff / 1000.f;
}

long Timer::get_milliseconds(){
    return timediff;
}
