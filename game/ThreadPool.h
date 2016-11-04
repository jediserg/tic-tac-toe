/*!
 * \brief Thread pool
 */

#ifndef TIC_TAC_TOE_THREADPOOL_H
#define TIC_TAC_TOE_THREADPOOL_H

#include <functional>
#include <string>
#include <queue>
#include <mutex>

#include <condition_variable>

class ThreadPool {
public:
    using TaskProc = std::function<void(void)>;

    ThreadPool(size_t N);

    void addTask(TaskProc task);

    void stop();

    bool isStopped() const;

    void waitForTasks();

    size_t getActiveTaskCount() const;


    static ThreadPool &getInstance(size_t size = 4);
private:

    std::queue<TaskProc> _queue;
    mutable std::mutex _mutex;
    std::condition_variable _queue_notify;
    std::condition_variable _task_count_notify;
    bool _stopped;
    size_t _active_task_count;
    size_t _run_threads_count;
};

#endif //TIC_TAC_TOE_THREADPOOL_H
