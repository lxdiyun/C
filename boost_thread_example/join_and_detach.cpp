#include <iostream>
#include <boost/thread.hpp>

namespace
{
    class Callable
    {
    private:
        int value_;
    public:
        Callable(int value) : value_(value) {}

        void operator()()
        {
            std::cout << "cout down ";
            while(value_ > 0)
            {
                std::cout << value_ << ' ';
                boost::this_thread::sleep(boost::posix_time::seconds(1));
                --value_;
            }
            std::cout << "done" << std::endl;
        }
    };
}

void t02()
{
    std::cout << "Launching a thread" << std::endl;

    boost::thread t1(Callable(6));
    t1.timed_join(boost::posix_time::seconds(2));
    std::cout << std::endl << "Expired waiting for timed_join()" << std::endl;

    t1.join();
    std::cout << "Secondary thread joined" << std::endl;

    Callable c2(3);
    boost::thread t2(c2);
    t2.detach();

    std::cout << "Secondary thread detached" << std::endl;
    boost::this_thread::sleep(boost::posix_time::seconds(5));
    std::cout << "Done thread testing" << std::endl;
}

