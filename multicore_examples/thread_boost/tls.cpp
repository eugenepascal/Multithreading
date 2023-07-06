#include <cstdlib>

#include <iostream>

#include <boost/thread/thread.hpp>
#include <boost/thread/tss.hpp>

class ThreadInfo
{
  public:
    ThreadInfo(const std::string& name, int val)
      : m_name("Thread " + name),
      m_val(val)
    {
    }

    void print() const
    {
      std::cout << m_name << " val = " << m_val << std::endl;
    }

  private:
    std::string m_name;
    int m_val;
};

static boost::thread_specific_ptr<class ThreadInfo> tss;

void thread_func(const std::string& name, int val)
{
  std::cout << "Thread function" << std::endl;

  if(!tss.get())
  {
    tss.reset(new ThreadInfo(name, val));
  }

  boost::this_thread::sleep_for(boost::chrono::seconds(1));
  tss->print();
}

int main(int argc, char** argv)
{
  boost::thread thread(thread_func, "A", 1);
  boost::thread thread2(thread_func, "B", 2);

  thread.join();
  thread2.join();

  std::cout << "Exit program" << std::endl;
  return EXIT_SUCCESS;
}

