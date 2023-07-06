#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

const int NUM_THREADS = 10;
std::mutex mutex;
std::condition_variable condition;
int notification_count = 0;

void work(int threadID)
{
    std::unique_lock<std::mutex> lock(mutex);
    condition.wait(lock);

    while (notification_count < 10)
    {
        std::cout << "Thread #" << threadID << " notified" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        condition.wait(lock);
    }
}

int main()
{
    std::thread th[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; ++i)
    {
        th[i] = std::thread(work, i);
    }

    std::cout << "Main thread started" << std::endl;

    while (notification_count < 10)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        condition.notify_all();
        ++notification_count;
    }

    std::cout << "Main thread finished" << std::endl;

    for (int i = 0; i < NUM_THREADS; ++i)
    {
        th[i].join();
    }

    return 0;
}