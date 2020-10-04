#ifndef MSGQ_H
#define MSGQ_H

#include <vector>
#include <mutex>
#include <atomic>
#include <memory>
#include <condition_variable>


template<typename T>
class MsgQ
{
 private:
    mutable std::mutex m_MuHead;
    mutable std::mutex m_MuTail;
    std::condition_variable m_ConditionVar;
    std::vector<std::shared_ptr<T> > m_RingQueue;
    std::atomic<int> m_nIN;
    std::atomic<int> m_nOUT;
 public:
    MsgQ(){
        m_RingQueue.resize(128);
        m_nIN = 0;
        m_nOUT = 0;
    }
    ~MsgQ(){
    }

    bool push(std::shared_ptr<T> inData){
        {
            std::lock_guard lock(m_MuHead);
            int nNextIN = ((m_nIN + 1) & (128 - 1)); // mod operation
            if (nNextIN == m_nOUT){ // if the next index of IN is index of OUT
                return false;
            }
            m_RingQueue[m_nIN] = std::move(inData);
            m_nIN = nNextIN; // InterlockedCompareExchange((long*)&m_nIN, nNextIN, m_nIN);
        }
        m_ConditionVar.notify_one();
        return true;
    }

    bool get(std::shared_ptr<T>& outData){
        std::unique_lock lock(m_MuTail);
        while (m_nOUT == m_nIN){ // if no more data in queue
            m_ConditionVar.wait(lock);
            //return false;
        }
        outData = std::move(m_RingQueue[m_nOUT]);
        int nNextOUT = ((m_nOUT + 1) & (128 - 1)); // mod operation
        m_nOUT = nNextOUT; // InterlockedCompareExchange((long*)&m_nOUT, nNextOUT, m_nOUT);
        lock.unlock();
        return true;
    }
};

#endif
