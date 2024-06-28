#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>

class Singleton {
    static std::atomic<Singleton*> instance;
    static std::mutex init_mutex;
    public:
        static Singleton* getInstance() {
            auto ptr = instance.load(std::memory_order_acquire);
            if(ptr == nullptr) {
                std::lock_guard<std::mutex> lock(init_mutex);
                ptr = instance.load(std::memory_order_acquire);
                if (ptr == nullptr) {
                    ptr = new Singleton();
                    instance.store(ptr, std::memory_order_release);
                }
            }
            return ptr;
        }

        Singleton() {
            std::cout << "Singleton instance created" << std::endl;
        }
        ~Singleton() = default;
};

void task() {
    [[maybe_unused]] auto instance = Singleton::getInstance();
}

std::atomic<Singleton*> Singleton::instance{nullptr};
std::mutex Singleton::init_mutex;

int main() {
    std::vector<std::thread> thrs;
    for(int i = 0 ; i < 10 ; i++) {
        thrs.push_back(std::thread(task));
    }
    for(auto& thr: thrs) {
        thr.join();
    }
    return EXIT_SUCCESS;
}