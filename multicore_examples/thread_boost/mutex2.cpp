#include <cstdlib>

#include <iostream>

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

int a = 0;
boost::mutex mtx; 

void thread_func()
{
  std::cout << "Thread function" << std::endl;

  for(int i = 0 ; i < 1000000 ; i++)
  {
    mtx.lock();
    a++;
    mtx.unlock();
  }
}

int main(int argc, char** argv)
{
  boost::thread thread(thread_func);
  boost::thread thread2(thread_func);

  thread.join();
  thread2.join();

  std::cout << "a =" << a << std::endl;

  std::cout << "Exit program" << std::endl;
  return EXIT_SUCCESS;
}

