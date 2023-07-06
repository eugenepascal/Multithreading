#include <cstdlib>

#include <iostream>
#include <boost/thread/thread.hpp>

void thread_func()
{
  std::cout << "Thread function" << std::endl;
}

int main(int argc, char** argv)
{
  boost::thread thread(thread_func);

  boost::thread thread2([]() {
        std::cout << "Thread lambda !" << std::endl;
      });

  thread.join();
  thread2.join();

  std::cout << "Exit program" << std::endl;
  return EXIT_SUCCESS;
}

