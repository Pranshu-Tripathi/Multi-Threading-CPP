#include <thread>
#include <iostream>
#include <mutex>
#include <string>
using namespace std::literals;

/**
 * When directly interacting in critical section if there is any
 * exception and the mutex is not unlocked, then all the threads
 * will be blocked. To avoid this, we can use lock_guard.
 */
std::mutex task_mutex;						// Global mutex object
void task(const std::string& str)
{
	for (int i = 0; i < 5; ++i) {
		try {
			// Lock the mutex before the critical section
			int lock_attemps = 0;
            while(!task_mutex.try_lock() && lock_attemps < 5) {
                std::this_thread::sleep_for(100ms);
                lock_attemps++;
            }
            if (lock_attemps == 5)
                throw std::runtime_error("Resouce not available. Try again later.");

			// Start of critical section
			std::cout << str[0] << str[1] << str[2] << std::endl;

			// Critical section throws an exception
			throw std::exception();
			// End of critical section

			// Never gets called
			task_mutex.unlock();
			std::this_thread::sleep_for(50ms);
		}
		catch (std::exception& e) {
			std::cout << "Exception caught: " << e.what() << '\n';
		}
	}
}


void task_using_lock_guard(std::string&& str) {
    for(int i = 0 ; i < 5 ; i++) {
        try{
            /**
             * RAII wrapper for mutex. It locks the mutex in the constructor
             * and unlocks the mutex in the destructor. This way, even if an
             * exception is thrown, the mutex will be unlocked.
             * 
             * 
             * Lock Guard will always lock the mutex in constructor. If we want more flexibility,
             * then we can use std::unique_lock. It has a method try_lock() which will try to lock
             * the mutex and return immediately. If the mutex is not available, it will return false.
             * 
             * std::unique_lock<std::mutex> lock(task_mutex, std::defer_lock);
             * std::unique_lock<std::mutex> lock(task_mutex, std::try_to_lock);
             * std::unique_lock<std::mutex> lock(task_mutex, std::adopt_lock);
             */
            std::lock_guard<std::mutex> lock(task_mutex);
            std::cout << str[0] << str[1] << str[2] << std::endl;

            throw std::exception();
            std::this_thread::sleep_for(50ms);
        } catch(std::exception& e) {
            std::cout << "Exception caught: " << e.what() << '\n';
        }
    }
}

int main()
{
    {
        std::thread thr1(task, "abc");
        std::thread thr2(task, "def");
        std::thread thr3(task, "xyz");

        thr1.join(); thr2.join(); thr3.join();
    }
    std::cout << "      =      =      =      =      =      =       =" << std::endl;
    {
        if(!task_mutex.try_lock()) {
            std::cout << "Main thread acquired lock" << std::endl;
            task_mutex.unlock();
        }
        std::thread thr1(task_using_lock_guard, "abc");
        std::thread thr2(task_using_lock_guard, "def");
        std::thread thr3(task_using_lock_guard, "xyz");

        thr1.join(); thr2.join(); thr3.join();
    }
    return EXIT_SUCCESS;
}