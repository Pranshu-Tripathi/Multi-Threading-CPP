#include <iostream>
#include <thread>
#include <mutex>

/**
 * recursive_mutex is a design smell.
 */

std::recursive_mutex mtx; /** multiple calls on lock is allowed. */

int factorial(int n) {
    if(n <= 1) {
        return 1;
    }

    std::lock_guard<std::recursive_mutex> lock(mtx);
    std::cout << "Thread ID: " << std::this_thread::get_id() << " calculating factorial of " << n << std::endl;
    return n * factorial(n-1);
}

int main() {
    std::thread t1(factorial, 4);
    std::thread t2(factorial, 5);
    t1.join();
    t2.join();
    return EXIT_SUCCESS;
}