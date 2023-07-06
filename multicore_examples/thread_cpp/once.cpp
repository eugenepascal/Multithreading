#include <cstdlib>

#include <iostream>
#include <thread>
#include <mutex>

std::once_flag once;

void init_once(int a, int b)
{
  std::cout << "Once initialization " <<
    "a = " << a << " " <<
    "b = " << b << std::endl;
}

void thread_func()
{
  std::cout << "Thread function" << std::endl;
  std::call_once(once, init_once, 1, 2);

  // lambda version
  /*
  std::call_once(once,
      [](int a, int b) {
        std::cout << "Once initialization " <<
        "a = " << a << " " <<
        "b = " << b << std::endl;
      },
      1,
      2 
    );
  */
}

int main(int argc, char** argv)
{
  std::thread threads[4];

  for(size_t i = 0 ; i < 4 ; i++)
  {
    threads[i] = std::thread(thread_func);
  }

  for(size_t i = 0 ; i < 4 ; i++)
  {
    threads[i].join();
  }

  std::cout << "Exit program" << std::endl;
  return EXIT_SUCCESS;
}

