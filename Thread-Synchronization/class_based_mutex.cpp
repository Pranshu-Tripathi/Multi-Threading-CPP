#include <thread>
#include <iostream>
#include <mutex>
#include <vector>

class Vector {
    std::vector<int> container;
    // std::mutex mtx;
public:
    Vector() = default;
    ~Vector() = default;
    void add(int value) {
        // mtx.lock();
        container.push_back(value);
        // mtx.unlock();
    }

    void get_elements() {
        // mtx.lock();
        std::cout << "Called by " << std::this_thread::get_id() << ": ";
        for(const auto& ele: container) {
            std::cout << ele << ' ';
        }
        std::cout << std::endl;
        // mtx.unlock();
    }
};

void func(Vector& vec) {
    for(int i = 0 ; i < 100 ; i++) {
        vec.add(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        vec.get_elements();
    }
}

int main() {
    Vector vec;
    std::thread t1(func, std::ref(vec));
    std::thread t2(func, std::ref(vec));
    t1.join();
    t2.join();
    return EXIT_SUCCESS;
}