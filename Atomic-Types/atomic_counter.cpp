#include <iostream>
#include <thread>
#include <atomic>

/**
 * Now the counter value is not pre-fetched by different thread cores
 * and the value is updated atomically, by flushing the store buffer.
 * 
 * This should only be used when datatype is trivially copyable and
 * the operation is simple. It is faster than lock-based synchronization.
 */
std::atomic<int> counter{0};

void increment_counter() {
    for(int i = 0 ; i < 1000 ; i++) {
        counter++;
    }
}

int main() {
    std::vector<std::thread> threads;
    for(int i = 0 ; i < 10 ; i++) {
        threads.push_back(std::thread(increment_counter));
    }
    for(auto& thread: threads) {
        thread.join();
    }
    std::cout << "Counter value: " << counter << std::endl;
    return EXIT_SUCCESS;
}