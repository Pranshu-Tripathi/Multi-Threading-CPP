#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>

static std::string data {"$"};
static bool notify_predicate = false;
std::mutex print_mutex;
std::mutex data_mutex;
std::condition_variable cv;

void print(const std::string& msg) {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::cout << msg << std::endl;
}


void read() {
    print("Reader lock acquired.");
    std::unique_lock<std::mutex> data_lock(data_mutex);
    print("Reader {} : Waiting.");
    cv.wait(data_lock, []() { return notify_predicate; });
    print("Reader {} : Active. Data ---> " + data);
}

void write() {
    print("Writer lock acquired.");
    {
        std::unique_lock<std::mutex> data_lock(data_mutex);
        print("Writer {} : Active.");
        data += "Writer Writes.";
        print("Writer {} : Data updated.");
        std::this_thread::sleep_for(std::chrono::seconds(2));
        notify_predicate = true;
    }
    cv.notify_one();
}

int main() {
    /**
     * With the use of predicate we no longer need
     * to depend on the order of execution of threads.
     * Lost Wake-up is avoided here.
     */
    std::thread writer(write);
    std::thread reader(read);

    reader.join();
    writer.join();
    return EXIT_SUCCESS;
}
