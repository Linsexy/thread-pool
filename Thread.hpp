//
// Created by benito on 3/25/18.
//

#ifndef THREAD_POOL_THREAD_HPP
#define THREAD_POOL_THREAD_HPP

#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

namespace Af
{
    class Thread
    {
    public:
        Thread(std::mutex&, std::condition_variable&);
        ~Thread();
        Thread(Thread&&);
        Thread(const Thread&) = delete;

    private:
        std::condition_variable& _cond;
        std::mutex&              _mut;
        std::atomic<bool>        _isReady;
        std::thread              _thread;
    };
}


#endif //THREAD_POOL_THREAD_HPP
