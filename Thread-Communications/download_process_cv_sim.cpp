/**
 * Here we simulate the download process using condition variables.
 */

#include "fetch_data.h"
#include <thread>
#include <mutex>
#include <condition_variable>

static std::mutex print_mutex;
static std::mutex data_mutex;

static std::condition_variable progress_cv;
static std::condition_variable completion_cv;

void print(const std::string& message) {
    std::unique_lock<std::mutex> print_lock(print_mutex);
    std::cout << message << std::endl;
}

static FetchData data;

void download_data() {
    for(int i = 0 ; i < 20 ; i++) {
        print("Fetching data... >>>> ");
        std::unique_lock<std::mutex> data_lock(data_mutex);
        data.updateFetch();
        data_lock.unlock();
        progress_cv.notify_one();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    print("Data fetched successfully.");
    std::unique_lock<std::mutex> completion_lock(data_mutex);
    data.notifyDownloadComplete();
    completion_lock.unlock();
    completion_cv.notify_all();
}

void show_progress() {
    int buf_len = 0;
    while(true) {
        print("Progress bar waiting for data...");
        std::unique_lock<std::mutex> progress_lock(data_mutex);
        progress_cv.wait(progress_lock, [](){ return data.isProgressMade();});

        auto len = data.getData().size();
        progress_lock.unlock();
        print("Received data : " + std::to_string(len - buf_len));
        buf_len = len;
        print("Progress : " + std::to_string(buf_len));
        
        std::unique_lock<std::mutex> completion_lock(data_mutex);
        completion_cv.wait_for(completion_lock, std::chrono::milliseconds(500), [](){ return data.isDownloadCompleted();});
        if (data.isDownloadCompleted()){
            break;
        }
    }
}

void process_download() {
    std::unique_lock<std::mutex> completion_lock(data_mutex);
    completion_cv.wait(completion_lock, [](){ return data.isDownloadCompleted();});
    print("Processing download...");
    print("Data : " + data.getData());
}

int main() {
    std::thread progress_bar(show_progress);
    std::thread processor(process_download);
    std::thread downloader(download_data);

    downloader.join();
    progress_bar.join();
    processor.join();
    return EXIT_SUCCESS;
}