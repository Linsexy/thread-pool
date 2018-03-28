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
#include <functional>
#include <list>
#include <future>
#include <queue>
#include <iostream>
#include "Thread.hpp"

namespace Af
{
    class ThreadPool
    {
    public:

        ThreadPool(int, Thread::DestroyBehavior behavior=Thread::DestroyBehavior::DETACH);
        ~ThreadPool();
        ThreadPool(ThreadPool&&) = default;
        ThreadPool(ThreadPool const&) = delete;

        template <typename Func, typename... Args>
        auto runAsyncTask(Func&& toCall, Args&&... args)
        {
            using RetType = typename std::result_of<Func(Args...)>::type;

            std::cout << "Hi there" << std::endl;
            std::promise<RetType> promise;
            auto ret = promise.get_future();

            auto task = std::make_unique<Thread::Task<RetType, Func, Args...>>(std::move(promise),
                                                                               std::forward<Func>(toCall),
                                                                               std::forward<Args>(args)...);

            std::cout << "threadpool acquiring mutex" << std::endl;
            std::unique_lock lk(*_mut);
            _tasks.emplace(std::move(task));
            std::cout << "unlocking it" << std::endl;
            lk.unlock();
            std::cout << "notify a thread" << std::endl;
            _cond->notify_one();
            std::cout << "returning" << std::endl;

            return ret;
        }

        class Error : public std::runtime_error
        {
        public:
            Error(const std::string& err) : std::runtime_error(err) {}
        };

    private:
        Thread::DestroyBehavior  _behavior;
        std::shared_ptr<std::condition_variable>  _cond;
        std::shared_ptr<std::mutex>               _mut;
        std::queue<std::unique_ptr<Thread::ITask>> _tasks;
        std::vector<Thread>      _threads;
    };
}

#endif //THREAD_POOL_THREADPOOL_HPP
