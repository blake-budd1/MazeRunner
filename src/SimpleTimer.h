#include <cstdlib>
#include <chrono>
using highResClock = std::chrono::high_resolution_clock;
using timePoint = highResClock::time_point;


//timer class
class SimpleTimer {
    timePoint start;
    timePoint end;

public:
    SimpleTimer();
    ~SimpleTimer();
    double stopTimer(); //stops and returns the elapsed time
};