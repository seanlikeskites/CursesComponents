#ifndef TIMER_HPP_INCLUDED
#define TIMER_HPP_INCLUDED

#include <thread>
#include <atomic>

class Timer
{
public:
    Timer()

    ~Timer()

    template <typename Rep>
    void startTimer (const std::chrono::duration <Rep> &callbackPeriod)
    {
    }

    void stopTimer()
    {
    }

private:
    std::thread thread;
    std::atomic_int controlFlag;

    
};

#endif // TIMER_HPP_INCLUDED
