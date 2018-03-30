//
// Created by benito on 3/25/18.
//

#ifndef THREAD_POOL_THREAD_HPP
#define THREAD_POOL_THREAD_HPP

#include <thread>
#include <list>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>
#include <future>
#include <iostream>

namespace Af
{
    class Thread
    {
    public:
        class ITask
        {
        public:
            virtual void operator()() = 0;
        };

        template <typename RetType, typename Func, typename... Args>
        class Task : public ITask
        {
        public:
            Task(std::promise<RetType>&& promise,
                 Func&& func,
                 Args&&... args) : _prom(std::move(promise)),
                                   _f(std::forward<Func>(func)),
                                   _args(std::make_tuple(std::forward<Args>(args)...)) {}

            Task(Task&&) = default;
            Task(const Task&) = delete;

            void operator()() override
            {
                std::cout << "In helper" << std::endl;
                try
                {
                    _prom.set_value(std::apply(_f, _args));
                }
                catch (...)
                {
                    _prom.set_exception(std::current_exception());
                }
            }

        private:
            std::promise<RetType> _prom;
            std::decay_t<Func> _f;
            std::tuple<std::decay_t<Args>...> _args;
        };

        template <typename Func, typename... Args>
        class Task<void, Func, Args...> : public ITask
        {
        public:
            Task(std::promise<void>&& promise,
                 Func&& func,
                 Args&&... args) : _prom(std::move(promise)),
                                   _f(std::forward<Func>(func)),
                                   _args(std::make_tuple(std::forward<Args>(args)...)) {}

            Task(Task&&) = default;
            Task(const Task&) = delete;

            void operator()() override
            {
                try
                {
                    std::cout << "In helper" << std::endl;
                    std::apply(_f, _args);
                }
                catch (...)
                {
                    _prom.set_exception(std::current_exception());
                }
            }

        private:
            std::promise<void> _prom;
            std::decay_t<Func> _f;
            std::tuple<std::decay_t<Args>...> _args;
        };

        enum class DestroyBehavior {DETACH, JOIN};

        Thread(DestroyBehavior, std::shared_ptr<std::mutex>&, std::shared_ptr<std::condition_variable>&, std::queue<std::unique_ptr<ITask>>&);
        ~Thread();
        Thread(Thread&&);
        Thread(const Thread&) = delete;

        std::thread& get() noexcept ;

        bool isWorking() const noexcept;

        void jobFinished() noexcept ;

    private:
        DestroyBehavior                         _behavior;
        std::atomic<bool>                       _jobFinished;
        std::weak_ptr<std::condition_variable>  _cond;
        std::weak_ptr<std::mutex>               _mut;
        std::atomic<bool>                       _isWorking;
        std::thread                             _thread;
        std::queue<std::unique_ptr<ITask>>&     _tasks;
    };
}


#endif //THREAD_POOL_THREAD_HPP
