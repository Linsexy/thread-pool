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
namespace Af
{
    template <int nbOfThreads>
    class ThreadPool
    {
    public:
        ThreadPool() = default;
        ~ThreadPool();
        ThreadPool(ThreadPool&&) = default;
        ThreadPool(ThreadPool const&) = delete;



        template <typename Task>
        auto runAsyncTask(Task&& toRun)
        {

        }
    };
}

#endif //THREAD_POOL_THREADPOOL_HPP
