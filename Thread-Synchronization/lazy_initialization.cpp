#include <thread>
#include <iostream>
#include <mutex>


class Singleton {
    static std::unique_ptr<Singleton> instance;
    static std::mutex initialization_mutex;
    static bool initialized;
    static std::once_flag one_flag;
public:
    Singleton() {
        std::cout << "Singleton instance created." << std::endl;
    }
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) = delete;

    /**
     * Double Checker Approach -> once initialized,
     * we don't want the threads to synchronize anymore,
     * they are free to concurrently, use this instance.
     * 
     * We double check to make sure two threads have not concurrently
     * started the initialization process.
     * 
     * Still this was not enough until C++17, cause if the initialization is
     * time taking then, the other threads may use the object 
     * that is not completely constructed. So in this case one
     * should use std::call_once. It makes sure to unlock the
     * thread only when complete initialization is performed.
     * 
     * In C++17 the order of initialization of static variables
     * is 1. Allocate memory 2. Call constructor 3. Assign to variable. Thus double check is safe.
     */
    static Singleton* get_instance_double_checker() {
        if(!initialized) {
            std::unique_lock<std::mutex> lock(initialization_mutex);
            if(!initialized){
                instance.reset(new Singleton());
            }
        }
    }

    static void initSingleton() {
        instance.reset(new Singleton());
    }

    static Singleton* get_instance_call_once() {
        std::call_once(one_flag, &Singleton::initSingleton);
        return instance.get();
    }
};

void task() {
    auto instance = Singleton::get_instance_call_once();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

std::unique_ptr<Singleton> Singleton::instance = nullptr;
std::once_flag Singleton::one_flag;

int main() {
    std::thread t1(task);
    std::thread t2(task);
    t1.join();
    t2.join();
    return EXIT_SUCCESS;
}