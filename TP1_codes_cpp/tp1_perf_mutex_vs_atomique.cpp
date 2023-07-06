#include <cstdlib>
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <time.h>

std::atomic_int var_atomic = 0;

int var_global = 0;

std::mutex mut;

// fonction pour exécuter chaque thread
void thread_function_atomic() {
    for (int i = 0; i < 1000000; i++)
    {
        var_atomic += 1;
    }
}

// fonction pour exécuter chaque thread
void thread_function_mutex() {
    mut.lock();
    for (int i = 0; i < 1000000; i++)
    {
        var_global += 1;
    }
    mut.unlock();
}

int main() {
    clock_t begin;
    clock_t end;

    var_atomic = 0;
    begin = clock();
    std::thread thread1(thread_function_atomic);
    std::thread thread2(thread_function_atomic);
    std::thread thread3(thread_function_atomic);
    thread1.join();
    thread2.join();
    thread3.join();
    end = clock();
    std::cout << "var: " << var_atomic << std::endl;
    std::cout << "atomic_exec_time: " << (double)(end - begin) / CLOCKS_PER_SEC << std::endl;


    var_global = 0;
    begin = clock();
    std::thread thread4(thread_function_mutex);
    std::thread thread5(thread_function_mutex);
    std::thread thread6(thread_function_mutex);
    thread4.join();
    thread5.join();
    thread6.join();
    end = clock();
    std::cout << "var: " << var_global << std::endl;
    std::cout << "mutex_exec_time: " << (double)(end - begin) / CLOCKS_PER_SEC << std::endl;

    std::cout << "Exit program" << std::endl;
    return EXIT_SUCCESS;
}