#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <atomic>
#include <latch>
#include <syncstream>
//Condition
/*
std::mutex mtx; // Mutex pour protéger la condition
std::condition_variable cv; // Condition variable pour la notification
bool is_notified = false; // Variable pour vérifier si la notification a été reçue

void sleep_and_notify() {
    std::this_thread::sleep_for(std::chrono::seconds(5)); // Le thread s'endort pendant 5 secondes

    {
        std::unique_lock<std::mutex> lock(mtx); // Verrouille le mutex
        is_notified = true; // Indique que la notification a été envoyée
    }
    cv.notify_one(); // Notifie le thread principal
}

int main() {
    std::thread work_thread(sleep_and_notify); // Crée un thread work

    {
        std::unique_lock<std::mutex> lock(mtx); // Verrouille le mutex
        cv.wait(lock, [] { return is_notified; }); // Attend la notification
    }

    std::cout << "Received notification from work thread." << std::endl;

    work_thread.join(); // Joindre le thread work dans le thread principal

    return 0;
}
*/



//Condition 2

/*
constexpr int NUM_THREADS = 10;
constexpr int NUM_NOTIFICATIONS = 10;

std::mutex mtx; // Mutex pour protéger la condition
std::condition_variable cv; // Condition variable pour la notification
std::atomic<int> notification_count{0}; // Compte des notifications
bool stop_threads = false; // Indicateur pour arrêter les threads

void work_thread(int id) {
    while (!stop_threads) {
        std::unique_lock<std::mutex> lock(mtx); // Verrouille le mutex
        cv.wait(lock); // Attend la notification

        if (!stop_threads) {
            std::cout << "Thread " << id << " received notification." << std::endl;
            lock.unlock(); // Déverrouille le mutex avant de dormir
            std::this_thread::sleep_for(std::chrono::seconds(1)); // Dors pendant 1 seconde
        }
    }
}

int main() {
    std::vector<std::thread> threads;

    // Crée 10 threads work
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back(work_thread, i);
    }

    // Le thread principal notifie les threads work toutes les secondes
    for (int i = 0; i < NUM_NOTIFICATIONS; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        {
            std::scoped_lock<std::mutex> lock(mtx);
            ++notification_count;
        }
        cv.notify_all(); // Notifie tous les threads work
    }

    // Arrête les threads work
    {
        std::scoped_lock<std::mutex> lock(mtx);
        stop_threads = true;
    }
    cv.notify_all(); // Notifie tous les threads work pour qu'ils puissent vérifier stop_threads

    // Joindre les threads work
    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
*/

constexpr int NUM_THREADS = 10;

std::latch latch(NUM_THREADS + 1); // Latch pour 11 threads (10 work + 1 main)

void work_thread(int id) {
    std::this_thread::sleep_for(std::chrono::seconds(5)); // Attends 5 secondes
    latch.count_down(); // Décompte le latch
    std::osyncstream(std::cout) << "Work thread " << id << " passed the latch." << std::endl;
}

int main() {
    std::vector<std::thread> threads;

    // Crée 10 threads work
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back(work_thread, i);
    }

    // Le thread principal attend le latch
    latch.wait();
    std::cout << "Main thread passed the latch." << std::endl;

    // Joindre les threads work
    for (auto& t : threads) {
        t.join();
    }

    return 0;
}