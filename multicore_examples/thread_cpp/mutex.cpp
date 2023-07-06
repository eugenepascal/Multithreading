#include <cstdlib>

#include <iostream>
#include <thread>

int a = 0;

void thread_func()
{
  std::cout << "Thread function" << std::endl;

  for(int i = 0 ; i < 1000000 ; i++)
  {
    a++;
  }
}

int main(int argc, char** argv)
{
  std::thread thread(thread_func);
  std::thread thread2(thread_func);

  thread.join();
  thread2.join();

  std::cout << "a = " << a << std::endl;

  std::cout << "Exit program" << std::endl;
  return EXIT_SUCCESS;
}

