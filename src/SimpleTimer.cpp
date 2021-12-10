
#include "SimpleTimer.h"


SimpleTimer::SimpleTimer()
{
    start = highResClock::now();
}

SimpleTimer::~SimpleTimer()
{

}

double SimpleTimer::stopTimer()
{
    end = highResClock::now();
    std::chrono::duration<double, std::milli> float_ms = end - start;
    return float_ms.count();
}
