#include <cstdlib>

#include <iostream>
#include <thread>
#include <shared_mutex>

int a = 0;
std::shared_mutex mtx; 

void thread_func()
{
  std::cout << "Thread function" << std::endl;

  for(int i = 0 ; i < 10000 ; i++)
  {
    mtx.lock_shared();
    std::cout << std::this_thread::get_id() << " " << a << std::endl;

    /* shared_mutex costs a bit more than mutex 
     * BUT for long "read" operation it is better than mutex
     */
    /* simulate long operation ! */
    std::this_thread::sleep_for(std::chrono::microseconds(10));
    mtx.unlock();
  }
}

int main(int argc, char** argv)
{
  std::thread threads[5];

  for(size_t i = 0 ; i < 5 ; i++)
  {
    threads[i] = std::thread(thread_func);
  }

  for(int i = 0 ; i < 250 ; i++)
  {
    mtx.lock();
    a++;
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    mtx.unlock();
  }

  for(size_t i = 0 ; i < 5 ; i++)
  {
    threads[i].join();
  }

  std::cout << "a = " << a << std::endl;

  std::cout << "Exit program" << std::endl;
  return EXIT_SUCCESS;
}

