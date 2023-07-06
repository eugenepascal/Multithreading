#include <cstdlib>

#include <iostream>

#include <boost/thread/thread.hpp>
#include <boost/thread/shared_mutex.hpp>

int a = 0;
boost::shared_mutex mtx; 

void thread_func()
{
  std::cout << "Thread function" << std::endl;

  for(int i = 0 ; i < 10000 ; i++)
  {
    mtx.lock_shared();
    std::cout << boost::this_thread::get_id() << " " << a << std::endl;
    mtx.unlock();
  }
}

int main(int argc, char** argv)
{
  boost::thread threads[5];

  for(size_t i = 0 ; i < 5 ; i++)
  {
    threads[i] = boost::thread(thread_func);
  }

  for(int i = 0 ; i < 250 ; i++)
  {
    mtx.lock();
    a++;
    boost::this_thread::sleep_for(boost::chrono::microseconds(100));
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

