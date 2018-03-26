//
// Created by benito on 3/25/18.
//

#include "ThreadPool.hpp"

Af::ThreadPool::ThreadPool(int nbOfThreads)
{
    if (nbOfThreads < 1)
    {
        throw Error("Number of threads should be superior to 0");
    }
    _threads.reserve(nbOfThreads);
    for (auto i = 0 ; i != nbOfThreads ; ++i)
    {
        _threads.emplace_back(_mut, _cond, _tasks);
    }
}