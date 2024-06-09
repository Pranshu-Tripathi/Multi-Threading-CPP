#include <thread>
#include <iostream>

class Hello {
    public:
        void operator()() {
            std::cout << "HI! I was the entry point of this thread." << std::endl; 
        }
        Hello() {
            std::cout << "Hello constructor." << std::endl;
        }
        ~Hello() {
            std::cout << "Hello destructor." << std::endl;
        }
};

int main() {
    Hello hello;
    std::thread thr(std::ref(hello)); // if ref is not passed then thread will make a copy of hello object.
                                      // causing the destructor to be called twice. Copy constructor is called.
    thr.join();

    std::thread lambda_thr(
        [](){
            std::cout << "HI! I am lambda thread entry point." << std::endl;
        }
    );
    lambda_thr.join();
    return 0;
}