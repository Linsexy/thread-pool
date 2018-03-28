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
                                                            _isWorking(false),
                                                            _tasks(t)
{
    auto func = [this]()
    {
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
                _isWorking.store(true);
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
    _isWorking.store(t._isWorking.load());
}

bool Af::Thread::isWorking() const noexcept { return _isWorking.load();}

void Af::Thread::jobFinished() noexcept { _jobFinished.store(true); }

std::thread& Af::Thread::get() { return _thread; }

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