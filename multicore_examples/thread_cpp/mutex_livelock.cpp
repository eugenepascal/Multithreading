#include <cstdlib>

#include <iostream>
#include <thread>
#include <mutex>

int a = 0;
std::mutex mtx; 
std::mutex mtx2; 

void thread_func()
{
  //std::cout << "Thread function" << std::endl;
  //
  mtx.lock();
  std::this_thread::sleep_for(std::chrono::microseconds(10));
  while(!mtx2.try_lock())
  {
    // other work
    //std::this_thread::sleep_for(std::chrono::microseconds(10));
  }
  mtx2.unlock();
  std::cout << "Thread 1 never happened" << std::endl;
  mtx.unlock();
 
  std::this_thread::sleep_for(std::chrono::microseconds(10));
}

void thread_func2()
{
  //std::cout << "Thread2 function" << std::endl;
  mtx2.lock();
  std::this_thread::sleep_for(std::chrono::microseconds(10));
  while(!mtx.try_lock())
  {
    // other work
  }
  mtx.unlock();
  std::cout << "Thread 2 never happened" << std::endl;
  mtx2.unlock();

  std::this_thread::sleep_for(std::chrono::microseconds(10));
}


int main(int argc, char** argv)
{
  std::thread thread(thread_func);
  std::thread thread2(thread_func2);

  thread.join();
  thread2.join();

  std::cout << "a = " << a << std::endl;

  std::cout << "Exit program" << std::endl;
  return EXIT_SUCCESS;
}

