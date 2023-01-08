//Name: Grant Clark
//File: Timer.h

#ifndef TIMER_H
#define TIMER_H

#include <sys/time.h>
#include <sys/resource.h>

class Timer {
public:
    Timer() :
        starttime_(0),
        endtime_(0),
        total_time(0)
    {}

    ~Timer() {}

    void start()
    {
        getrusage(RUSAGE_SELF, &ru_start);

        starttime_ = ru_start.ru_utime.tv_sec
                     + ru_start.ru_utime.tv_usec * 1e-6;
        
        return;
    }

    void stop()
    {
        getrusage(RUSAGE_SELF, &ru_end);

        endtime_ = ru_end.ru_utime.tv_sec
                    + ru_end.ru_utime.tv_usec * 1e-6;

        total_time = endtime_ - starttime_;

        return;
    }

    double gettime()
    {
        rusage ru;
        double ret;
        getrusage(RUSAGE_SELF, &ru);

        ret = ru.ru_utime.tv_sec + ru.ru_utime.tv_usec * 1e-6;

        return ret;
    }

    double starttime() const { return starttime_; }
    double endtime() const { return endtime_; }
    double read() const { return total_time; }
    
private:
    rusage ru_start;
    rusage ru_end;
    
    double starttime_;
    double endtime_;
    double total_time;
};

#endif
