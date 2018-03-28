//
// Created by benito on 3/25/18.
//

#include "ThreadPool.hpp"

Af::ThreadPool::ThreadPool(int nbOfThreads,
                           Thread::DestroyBehavior behavior) : _behavior(behavior),
                                                               _cond(std::make_shared<std::condition_variable>()),
                                                               _mut(std::make_shared<std::mutex>())
{
    if (nbOfThreads < 1)
    {
        throw Error("Number of threads should be superior to 0");
    }
    _threads.reserve(nbOfThreads);
    for (auto i = 0 ; i != nbOfThreads ; ++i)
    {
        _threads.emplace_back(behavior, _mut, _cond, _tasks);
    }
}

Af::ThreadPool::~ThreadPool()
{
    for (auto& t : _threads)
    {
        t.jobFinished();
    }
    _cond->notify_all();
}