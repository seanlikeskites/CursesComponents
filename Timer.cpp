#include "Timer.hpp"

Timer::Timer()
    : callbackPeriod (0),
      controlFlag (ControlState::Stopped)
{
}

Timer::~Timer()
{
    stopTimer();
}

void Timer::startTimer (const std::chrono::milliseconds &newCallbackPeriod)
{
    std::unique_lock <std::mutex> lock (controlMutex);
    callbackPeriod = newCallbackPeriod;

    ControlState oldState = controlFlag;
    controlFlag = ControlState::Running;

    if (oldState == ControlState::Stopped)
    {
        timerThread = std::thread ([this] () {run();});
    }
}

void Timer::pauseTimer()
{
    std::unique_lock <std::mutex> lock (controlMutex);
    controlFlag = ControlState::Paused;
    controlCondition.notify_one();
}

void Timer::stopTimer()
{
    std::unique_lock <std::mutex> lock (controlMutex);
    controlFlag = ControlState::Stopped;
    controlCondition.notify_one();
    lock.unlock();
    timerThread.join();
}

void Timer::run()
{
    while (true)
    {
        std::unique_lock <std::mutex> lock (controlMutex);

        switch (controlFlag)
        {
            case ControlState::Running:
                timerCallback();
                controlCondition.wait_for (lock, callbackPeriod);
                break;

            case ControlState::Paused:
                std::this_thread::yield();
                break;

            case ControlState::Stopped:
                return;
        }
    }
}
