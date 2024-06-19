#include <thread>
#include <mutex>
#include <iostream>
#include <array>

constexpr int num_philosophers = 5;
std::array<std::mutex, num_philosophers> forks;
std::array<std::timed_mutex, num_philosophers> timed_forks;
std::mutex cout_mutex;

void print(const std::string& msg){
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << msg << std::endl;
}

void dine(int pos) {
    // think
    print("Philosopher " + std::to_string(pos) + " is thinking");
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    // pick left fork
    print("Philosopher " + std::to_string(pos) + " is waiting for left fork");
    std::unique_lock<std::mutex> left(forks[pos]);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    // pick right fork
    print("Philosopher " + std::to_string(pos) + " is waiting for right fork");
    std::unique_lock<std::mutex> right(forks[(pos + 1) % num_philosophers]);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    // eat
    print("Philosopher " + std::to_string(pos) + " is eating");
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

void dine_with_scope_lock(int pos) {
    // think
    print("Philosopher " + std::to_string(pos) + " is thinking");
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    // pick up both the forks. So lock two mutexes at once
    // this does change the problem statement a bit.
    print("Philosopher " + std::to_string(pos) + " is waiting for both forks");
    std::scoped_lock forks_lock(forks[pos], forks[(pos + 1)%num_philosophers]);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    // eat
    print("Philosopher " + std::to_string(pos) + " is eating");
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

void dine_hierachy_lock(int pos) {
    // this is a trick soluition to avoid deadlock.
    // pick up the fork with lower index first.
    // this way the last guy with 0 and n-1 fork will try to access the resources in the
    // opposite order. So deadlock is avoided.

    // think
    print("Philosopher " + std::to_string(pos) + " is thinking");
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    if(pos > (pos + 1)%num_philosophers) {
        print("Philosopher " + std::to_string(pos) + " is waiting for right fork");
        std::unique_lock<std::mutex> right(forks[(pos + 1) % num_philosophers]);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        print("Philosopher " + std::to_string(pos) + " is waiting for left fork");
        std::unique_lock<std::mutex> left(forks[pos]);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    } else  {
        print("Philosopher " + std::to_string(pos) + " is waiting for left fork");
        std::unique_lock<std::mutex> left(forks[pos]);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        print("Philosopher " + std::to_string(pos) + " is waiting for right fork");
        std::unique_lock<std::mutex> right(forks[(pos + 1) % num_philosophers]);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    // eat
    print("Philosopher " + std::to_string(pos) + " is eating");
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

void livelock_dine(int pos) {
    /**
     * This is a livelock prone solution.
     * Here the philosophers will pick up left immediately.
     * Then try to pick up right. If right is not available,
     * they will release left and try again.
     */

    // think
    print("Philosopher " + std::to_string(pos) + " is thinking");
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    
    // pick left fork
    print("Philosopher " + std::to_string(pos) + " is waiting for left fork");
    std::unique_lock<std::timed_mutex> left(timed_forks[pos]);
    std::unique_lock<std::timed_mutex> right(timed_forks[(pos + 1) % num_philosophers], std::defer_lock);
    while(!right.try_lock()){
        if(!right.try_lock_for(std::chrono::milliseconds(50))) {
            print("Philosopher " + std::to_string(pos) + " is releasing left fork");
            left.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            left.lock();
        }
    }

    // eat
    print("Philosopher " + std::to_string(pos) + " is eating");
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

int main() {
    std::array<std::thread, num_philosophers> philosophers;
    for(int i = 0 ; i < num_philosophers ; i++) {
        // philosophers[i] = std::thread(dine, i); this is deadlock prone.
        // philosophers[i] = std::thread(dine_with_scope_lock, i);
        // philosophers[i] = std::thread(dine_hierachy_lock, i);
        philosophers[i] = std::thread(livelock_dine, i); // this is livelock prone.
    }
    for(int i = 0 ; i < num_philosophers ; i++) {
        philosophers[i].join();
    }
}