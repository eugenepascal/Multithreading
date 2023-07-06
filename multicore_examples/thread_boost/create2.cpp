#include <cstdlib>

#include <iostream>
#include <boost/thread/thread.hpp>

void thread_func(int arg)
{
  std::cout << "Thread function" << std::endl;

  std::cout << "Parameter: " << arg << std::endl;
}

void thread_func_str(const std::string& str)
{
  std::cout << "Thread function" << std::endl;

  std::cout << "Parameter: " << str << std::endl;
}

int main(int argc, char** argv)
{
  int th_arg = 42;
  const std::string th_str = "mystr";

  boost::thread thread(thread_func, th_arg);
  boost::thread thread_str(thread_func_str, th_str);

  thread.join();
  thread_str.join();

  std::cout << "Exit program" << std::endl;
  return EXIT_SUCCESS;
}

