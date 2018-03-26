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

        /*class Task
        {
        public:
            template <typename T>
            Task()
            {

            }
            operator()
        };
         */


        template <typename Task, typename... Args>
        auto runAsyncTask(Task&& toCall, Args... args)
        {
            std::promise<std::result_of<Task(Args...)>> promise;
            auto task = [=]()
            {
                promise.set_value(toCall(std::forward<Args>(args)...));
            };


            std::unique_lock lk(_mut);
            _tasks.push(task);
            lk.unlock();
            _cond.notify_one();

            return promise.get_future();
        }

        class Error : public std::runtime_error
        {
        public:
            Error(const std::string& err) : std::runtime_error(err) {}
        };

    private:
        std::condition_variable  _cond;
        std::mutex               _mut;
        std::vector<Thread>      _threads;
        std::queue<std::function<void()>> _tasks;
    };
}

#endif //THREAD_POOL_THREADPOOL_HPP
