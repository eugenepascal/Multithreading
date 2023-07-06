#include <cstdlib>

#include <iostream>
#include <thread>

thread_local int a = 0;

void thread_func(int val)
{
  std::cout << "Thread function" << std::endl;

  // asign thread_local
  a = val;

  std::cout << "Thread " << val << " " << a << std::endl; 
}

int main(int argc, char** argv)
{
  std::thread thread(thread_func, 1);
  std::thread thread2(thread_func, 2);

  thread.join();
  thread2.join();

  std::cout << "a = " << a << std::endl;

  std::cout << "Exit program" << std::endl;
  return EXIT_SUCCESS;
}

