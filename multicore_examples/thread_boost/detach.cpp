#include <cstdlib>

#include <iostream>
#include <chrono>
#include <boost/thread/thread.hpp>

void thread_func()
{
  std::cout << "Thread function" << std::endl;
}

int main(int argc, char** argv)
{
  boost::thread thread(thread_func);

  thread.detach();

  /* no need to join */

  boost::this_thread::sleep_for(boost::chrono::seconds(5));
  std::cout << "Exit program" << std::endl;
  return EXIT_SUCCESS;
}

