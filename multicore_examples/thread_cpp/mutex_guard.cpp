#include <cstdlib>

#include <iostream>
#include <thread>
#include <mutex>

int a = 0;
std::mutex mtx; 

void thread_func()
{
  std::cout << "Thread function" << std::endl;

  for(int i = 0 ; i < 1000000 ; i++)
  {
    std::lock_guard<std::mutex> lock(mtx);
    a++;
  }
}

int main(int argc, char** argv)
{
  std::thread thread(thread_func);
  std::thread thread2(thread_func);

  thread.join();
  thread2.join();

  std::cout << a << std::endl;

  std::cout << "Exit program" << std::endl;
  return EXIT_SUCCESS;
}

