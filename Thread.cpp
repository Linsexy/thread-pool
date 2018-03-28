//
// Created by benito on 3/25/18.
//

#include "Thread.hpp"

//static functor ?

Af::Thread::Thread(DestroyBehavior behavior,
                   std::shared_ptr<std::mutex>& m,
                   std::shared_ptr<std::condition_variable>& cond,
                   std::queue<std::unique_ptr<ITask>>& t) : _behavior(behavior),
                                                            _jobFinished(false),
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
            if (_mut.expired())
                return ;

            auto mutex = this->_mut.lock();

            std::unique_lock<std::mutex> lk(*mutex);
            if (_tasks.empty() && !_cond.expired())
            {
                auto cond = _cond.lock();

                cond->wait(lk);

                if (_mut.expired())
                    return;
                lk.lock();
            }
            if (!_jobFinished && !_tasks.empty())
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
    std::cout << "Thread destroyed" << std::endl;
    if (_thread.joinable())
    {
        std::cout << "Thread joinable" << std::endl;
        std::cout << "_jobFinished = " << std::boolalpha << _jobFinished.load() << std::endl;
        _jobFinished.store(true);
        if (_behavior == Thread::DestroyBehavior::JOIN)
            _thread.join();
        else {
            _thread.detach();
            std::cout << "detach" << std::endl;
        }
    }
}