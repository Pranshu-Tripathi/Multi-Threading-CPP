#include <thread>
#include <iostream>
#include <mutex>

static int var = 0;
static int counter = 100000;
std::mutex counter_lock;

void func(int _counter) {
    for(int i = 0 ; i < _counter; i++){
        counter_lock.lock();
        var++;
        counter_lock.unlock();
    }
}

int main() {
    std::thread t1(func, counter); // var = 100000
    std::thread t2(func, counter); // var = 200000
    t1.join();
    t2.join();
    /**
     * Now since we used a mutex to synchronize the access to the shared variable var,
     * the value of var will be 200000 as expected.
     */
    std::cout << "Value of var: " << var << std::endl;
    return EXIT_SUCCESS;
}