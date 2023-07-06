#include <cstdlib>
#include <iostream>
#include <thread>

// fonction pour exécuter chaque thread
void thread_function(int id) {
    int thread_num = id; // récupère le numéro de thread
    printf("Hello thread %d\n", thread_num); // affiche le message avec le numéro de thread
}

int main() {
    std::thread thread1(thread_function,1);
    std::thread thread2(thread_function,2);
    std::thread thread3(thread_function,3);

    thread1.join();
    thread2.join();
    thread3.join();

    std::cout << "Exit program" << std::endl;
    return EXIT_SUCCESS;
}