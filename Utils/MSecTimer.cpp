#include "MSecTimer.h"

MSecTimer::~MSecTimer(){
    m_bTimerThreadStop = true;
    m_bTimerThreadExit = true;
    m_TimerThread.join();
}

void MSecTimer::Start(){
    m_bTimerThreadStop = false;
}

void MSecTimer::Stop(){
    m_bTimerThreadStop = true;
}

void MSecTimer::Main(){
    while (!m_bTimerThreadExit){
        boost::this_thread::sleep_for(boost::chrono::milliseconds(m_unDuration));
        if (!m_bTimerThreadStop){
            m_fn();
        }
    }
}
