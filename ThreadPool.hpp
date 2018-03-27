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
        ThreadPool(int);
        ~ThreadPool() = default;
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

        template <typename retType, typename Func, typename... Args>
        struct Helper
        {
            Helper(std::promise<retType>& promise,
                   Func&& func,
                   Args&&... args) : _prom(promise),
                                     _f(std::forward<Func>(func)),
                                     _args(std::make_tuple(std::forward<Args>(args)...)) {}
            void operator()()
            {
                std::cout << "In helper" << std::endl;
                _prom.set_value(std::apply(_f, _args));
            }

        private:
            std::promise<retType>& _prom;
            std::decay_t<Func> _f;
            std::tuple<std::decay_t<Args>...> _args;
        };

        template <typename Func, typename... Args>
        struct Helper<void, Func, Args...>
        {
            Helper(std::promise<void>& promise,
                   Func&& func,
                   Args&&... args) : _f(std::forward<Func>(func)),
                                     _args(std::make_tuple(std::forward<Args>(args)...)) {}
            void operator()()
            {
                std::cout << "In helper" << std::endl;
                std::apply(_f, _args);
            }

        private:
            std::decay_t<Func> _f;
            std::tuple<std::decay_t<Args>...> _args;
        };

        template <typename Task, typename... Args>
        auto runAsyncTask(Task&& toCall, Args&&... args)
        {
            std::promise<typename std::result_of<Task(Args...)>::type > promise;
            auto ret = promise.get_future();
            auto task = [prom=std::move(promise),
                    toCall = std::forward<Task>(toCall),
                    &args...]() mutable /* ça pue la merde, a modifier (args by ref)*/
            {
               Helper(prom, std::forward<Task>(toCall), std::forward<Args>(args)...);
            };


            std::unique_lock lk(_mut);
            //_tasks.push(task);
            lk.unlock();
            _cond.notify_one();

            return ret;
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
