/**
 * This file is a simulation of a download process.
 * It is a mix of three operations
 * 1. Downloading data.
 * 2. Showing progress.
 * 3. processing data.
 */
#include "fetch_data.h"
#include <thread>
#include <mutex>


static std::mutex print_mutex;
static std::mutex completion_mutex;
static std::mutex data_mutex;

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
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    print("Data fetched successfully.");
    std::unique_lock<std::mutex> completion_lock(completion_mutex);
    data.notifyDownloadComplete();
}

void show_progress() {
    int buf_len = 0;

    while(true) {
        std::unique_lock<std::mutex> progress_lock(data_mutex);
        while(!data.isProgressMade()) {
            print("Progress bar waiting for data...");
            progress_lock.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            progress_lock.lock();
        }
        std::unique_lock<std::mutex> completion_lock(completion_mutex);
        if (data.isDownloadCompleted()) {
            break;
        }
        auto len = data.getData().size();
        data.progressIncorporated();
        progress_lock.unlock();
        print("Received data : " + std::to_string(len - buf_len));
        buf_len = len;
        print("Progress : " + std::to_string(buf_len));

    }
}

void process_download() {
    while(true) {
        std::unique_lock<std::mutex> completion_lock(completion_mutex);
        while(!data.isDownloadCompleted()) {
            print("Waiting for data to process...");
            completion_lock.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            completion_lock.lock();
        }
        completion_lock.unlock();
        print("Processing data...");
        std::unique_lock<std::mutex> data_lock(data_mutex);
        auto data_str = data.getData();
        data_lock.unlock();

        std::this_thread::sleep_for(std::chrono::seconds(1));
        print("Fetched Data = " + data_str);
        break;
    }
}

int main() {
    /**
     * This code works but is not ideal.
     * We have hard coded the wait times for each operation.
     * This may cause hot loops and unncessary utilization of CPU.
     * Hot loops are the checks that occur repeatedly but do not yield any result.
     * Better solution would be that the threads are notified when the data is ready.
     */
    std::thread fetcher_job(download_data);
    std::thread progress_job(show_progress);
    std::thread process_job(process_download);

    fetcher_job.join();
    progress_job.join();
    process_job.join();
    return EXIT_SUCCESS;
}

