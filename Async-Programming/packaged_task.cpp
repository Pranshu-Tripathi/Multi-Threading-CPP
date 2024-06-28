#include <iostream>
#include <thread>
#include <future>


int main() {
    std::packaged_task<int(int, int)> task([](int a, int b) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        return a + b;
    });

    auto future = task.get_future();

    task(2, 5); // this will run synchronously, i.e main thread will wait for this to finish

    std::cout << "Waiting for the result..." << std::endl;
    auto result = future.get();

    std::cout << "Result: " << result << std::endl;

    std::packaged_task<int(int, int)>task2([](int a, int b) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        return a + b;
    });

    auto future2 = task2.get_future(); // get future before moving the task to thread
    std::thread thr(std::move(task2), 3 , 4);
    thr.detach(); // this will run asynchronously
    std::cout << "Waiting for the result2..." << std::endl;
    auto result2 = future2.get();
    std::cout << "Result2: " << result2 << std::endl;

    return EXIT_SUCCESS;
}