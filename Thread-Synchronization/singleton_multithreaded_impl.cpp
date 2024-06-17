#include <thread>
#include <iostream>
#include <memory>
#include <mutex>

static const int THREADS = 10;

class Singleton {
    static std::unique_ptr<Singleton> instance;
    static std::mutex initialization_mutex;
public:
    Singleton() {
        std::cout << "Singleton instance created." << std::endl;
    }
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) = delete;

    static Singleton* get_instance() {
        std::unique_lock<std::mutex> lock(initialization_mutex);
        if(!instance) {
            instance.reset(new Singleton());
        }
        return instance.get();
    }
};

std::unique_ptr<Singleton> Singleton::instance = nullptr;
std::mutex Singleton::initialization_mutex;

void task() {
    auto instance = Singleton::get_instance();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

class SingletonWitoutMutex {
public:
    SingletonWitoutMutex() {
        std::cout << "Singleton {Without Mutex} instance created." << std::endl;
    }
    SingletonWitoutMutex(const SingletonWitoutMutex&) = delete;
    SingletonWitoutMutex(SingletonWitoutMutex&&) = delete;
    SingletonWitoutMutex& operator=(const SingletonWitoutMutex&) = delete;
    SingletonWitoutMutex& operator=(SingletonWitoutMutex&&) = delete;

    static SingletonWitoutMutex* get_instance() {
        /// Once a thread initializes the instance, the other threads will get the same instance.
        /**
         * Static keyword is thread-safe from C++11.
         */
        static SingletonWitoutMutex instance;
        return &instance;
    }
};

void task_without_mutex() {
    auto instance = SingletonWitoutMutex::get_instance();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

int main() {
    std::vector<std::thread> threads;
    for(int i = 0 ; i < THREADS ; i++) {
        threads.push_back(std::thread(task));
        threads.push_back(std::thread(task_without_mutex));
    }
    for(auto& thread: threads) {
        thread.join();
    }
    return EXIT_SUCCESS;
}