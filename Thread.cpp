//
// Created by benito on 3/25/18.
//

#include "Thread.hpp"

//static functor ?

Af::Thread::Thread(std::mutex& m,
                   std::condition_variable& cond,
                   std::queue<std::unique_ptr<ITask>>& t) : _jobFinished(false),
                                                          _mut(m),
                                                          _cond(cond),
                                                          _isReady(false),
                                                          _tasks(t)
{
    auto func = [this]()
    {
        _isReady.store(true);
        while (!_jobFinished)
        {
            std::unique_lock<std::mutex> lk(this->_mut);
            if (_tasks.empty())
            {
                this->_cond.wait(lk);
                lk.lock();
            }
            if (!_tasks.empty() && !_jobFinished)
            {
                auto task = std::move(_tasks.front());
                _tasks.pop();
                lk.unlock();
                (*task)();
            }
        }
    };
    _thread = std::thread(func);
}

Af::Thread::Thread(Thread && t) : _mut(t._mut),
                                  _cond(t._cond),
                                  _thread(std::move(t._thread)),
                                  _tasks(t._tasks)
{
    _isReady.store(t._isReady.load());
}

bool Af::Thread::isReady() const noexcept { return _isReady.load();}

void Af::Thread::jobFinished() noexcept { _jobFinished.store(true); }

Af::Thread::~Thread()
{
    if (_thread.joinable())
    {
        _jobFinished.store(true);
        _thread.join(); //TODO: detach ?
    }
}