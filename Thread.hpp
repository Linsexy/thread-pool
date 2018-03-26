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

namespace Af
{
    class Thread
    {
    public:
        Thread(std::mutex&, std::condition_variable&, std::queue<std::function<void()>>);
        ~Thread();
        Thread(Thread&&);
        Thread(const Thread&) = delete;

        bool isReady() const noexcept;

        /*
        template <typename Task>
        void giveNewTask(Task&& task)
        {
            _tasks.push(std::forward<Task>(task));
            _isReady.store(false);
        }
         */

    private:
        std::atomic<bool>        _jobFinished;
        std::condition_variable& _cond;
        std::mutex&              _mut;
        std::atomic<bool>        _isReady;
        std::thread              _thread;
        std::queue<std::function<void()>>& _tasks;
    };
}


#endif //THREAD_POOL_THREAD_HPP
