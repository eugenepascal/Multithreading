#include <cstdlib>

#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx; 
std::mutex mtx2; 

void thread_func()
{
  std::this_thread::sleep_for(std::chrono::microseconds(10));

  mtx.lock();
  std::this_thread::sleep_for(std::chrono::microseconds(10));
  mtx2.lock();
  std::this_thread::sleep_for(std::chrono::microseconds(10));
  mtx2.unlock();
  mtx.unlock();
}

void thread_func2()
{
  std::this_thread::sleep_for(std::chrono::microseconds(10));

  mtx2.lock();
  std::this_thread::sleep_for(std::chrono::microseconds(2));
  mtx.lock();
  std::this_thread::sleep_for(std::chrono::microseconds(10));
  mtx.unlock();
  mtx2.unlock();
}


int main(int argc, char** argv)
{
  std::thread thread(thread_func);
  std::thread thread2(thread_func2);

  thread.join();
  thread2.join();

  std::cout << "Exit program" << std::endl;
  return EXIT_SUCCESS;
}

