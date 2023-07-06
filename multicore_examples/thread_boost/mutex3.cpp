#include <cstdlib>

#include <iostream>

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

int a = 0;
boost::mutex mtx; 

void thread_func()
{
  std::cout << "Thread function" << std::endl;

  for(int i = 0 ; i < 10000 ; i++)
  {
    mtx.lock();
    std::cout << boost::this_thread::get_id() << " " << a << std::endl;
    boost::this_thread::sleep_for(boost::chrono::microseconds(10));
    mtx.unlock();
  }
}

int main(int argc, char** argv)
{
  boost::thread thread(thread_func);
  boost::thread thread2(thread_func);

  for(int i = 0 ; i < 250 ; i++)
  {
    mtx.lock();
    a++;
    boost::this_thread::sleep_for(boost::chrono::microseconds(100));
    mtx.unlock();
  }

  thread.join();
  thread2.join();

  std::cout << "a = " << a << std::endl;

  std::cout << "Exit program" << std::endl;
  return EXIT_SUCCESS;
}

