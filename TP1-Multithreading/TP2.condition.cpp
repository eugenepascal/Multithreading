#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>

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
