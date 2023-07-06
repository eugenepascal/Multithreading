#include <cstdlib>

#include <iostream>

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

int good_a = 0;
boost::mutex mtx;
boost::condition_variable cond;

void thread_func()
{
  int a = 0;

  for(int i = 0 ; i < 1000000 ; i++)
  {
    a++;

    if(a == 1000000)
    {
      boost::unique_lock<boost::mutex> lock(mtx);
      good_a = 1;
      cond.notify_all();
    }
  }
}

int main(int argc, char** argv)
{
  boost::thread th(thread_func);

  {
    boost::unique_lock<boost::mutex> lock(mtx);
    std::cout << "Wait for good_a to be 1" << std::endl;
    cond.wait(lock, []() {
          return good_a == 1;
        });
  }

  std::cout << "a = " << good_a << std::endl;

  /* wait threads to terminate */
  th.join();

  std::cout << "Exit program" << std::endl;
  return EXIT_SUCCESS;
}

