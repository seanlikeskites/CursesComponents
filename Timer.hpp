#ifndef TIMER_HPP_INCLUDED
#define TIMER_HPP_INCLUDED

#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

/** A timer class which calls a given function periodically.
 *
 *  Classes which inherit this class will have their implementation timerCallback called
 *  periodically.
 */
class Timer
{
public:
    /** Constructor */
    Timer();
    /** Destructor */
    ~Timer();

    /** Start the timer.
     *
     *  Starts the timer such that timerCallback will be called periodically using the
     *  provided period.
     *
     *  @param newCallbackPeriod the new callback period
     *
     *  If the timer is currently stopped this involves the starting of a new thread.
     */
    void startTimer (const std::chrono::milliseconds &newCallbackPeriod);
    /** Pause the timer.
     *
     *  Causes the timer thread to busy sleep. The timerCallback will no longer get called
     *  by the thread will continue to run in the background.
     */
    void pauseTimer();
    /** Stop the timer.
     *
     *  Causes the timer thread to exit.
     */
    void stopTimer();

    /** The timer callback.
     *
     *  Override this function to provide the things you want to happen periodically.
     */
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
