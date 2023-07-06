#include <cstdlib>

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

int good_a = 0;
std::mutex mtx;
std::condition_variable cond;

void thread_func()
{
  int a = 0;

  for(int i = 0 ; i < 1000000 ; i++)
  {
    a++;

    if(a == 1000000)
    {
      std::unique_lock<std::mutex> lock(mtx);
      good_a = 1;
      cond.notify_all();
    }
  }
}

int main(int argc, char** argv)
{
  std::thread th(thread_func);

  {
    std::unique_lock<std::mutex> lock(mtx);
    while(good_a != 1)
    {
      std::cout << "Wait for good_a to be 1" << std::endl;
      cond.wait(lock);
    }
  }

  std::cout << "a = " << good_a << std::endl;

  /* wait threads to terminate */
  th.join();

  std::cout << "Exit program" << std::endl;
  return EXIT_SUCCESS;
}

