#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <chrono>

//Les différents codes


//Création
/*std::mutex mtx; // Mutex pour synchroniser l'accès à la sortie standard

// Fonction pour afficher le message des threads
void print_hello(int thread_num) {
    // Verrouiller le mutex avant d'écrire dans la sortie standard
    std::unique_lock<std::mutex> lock(mtx);
    std::cout << "Hello thread " << thread_num << std::endl;
    lock.unlock(); // Déverrouiller le mutex après avoir écrit
}

int main() {
    // Créer un vecteur pour stocker les threads
    std::vector<std::thread> threads;

    // Créer et lancer trois threads
    for (int i = 1; i <= 3; ++i) {
        threads.emplace_back(print_hello, i);
    }

    // Joindre les threads dans le thread principal
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    return 0;
}
*/

// Partage de données & verrou

/*
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

std::mutex mtx; // Mutex pour synchroniser l'accès à la sortie standard et à la variable partagée

int shared_variable = 0; // Variable partagée initialisée à 0

// Fonction pour afficher le message des threads et incrémenter la variable partagée
void increment_shared_variable(int thread_num) {
    for (int i = 0; i < 100000; ++i) {
        // Verrouiller le mutex avant d'accéder à la variable partagée
        std::unique_lock<std::mutex> lock(mtx);
        ++shared_variable;
        lock.unlock(); // Déverrouiller le mutex après avoir modifié la variable
    }

    // Verrouiller le mutex avant d'écrire dans la sortie standard
    std::unique_lock<std::mutex> lock(mtx);
    std::cout << "Hello thread " << thread_num << std::endl;
    lock.unlock(); // Déverrouiller le mutex après avoir écrit
}

int main() {
    // Créer un vecteur pour stocker les threads
    std::vector<std::thread> threads;

    // Créer et lancer trois threads
    for (int i = 1; i <= 3; ++i) {
        threads.emplace_back(increment_shared_variable, i);
    }

    // Joindre les threads dans le thread principal
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    std::cout << "Shared variable value: " << shared_variable << std::endl;

    return 0;
}
*/

//Atomique

/*#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>

std::mutex mtx; // Mutex pour synchroniser l'accès à la sortie standard

std::atomic<int> shared_variable{0}; // Variable partagée initialisée à 0, en utilisant std::atomic

// Fonction pour afficher le message des threads et incrémenter la variable partagée
void increment_shared_variable(int thread_num) {
    for (int i = 0; i < 100000; ++i) {
        ++shared_variable; // Incrémentation atomique de la variable partagée
    }

    // Verrouiller le mutex avant d'écrire dans la sortie standard
    std::unique_lock<std::mutex> lock(mtx);
    std::cout << "Hello thread " << thread_num << std::endl;
    lock.unlock(); // Déverrouiller le mutex après avoir écrit
}

int main() {
    // Créer un vecteur pour stocker les threads
    std::vector<std::thread> threads;

    // Créer et lancer trois threads
    for (int i = 1; i <= 3; ++i) {
        threads.emplace_back(increment_shared_variable, i);
    }

    // Joindre les threads dans le thread principal
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    std::cout << "Shared variable value: " << shared_variable.load() << std::endl;

    return 0;
}
*/

//version mutex
// Ajoutez l'en-tête chrono

/*
int main() {
    // ...
    auto start = std::chrono::high_resolution_clock::now(); // Enregistrez le temps de début

    // Créez et lancez les threads, puis joignez-les (comme dans le code précédent)

    auto end = std::chrono::high_resolution_clock::now(); // Enregistrez le temps de fin
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Mutex version took: " << duration << " milliseconds" << std::endl;

    return 0;
}
*/

 // version atomique


 int main() {

    auto start = std::chrono::high_resolution_clock::now(); // Enregistrez le temps de début

    // Créez et lancez les threads, puis joignez-les (comme dans le code précédent)

    auto end = std::chrono::high_resolution_clock::now(); // Enregistrez le temps de fin
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Atomic version took: " << duration << " milliseconds" << std::endl;

    return 0;
}

//la version atomique est plus rapide que la version mutex. Les raisons en sont les suivantes :
//Les opérations atomiques sont généralement plus rapides que l'acquisition et la libération d'un verrou mutex, car elles sont
// optimisées pour les opérations de bas niveau sur le matériel.
//La contention pour le verrou dans la version mutex peut entraîner des attentes, ce qui ralentit l'exécution globale du programme.
//Cependant, il est important de noter que les opérations atomiques ne sont pas toujours plus rapides que les mutex, et
// leur performance peut varier en fonction de l'architecture matérielle et de la complexité des opérations. Les opérations atomiques sont mieux adaptées aux scénarios où les
// opérations sur les données partagées sont simples, tandis que les mutex sont plus appropriés pour protéger des sections de code plus importantes et complexes.
