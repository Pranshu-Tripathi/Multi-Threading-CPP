#include <thread>
#include <iostream>
#include <mutex>
#include <assert.h>

/**
 * try_lock() is a non-blocking call. It tries to lock the mutex and returns immediately. 
 * If the mutex is not available, it returns false. The lock() call will block the
 * thread until the mutex is available and locked by this thread.
 */

std::mutex mtx;
static int var = 0;

void task(const std::string& task_name, int critial_time, int sleep_time, int update_var) {
    std::cout << task_name << " called in " <<  std::this_thread::get_id() << std::endl;
    while(!mtx.try_lock()) {
        std::cout << task_name << " waiting in " <<  std::this_thread::get_id() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(sleep_time));
    }
    std::cout << task_name << " acquired lock in " <<  std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(critial_time));
    var += update_var;
    std::cout << task_name << " updated var in " <<  std::this_thread::get_id() << " to " << var << std::endl;
    std::cout << task_name << " released lock in " <<  std::this_thread::get_id() << std::endl;
    mtx.unlock();
}

int main() {
    std::thread t1(task, "Task1", 5, 1, -10);
    std::thread t2(task, "Task2", 5, 2, 20);
    std::thread t3(task, "Task3", 5, 3, 30);

    t1.join();
    t2.join();
    t3.join();
    assert(var == 40);
    return EXIT_SUCCESS;
}