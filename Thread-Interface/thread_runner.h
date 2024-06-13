#pragma once
#include <iostream>

class ThreadRunner{
    public:
        void operator()() {
            std::cout << "ThreadRunner operator() was the entry point." << std::endl;
        }
        void classMethodEntryPoint() {
            std::cout << "ThreadRunner classMethodEntryPoint was the entry point." << std::endl;
        }
        void sleepThreadEntryPoint() {
            using namespace std::literals;
            std::cout << "ThreadRunner sleepThreadEntryPoint was the entry point." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(5));
            std::cout << "ThreadRunner sleepThreadEntryPoint woke up after 5 seconds" << std::endl;
            std::this_thread::sleep_for(2s);
            std::cout << "ThreadRunner sleepThreadEntryPoint woke up after 2 seconds." << std::endl;
        }
        ThreadRunner() {};
        ~ThreadRunner() = default;
};