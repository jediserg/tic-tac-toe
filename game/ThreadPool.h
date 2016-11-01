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

    static ThreadPool &getInstance(size_t size = 4);
private:

    std::queue<TaskProc> _queue;
    std::mutex _mutex;
    std::condition_variable _queue_notify;
    bool _stopped;
};

#endif //TIC_TAC_TOE_THREADPOOL_H
