#include <cstdlib>

#include <iostream>
#include <thread>
#include <array>

#include <semaphore>

std::counting_semaphore<2> sem(2);

void thread_func(int i)
{
  sem.acquire();
  std::cout << "Hello from thread " << i << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(2));
  sem.release();
}

int main(int argc, char** argv)
{
  std::array<std::thread, 10> th;
  size_t nb = th.size();

  for(size_t i = 0 ; i < nb ; i++)
  {
    th[i] = std::thread(thread_func, i);
  }

  for(size_t i = 0 ; i < nb ; i++)
  {
    th[i].join();
  }
  std::cout << "Exit program" << std::endl;
  return EXIT_SUCCESS;
}

