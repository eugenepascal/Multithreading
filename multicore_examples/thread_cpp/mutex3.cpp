#include <cstdlib>

#include <iostream>
#include <thread>
#include <mutex>

int a = 0;
std::mutex mtx; 

void thread_func()
{
  std::cout << "Thread function" << std::endl;

  for(int i = 0 ; i < 10000 ; i++)
  {
    mtx.lock();
    std::cout << std::this_thread::get_id() << " " << a << std::endl;
    std::this_thread::sleep_for(std::chrono::microseconds(10));
    mtx.unlock();
  }
}

int main(int argc, char** argv)
{
  std::thread thread(thread_func);
  std::thread thread2(thread_func);

  for(int i = 0 ; i < 250 ; i++)
  {
    mtx.lock();
    a++;
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    mtx.unlock();
  }

  thread.join();
  thread2.join();

  std::cout << "a = " << a << std::endl;

  std::cout << "Exit program" << std::endl;
  return EXIT_SUCCESS;
}

