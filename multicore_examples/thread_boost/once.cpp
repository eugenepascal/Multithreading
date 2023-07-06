#include <cstdlib>

#include <iostream>

#include <boost/thread/thread.hpp>
#include <boost/thread/once.hpp>

boost::once_flag once = BOOST_ONCE_INIT;

void thread_func()
{
  std::cout << "Thread function" << std::endl;

  boost::call_once(
      once,
      []() {
        std::cout << "Once initialization";
      });
}

int main(int argc, char** argv)
{
  boost::thread thread(thread_func);
  boost::thread thread2(thread_func);

  thread.join();
  thread2.join();

  std::cout << "Exit program" << std::endl;
  return EXIT_SUCCESS;
}

