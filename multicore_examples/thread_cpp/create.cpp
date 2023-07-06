#include <cstdlib>

#include <iostream>
#include <thread>

void thread_func()
{
  std::cout << "Thread function" << std::endl;
}

int main(int argc, char** argv)
{
  std::thread thread(thread_func);

  std::thread thread2([]() {
        std::cout << "Thread lambda !" << std::endl;
      });

  thread.join();
  thread2.join();

  std::cout << "Exit program" << std::endl;
  return EXIT_SUCCESS;
}

