// Ajouter C++ 20 dans les paramètres

#include <iostream>
#include <thread>
#include <chrono>
#include <barrier>

// Define the number of threads to create
const int NUM_THREADS = 10;

// This is the function each worker thread will run
void workerThread(std::barrier<>& barrier)
{
    // Wait for 5 seconds before attempting to cross the barrier
    std::this_thread::sleep_for(std::chrono::seconds(3));

    // Wait for all threads to reach the barrier
    barrier.arrive_and_wait();

    // At this point, the barrier has been reached, so print a message indicating which thread crossed it
    std::cout << "Thread " << std::this_thread::get_id() << " passed the barrier." << std::endl;
}

int main()
{
    // Create a barrier with the required number of threads
    std::barrier<> barrier(NUM_THREADS + 1);

    // Print a message indicating the start of the main thread
    std::cout << "Main thread started." << std::endl;

    // Create an array of threads to run the worker function
    std::thread threads[NUM_THREADS];

    // Create and start the worker threads
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        threads[i] = std::thread(workerThread, std::ref(barrier));
    }

    // Wait for all threads to reach the barrier
    barrier.arrive_and_wait();

    // At this point, the barrier has been reached, so print a message indicating the main thread crossed it
    std::cout << "Main thread passed the barrier." << std::endl;

    // Join all the worker threads back to the main thread
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        threads[i].join();
    }

    // Print a message indicating the end of the program
    std::cout << "Main thread finished." << std::endl;

    return 0;
}