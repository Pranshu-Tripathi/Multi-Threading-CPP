#include <iostream>
#include <thread>
#include <mutex>
#include <string>

static std::string data {"$"};

static std::mutex print_mutex;
static std::mutex data_mutex;
static std::condition_variable cv;

void print(const std::string& msg) {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::cout << msg << std::endl;
}

void read() 
{
    print("Reader lock acquired.");
    std::unique_lock<std::mutex> data_lock(data_mutex);
    print("Reader {} : Waiting.");
    cv.wait(data_lock);

    print("Reader {} : Active. Data ---> " + data);
}

void write()
{
    print("Writer lock acquired.");
    {
        std::unique_lock<std::mutex> data_lock(data_mutex);
        print("Writer {} : Active.");
        data += "Writer Writes.";
        print("Writer {} : Data updated.");
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    cv.notify_one();
}

int main()
{
    std::thread reader(read);
    std::thread writer(write);

    reader.join();
    writer.join();
    return EXIT_SUCCESS;
}