#include <iostream>
#include <thread>
#include <future>

int get_gcd(int a, int b) {
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    if( a < 0 || b < 0) {
        throw std::invalid_argument("Invalid arguments : a and b should be positive integers.");
    }
    if(b == 0) {
        return a;
    }
    return get_gcd(b, a % b);
}

int main() {
    auto future = std::async(get_gcd, 20, 30);

    /**
     * wait_for() and wait_until() return states:
     * 1. std::future_status::ready : The result is available.
     * 2. std::future_status::timeout : The result is not available yet.
     * 3. std::future_status::deferred : The result is not available yet and the task is deferred.
     */
    while(future.wait_for(std::chrono::milliseconds(100)) != std::future_status::ready) {
        std::cout << "Waiting for the result..." << std::endl;
    }

    auto res = future.get();
    std::cout << "Result: " << res << std::endl;

    future = std::async(get_gcd, -20, 30);
    try {
        auto result = future.get();
        std::cout << "Result: " << result << std::endl;
    } catch (std::exception& e) {
        std::cout << "Error : " << e.what() << std::endl;
    }

    /**
     * Async launch policies:
     * 1. std::launch::async : The task is executed asynchronously.
     * 2. std::launch::deferred : The task is executed synchronously when get() or wait()
     * is called on the future object. This is lazy evaluation.
     * 3. std::launch::async | std::launch::deferred : The implementation chooses whether to perform
     * asynchronous execution or lazy evaluation. (default launch policy)
     */
    future = std::async(std::launch::deferred, get_gcd, 50, 75);

    std::cout << "calling get() on deferred future..." << std::endl;
    auto result = future.get();
    std::cout << "Result: " << result << std::endl;

    return EXIT_SUCCESS;
}