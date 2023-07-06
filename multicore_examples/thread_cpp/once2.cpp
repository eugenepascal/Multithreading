#include <cstdlib>

#include <iostream>
#include <stdexcept>
#include <thread>
#include <mutex>

std::once_flag once;

// Warnings: it may not work on gcc / clang
// see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=66146
void init_once(int a)
{
  if(a == 0)
  {
    throw std::exception();
  }
  std::cout << "Once initialization " <<
    "a = " << a << std::endl;
}

void thread_func(int a)
{
  //std::cout << "Thread function" << std::endl;

  try
  {
    std::call_once(once, init_once, a);
  }
  catch(std::exception& e)
  {
    std::cout << "init_once failed, it will retry" << std::endl;
  }
  
  std::cout << "exit" << std::endl;
}

int main(int argc, char** argv)
{
  std::thread threads[4];

  for(size_t i = 0 ; i < 4 ; i++)
  {
    threads[i] = std::thread(thread_func, i);
  }

  for(size_t i = 0 ; i < 4 ; i++)
  {
    threads[i].join();
  }

  std::cout << "Exit program" << std::endl;
  return EXIT_SUCCESS;
}

