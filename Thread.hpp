//
// Created by benito on 3/25/18.
//

#ifndef THREAD_POOL_THREAD_HPP
#define THREAD_POOL_THREAD_HPP

#include <thread>
#include <atomic>

namespace Af
{
    class Thread
    {
        Thread();
        ~Thread();
        Thread(Thread&&) = default;
        Thread(const Thread&) = delete;

    private:
        std::thread _thread;
        std::atomic<bool> isReady; /* find a better name */
    };
}


#endif //THREAD_POOL_THREAD_HPP
