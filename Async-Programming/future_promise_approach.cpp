#include <thread>
#include <iostream>
#include <future>

void addition_task(std::promise<int>&& prom, int a, int b) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    prom.set_value(a + b);
}

void addition_result(std::future<int>&& fut) {
    std::cout << "Waiting for the result..." << std::endl;
    auto result = fut.get();
    std::cout << "Result: " << result << std::endl;
}

int main() {
    std::promise<int> prom;
    auto fut = prom.get_future();
    std::thread promise_thr(addition_task, std::move(prom), 2, 4);
    std::thread future_thr(addition_result, std::move(fut));
    promise_thr.join();
    future_thr.join();
    return EXIT_SUCCESS;
}