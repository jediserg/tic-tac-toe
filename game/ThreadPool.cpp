//
// Created by serg on 10/31/16.
//
#include "ThreadPool.h"
#include <thread>
#include <cassert>

ThreadPool::ThreadPool(size_t N) : _stopped(false)
{
    for(int i = 0; i < N; i++){
        std::thread([this](){
            TaskProc task;
            while(true) {
                {
                    std::unique_lock<std::mutex> lock(_mutex);

                    if(_stopped)
                        break;

                    if(_queue.size() == 0)
                        _queue_notify.wait(lock);

                    task = _queue.front();
                    _queue.pop();
                }
                task();
            }
        }).detach();
    }
}

void ThreadPool::addTask(ThreadPool::TaskProc task) {
    assert(!_stopped);
    std::unique_lock<std::mutex> lock(_mutex);
    _queue.push(task);
    _queue_notify.notify_one();
}

void ThreadPool::stop() {
    std::unique_lock<std::mutex> lock(_mutex);
    _stopped = true;
}

bool ThreadPool::isStopped() const {
    return _stopped;
}

ThreadPool &ThreadPool::getInstance(size_t size) {
    static ThreadPool _pool(size);
    return _pool;
}
