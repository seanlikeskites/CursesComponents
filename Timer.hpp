#ifndef TIMER_HPP_INCLUDED
#define TIMER_HPP_INCLUDED

#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

class Timer
{
public:
    Timer();
    ~Timer();

    void startTimer (const std::chrono::milliseconds &newCallbackPeriod);
    void pauseTimer();
    void stopTimer();

    virtual void timerCallback() = 0;

private:
    std::thread timerThread;
    std::mutex controlMutex;
    std::condition_variable controlCondition;
    std::chrono::milliseconds callbackPeriod;

    enum class ControlState
    {
        Running,
        Paused,
        Stopped
    } controlFlag;

    void run();
};

#endif // TIMER_HPP_INCLUDED
