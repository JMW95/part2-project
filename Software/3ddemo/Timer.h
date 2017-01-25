#pragma once

#include <time.h>

class Timer{
private:
    timespec stime;
    timespec etime;
    long timediff;
public:
    Timer();
    void start();
    void stop();
    float get_seconds();
    long get_milliseconds();
};
