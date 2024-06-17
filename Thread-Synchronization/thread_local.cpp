#include <thread>
#include <iostream>
#include <random>


/**
 * thread_local keyword is used to declare a variable that will have a separate copy for each thread.
 * The variable will be initialized when the thread is created and will be destroyed when the thread is destroyed.
 * 
 * thread_local variables are stored in the thread's local storage.
 * 
 * thread_local variables are initialized before the thread function is called.
 * 
 * thread_local variables are destroyed after the thread function is called
 * 
 * thread_local variables are not shared between threads.
 * 
 */

thread_local std::mt19937 mt; // Mersenne Twister pseudo-random number generator

void tl_function() {
    std::uniform_int_distribution<int> dist(1, 100);
    std::cout << " { " << std::this_thread::get_id() << " } : ";
    for(int i = 0 ; i < 10 ; i++) {
        std::cout << dist(mt) << ' ';
    }
    std::cout << std::endl;
}

std::mt19937 mt_global;

void g_function() {
    std::uniform_int_distribution<int> dist(1, 100);
    std::cout << " { " << std::this_thread::get_id() << " } : ";
    for(int i = 0 ; i < 10 ; i++) {
        std::cout << dist(mt_global) << ' ';
    }
    std::cout << std::endl;
}


int main() {
    std::cout << " Outputs on global variable => " << std::endl;
    std::thread t1(g_function);
    t1.join();
    std::thread t2(g_function);
    t2.join();

    std::cout << " Outputs on thread_local variable => " << std::endl;
    std::thread t3(tl_function);
    t3.join();
    std::thread t4(tl_function);
    t4.join();
    return EXIT_SUCCESS;
}