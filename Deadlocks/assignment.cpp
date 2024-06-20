#include <iostream>
#include <thread>
#include <mutex>

static int x{0};
static std::mutex m;
/**
 * @brief This function is a simple example of a livelock.
 * In liveloack, threads are not blocked but they are not making
 * any progress.
 */
void func() {
    std::lock_guard<std::mutex> lock(m);
    while (x == 0) {
        x = 1 - x;
    }
}

int main() {
    std::thread t1(func);
    std::thread t2(func);
    t1.join();
    t2.join();
    std::cout << "Threads are done executing\n"; // This line will never be printed
    return EXIT_SUCCESS;
}