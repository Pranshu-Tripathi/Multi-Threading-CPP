#include <iostream>
#include <thread>
#include <mutex>
#include <string>

static std::string data {"$"};
static bool is_data_ready = false;

static std::mutex print_mutex;
static std::mutex data_mutex;
static std::mutex flag_mutex;

void print(const std::string& msg) {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::cout << msg << std::endl;
}

void read()
{
    print("Reader waiting for flag");
    std::unique_lock<std::mutex> flag_lock(flag_mutex);
    while(!is_data_ready) {
        print("Reader : Waiting for data.");
        flag_lock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        flag_lock.lock();
    }

    flag_lock.unlock();
    print("Reader : Data ready. Reading data.");
    std::unique_lock<std::mutex> data_lock(data_mutex);
    print("Reader : Data ---> " + data);
}

void write()
{
    print("Writer : Writing data.");
    std::unique_lock<std::mutex> data_lock(data_mutex);
    data += "I am data";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::unique_lock<std::mutex> flag_lock(flag_mutex);
    is_data_ready = true;
    print("Writer : Data written.");
}

int main() {
    std::thread reader(read);
    std::thread writer(write);

    reader.join();
    writer.join();
    return EXIT_SUCCESS;
}