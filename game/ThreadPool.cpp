//
// Created by serg on 10/31/16.
//
#include "ThreadPool.h"
#include <thread>
#include <cassert>
#include <iostream>

ThreadPool::ThreadPool(size_t N) : _stopped(false), _active_task_count(0), _run_threads_count(N)
{
    for(int i = 0; i < N; i++){
        std::thread([this](){
            TaskProc task;

            bool first_enter = true;

            while(true) {
                {
                    std::unique_lock<std::mutex> lock(_mutex);

                    //if we here not the first time, than we complete a task, so decrement active tasks count
                    if (first_enter) {
                        first_enter = false;
                    } else {
                        _active_task_count--;
                    }

                    if (_stopped) {
                        _run_threads_count--;
                        break;
                    }

                    if (_queue.empty())
                        _queue_notify.wait(lock, [this]() { return !_queue.empty() || _stopped; });


                    if (_stopped) {
                        _run_threads_count--;
                        break;
                    }

                    assert(!_queue.empty());

                    task = _queue.front();
                    _queue.pop();
                }
                try {
                    task();
                } catch (std::exception &e) {
                    std::cout << "Exception:" << e.what() << std::endl;
                }
            }
        }).detach();
    }
}

void ThreadPool::addTask(ThreadPool::TaskProc task) {
    assert(!_stopped);

    std::unique_lock<std::mutex> lock(_mutex);

    _queue.push(task);

    _active_task_count++;

    _queue_notify.notify_one();
}

void ThreadPool::stop() {
    std::unique_lock<std::mutex> lock(_mutex);

    _active_task_count = 0;
    _stopped = true;

    _queue_notify.notify_all();


    while (_run_threads_count != 0) {
        lock.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        lock.lock();
    }


    lock.unlock();
}

bool ThreadPool::isStopped() const {
    return _stopped;
}

ThreadPool &ThreadPool::getInstance(size_t size) {
    static ThreadPool _pool(size);
    return _pool;
}

size_t ThreadPool::getActiveTaskCount() const {
    std::unique_lock<std::mutex> lock(_mutex);

    return _active_task_count;
}

void ThreadPool::waitForTasks() {
    std::unique_lock<std::mutex> lock(_mutex);
    while (_active_task_count != 0) {
        lock.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        lock.lock();
    }

    lock.unlock();
}
