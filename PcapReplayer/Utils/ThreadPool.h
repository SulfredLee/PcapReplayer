#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include <thread>
#include <queue>
#include <mutex>
#include <functional>
#include <vector>
#include <memory>
#include <condition_variable>
#include <atomic>

template <class T>
class ThreadPool
{
 public:
    ThreadPool();
    ~ThreadPool();

    bool InitComponent(int numThread, const std::function<void(const T&)>& fn);
    void PushJob(const T& job);
    void StartPool();
    void StopPool();
    void ForceDoAllJobs();

 private:
    bool GetJob(T& job);
    void Main();

 private:
    std::function<void(const T&)> m_fn;
    std::vector<std::unique_ptr<std::thread> > m_threads;
    std::queue<T> m_jobQ;
    std::mutex m_mutex;
    std::condition_variable m_cond;
    std::atomic<bool> m_isRun;
    int m_numThread;
};

template <class T>
ThreadPool<T>::ThreadPool()
{
    m_isRun.store(false);
    m_numThread = 1;
}

template <class T>
ThreadPool<T>::~ThreadPool()
{
    StopPool();
}

template <class T>
bool ThreadPool<T>::InitComponent(int numThread, const std::function<void(const T&)>& fn)
{
    // store callback function
    m_fn = fn;

    // create thread pool
    if (numThread < 1) numThread = 1;
    m_numThread = numThread;

    return true;
}

template <class T>
void ThreadPool<T>::PushJob(const T& job)
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_jobQ.push(job);
    }
    m_cond.notify_one();
}

template <class T>
void ThreadPool<T>::StartPool()
{
    m_isRun.store(true);
    m_threads.clear(); m_threads.reserve(m_numThread);
    for (int i = 0; i < m_numThread; i++)
    {
        m_threads.push_back(std::make_unique<std::thread>(&ThreadPool<T>::Main, this));
        m_threads.back()->detach();
    }
}

template <class T>
void ThreadPool<T>::StopPool()
{
    m_isRun.store(false);
    m_cond.notify_all();
}

template <class T>
void ThreadPool<T>::ForceDoAllJobs()
{
    while (m_jobQ.size())
    {
        m_fn(m_jobQ.front());
        m_jobQ.pop();
    }
}

template <class T>
bool ThreadPool<T>::GetJob(T& job)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_cond.wait(lock, [&] { return m_jobQ.size() != 0 || !m_isRun.load(); });
    if (!m_isRun.load()) return false;

    job = m_jobQ.front();
    m_jobQ.pop();

    return true;
}

template <class T>
void ThreadPool<T>::Main()
{
    while (m_isRun.load())
    {
        T job;
        if (GetJob(job))
            m_fn(job);
    }
}
#endif
