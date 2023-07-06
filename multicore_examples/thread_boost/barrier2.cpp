#include <cstdlib>

#include <iostream>

#include <boost/thread/thread.hpp>
#include <boost/thread/latch.hpp>

static int a = 0;
static boost::latch barrier(11);

void thread_func(int arg)
{
  int idx = arg; 
  std::cout << "Thread " << idx << " starts" << std::endl;

  for(int i = 0 ; i < 1000 ; i++)
  {
    a++;
  }

  std::cout << "Thread " << idx << " wait" << std::endl;
  barrier.count_down();
  barrier.wait();
}

int main(int argc, char** argv)
{
  std::array<boost::thread, 10> th;
  size_t nb = th.size();

  for(size_t i = 0 ; i < nb ; i++)
  {
    th[i] = boost::thread(thread_func, i);
  }

  barrier.count_down_and_wait();
  std::cout << "All threads are initialized" << std::endl;

  for(size_t i = 0 ; i < nb ; i++)
  {
    /* wait threads to terminate */
    th[i].join();
  }

  std::cout << "a = " << a << std::endl;

  std::cout << "Exit program" << std::endl;
  return EXIT_SUCCESS;
}

