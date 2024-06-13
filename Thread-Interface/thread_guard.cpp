#include <thread>
#include <iostream>
#include "thread_runner.h"

class ThreadGuard {
    std::thread t;

    public:
    ThreadGuard(std::thread&& _t) : t(std::move(_t)) {};
    ~ThreadGuard() {
        if(t.joinable()) {
            t.join();
        }
    }
};

int main() {
    ThreadRunner tr;

    /**
     * In below code, we are throwing an exception from main thread.
     * When exception is thrown by a parent the desctructor of the objects
     * created in that scope will be called. In this case, since we could not
     * properly join the thread, the program will terminate.
     */
    // try {
    //     std::thread t1 (tr);
    //     throw std::runtime_error("Exception from main thread");
    //     t1.join();
    // } catch(std::runtime_error& e) {
    //     std::cout << "Exception caught: " << e.what() << std::endl;
    // }


    /**
     * To avoid the above issue, we can use a ThreadGuard class which will
     * join the thread in its destructor before std::terminate() is called.
     */
    try{
        ThreadGuard tg{std::thread(tr)};
        throw std::runtime_error("Exception from main thread");
    } catch(std::runtime_error& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
}