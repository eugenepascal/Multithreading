#include <cstdlib>

#include <iostream>
#include <chrono>
#include <thread>

void thread_func()
{
  std::cout << "Thread function" << std::endl;
}

int main(int argc, char** argv)
{
  std::thread thread(thread_func);

  thread.detach();

  /* no need to join */

  std::this_thread::sleep_for(std::chrono::seconds(5));
  std::cout << "Exit program" << std::endl;
  return EXIT_SUCCESS;
}

