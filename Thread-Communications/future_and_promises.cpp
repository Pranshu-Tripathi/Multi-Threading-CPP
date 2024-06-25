#include <thread>
#include <iostream>
#include <future>

void produce(std::promise<int>&& prom) {
    std::cout << "Producer computing the value" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    prom.set_value(20);
}

void comsume(std::future<int>&& fut) {
    std::cout << "Consumer waiting for the value" << std::endl;
    int value = fut.get();
    std::cout << "Consumer received the value : " << value << std::endl;
}

void produce_with_exception(std::promise<int>&& prom) {
    std::cout << "Producer computing the value" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    prom.set_exception(std::make_exception_ptr(std::runtime_error("Producer: Something went wrong.")));
}

void consume_with_exception(std::future<int>&& fut) {
    std::cout << "Consumer waiting for the value" << std::endl;
    try{
        auto value = fut.get();
        std::cout << "Consumer received the value : " << value << std::endl;
    } catch(std::exception& e) {
        std::cout << "Consumer Error : " << e.what() << std::endl;
    }
}

void shared_produce(std::promise<int>&& prom) {
    std::cout << std::this_thread::get_id() << " {Producer} : Computing the value" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    prom.set_value(20);

}

void shared_consume(std::shared_future<int>&& fut) {
    std::cout << std::this_thread::get_id() << " {Consumer} : Waiting for the value" << std::endl;
    try{
        int value = fut.get();
        std::cout << std::this_thread::get_id() << " {Consumer} : Received the value : " << value << std::endl;
    } catch (std::exception& e) {
        std::cout << std::this_thread::get_id() << " {Consumer} : Error : " << e.what() << std::endl;
    }
}

int main() {
    {
        auto prod = std::promise<int>();
        auto cons = prod.get_future();
        std::thread consumer(comsume, std::move(cons));
        std::thread producer(produce, std::move(prod));
        consumer.join();
        producer.join();
    }
    { 
        auto prod_ex = std::promise<int>();
        auto cons_ex = prod_ex.get_future();
        std::thread consumer_ex(consume_with_exception, std::move(cons_ex));
        std::thread producer_ex(produce_with_exception, std::move(prod_ex));
        consumer_ex.join();
        producer_ex.join();
    }
    {
        auto prod = std::promise<int>();
        std::vector<std::shared_future<int>> consumers;
        auto fut = prod.get_future().share();
        for(int i = 0 ; i < 5 ; i++) {
            auto copy_fut = fut;
            consumers.push_back(copy_fut);
        }
        std::vector<std::thread> consumer_threads;
        for(int i = 0 ; i < 5 ; i++) {
            consumer_threads.push_back(std::thread(shared_consume, std::move(consumers[i])));
        }
        std::thread producer(shared_produce, std::move(prod));
        producer.join();
        for(auto& consumer : consumer_threads) {
            consumer.join();
        }
    }


    return EXIT_SUCCESS;
}