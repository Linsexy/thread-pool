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
        try {

            while (!_jobFinished.load()) {
                //std::cout << _thread.get_id() << "   ->   " << "Starting a new loop" << std::endl;
                if (_mut.expired())
                    return;

                //std::cout << _thread.get_id() << "   ->   " <<"locking !" << std::endl;
                auto mutex = this->_mut.lock();

                std::unique_lock<std::mutex> lk(*mutex);
                if (_tasks.empty() && !_cond.expired() && !_jobFinished.load()) {
                    auto cond = _cond.lock();

                    //std::cout << _thread.get_id() << "   ->   " <<"I'm sleeping" << std::endl;
                    //std::cout << _thread.get_id() << "   ->    " << "is my Job finished ?  -> " << std::boolalpha << _jobFinished.load() << std::endl;
                    cond->wait(lk);
                    //std::cout << _thread.get_id() << "   ->   " <<"Just woke up !" << std::endl;

                    if (_mut.expired())
                        return;
                    // le temps d'acquire le mutex, il est détruit
                    //std::cout << _thread.get_id() << "   ->   " <<"locked with success" << std::endl;
                }
                if (!_jobFinished.load() && !_tasks.empty()) {
                    //std::cout << _thread.get_id() << "   ->   " <<"I have something to do" << std::endl;
                    auto task = std::move(_tasks.front());
                    _tasks.pop();
                    //std::cout << _thread.get_id() << "   ->   " <<"going to unlock !" << std::endl;
                    lk.unlock();
                    _isWorking.store(true);
                    (*task)();
                }
            }
        }
        catch (const std::system_error& e)
        {
            std::cerr << e.what() << std::endl;
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

std::thread& Af::Thread::get() noexcept { return _thread; }

Af::Thread::~Thread()
{
    std::cout << "Thread destroyed" << std::endl;
    if (_thread.joinable())
    {
        std::cout << std::this_thread::get_id() << std::endl;
        if (_behavior == Thread::DestroyBehavior::JOIN)
        {
            std::cout << "joining mister   -> " << _thread.get_id() << std::endl;
            _thread.join();
        }
        else
        {
            _thread.detach();
            std::cout << "detach" << std::endl;
        }
    }
}