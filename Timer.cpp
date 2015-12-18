#include "Timer.hpp"

Timer::Timer()
    : timerThread ([this] () {run();}),
      callbackPeriod (0),
      controlFlag (ControlState::Stopped)
{
}

Timer::~Timer()
{
    std::unique_lock <std::mutex> lock (controlMutex);
    controlFlag = ControlState::Exit;
    lock.unlock();

    timerThread.join();
}

void Timer::startTimer (const std::chrono::milliseconds &newCallbackPeriod)
{
    std::unique_lock <std::mutex> lock (controlMutex);
    callbackPeriod = newCallbackPeriod;
    controlFlag = ControlState::Running;
}

void Timer::stopTimer()
{
    std::unique_lock <std::mutex> lock (controlMutex);
    controlFlag = ControlState::Stopped;
}

void Timer::run()
{
    while (controlFlag != ControlState::Exit)
    {
        std::unique_lock <std::mutex> lock (controlMutex);

        if (controlFlag == ControlState::Running)
        {
            timerCallback();
            controlCondition.wait_for (lock, callbackPeriod);
        }
        else if (controlFlag == ControlState::Stopped)
        {
            std::this_thread::yield();
        }
    }
}
