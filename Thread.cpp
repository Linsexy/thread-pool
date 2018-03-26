//
// Created by benito on 3/25/18.
//

#include "Thread.hpp"

//static functor ?

Af::Thread::Thread(std::mutex& m,
                   std::condition_variable& cond) : _mut(m),
                                                    _cond(cond),
                                                    _isReady(false)
{
    auto func = [this]()
    {
        std::unique_lock<std::mutex> lk(this->_mut);
        this->_cond.wait(lk);
    };
    _thread = std::thread(func);
}

Af::Thread::Thread(Thread && t) : _mut(t._mut),
                                  _cond(t._cond),
                                  _thread(std::move(t._thread))
{
    _isReady.store(t._isReady.load());
}

Af::Thread::~Thread() {}