//
// Created by serg on 11/1/16.
//
#include <gtest/gtest.h>
#include "ThreadPool.h"
#include <thread>

TEST(ThreadPoolTests, SimpleChecks) {
    ThreadPool pool(4);

    EXPECT_FALSE(pool.isStopped());

    pool.stop();

    EXPECT_TRUE(pool.isStopped());
}

TEST(ThreadPoolTests, TestTasks) {
    ThreadPool pool(7);

    const size_t TASK_COUNT = 1000;

    char tasks_complete[TASK_COUNT] = {0};

    for(int i = 0; i < TASK_COUNT; i++)
        pool.addTask([&tasks_complete, i](){
            std::this_thread::sleep_for(std::chrono::milliseconds(10));

            tasks_complete[i] = 1;
        });

   while(true){
       for(int i = 0; i < TASK_COUNT; ++i){
           if(tasks_complete[i] == 0) {
               std::this_thread::sleep_for(std::chrono::milliseconds(10));
               continue;
           }
       }
       break;
   }
}

char *test_data = NULL;

TEST(ThreadPoolTests, WaitForTasks) {
    try {
        ThreadPool pool(90);

        const size_t TASK_COUNT = 20000;

        char tasks_complete[TASK_COUNT] = {0};
        test_data = tasks_complete;

        for (int i = 0; i < TASK_COUNT; i++)
            pool.addTask([&tasks_complete, i]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));

                tasks_complete[i] = 1;
            });

        //EXPECT_EQ(TASK_COUNT, pool.getActiveTaskCount());

        pool.waitForTasks();

        EXPECT_EQ(0, pool.getActiveTaskCount());
    } catch (const std::exception &e) {
        std::cout << "Exception:" << e.what() << std::endl;
    }
}
