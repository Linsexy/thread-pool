//
// Created by benito on 3/25/18.
//

#ifndef THREAD_POOL_THREADPOOL_HPP
#define THREAD_POOL_THREADPOOL_HPP

/* Written for the Arfang Engine
 *
 * https://github.com/Linsexy/arfang-engine
 *
 */
#include <array>
#include <vector>
#include "Thread.hpp"

namespace Af
{
    class ThreadPool
    {
    public:
        ThreadPool(int);
        ~ThreadPool();
        ThreadPool(ThreadPool&&) = default;
        ThreadPool(ThreadPool const&) = delete;



        template <typename Task>
        auto runAsyncTask(Task&& toRun)
        {
            return 1;
        }

    private:
        std::vector<Thread> _threads;
    };
}

#endif //THREAD_POOL_THREADPOOL_HPP
