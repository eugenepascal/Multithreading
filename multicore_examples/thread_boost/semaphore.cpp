#include <cstdlib>

#include <iostream>
#include <boost/thread/thread.hpp>

#include <boost/interprocess/sync/interprocess_semaphore.hpp>

boost::interprocess::interprocess_semaphore sem(2);

void thread_func(int i)
{
  sem.wait();
  std::cout << "Hello from thread " << i << std::endl;
  boost::this_thread::sleep_for(boost::chrono::seconds(1));
  sem.post();
}

int main(int argc, char** argv)
{
  std::array<boost::thread, 10> th;
  size_t nb = th.size();

  for(size_t i = 0 ; i < nb ; i++)
  {
    th[i] = boost::thread(thread_func, i);
  }

  for(size_t i = 0 ; i < nb ; i++)
  {
    th[i].join();
  }
  std::cout << "Exit program" << std::endl;
  return EXIT_SUCCESS;
}

