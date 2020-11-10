#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>


std::mutex cv_num_guard;
std::mutex cv_char_guard;
std::condition_variable cv_num;
std::condition_variable cv_char;


void print_num()
{

    for ( int i = 1; i <= 26; ++i ) {

        if ( i != 1 ) {
            std::unique_lock<std::mutex> guard{cv_num_guard};
            cv_num.wait(guard);
        }

        std::cout << i;

        cv_char.notify_one();

    }
}

void print_char()
{

    for ( char i = 'A'; i <= 'Z'; ++i ) {

        std::unique_lock<std::mutex> guard{cv_char_guard};
        cv_char.wait(guard);

        std::cout << i;

        cv_num.notify_one();

    }
}


int main()
{

    std::thread th_print_char{print_char};
    std::thread th_print_num{print_num};

    th_print_char.join();
    th_print_num.join();
    
    return 0;
}

