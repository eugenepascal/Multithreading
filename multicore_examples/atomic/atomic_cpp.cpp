#include <cstdlib>

#include <iostream>
#include <array>
#include <atomic>
#include <thread>

static std::atomic<int> a;
//static int a;

void thread_func()
{
  for(int i = 0 ; i < 1000000 ; i++)
  {
    //a.fetch_add(1);
    a++;
  }
}

int main(int argc, char** argv)
{
  std::array<std::thread, 2> th;
  size_t nb = th.size();

  a = 0;

  for(size_t i = 0 ; i < nb ; i++)
  {
    th[i] = std::thread(thread_func);
  }

  for(size_t i = 0 ; i < nb ; i++)
  {
    /* wait threads to terminate */
    th[i].join();
  }

  std::cout << "a = ";
  std::cout << a << std::endl;

  std::cout << "Exit program" << std::endl;
  return EXIT_SUCCESS;
}

