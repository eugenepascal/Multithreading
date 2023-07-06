#include <cstdlib>
#include <iostream>
#include <thread>

int var = 0;

// fonction pour exécuter chaque thread
void thread_function() {
    for (int i = 0; i < 100000; i++)
    {
        var += 1;
    }
}

int main() {
    std::thread thread1(thread_function);
    std::thread thread2(thread_function);
    std::thread thread3(thread_function);

    thread1.join();
    thread2.join();
    thread3.join();
   
    std::cout << "var: " << var << std::endl;

    std::cout << "Exit program" << std::endl;
    return EXIT_SUCCESS;
}