#include <thread>
#include <iostream>

void execptionThread() {
    std::ostringstream oss;
    oss << "Exception from thread " << std::this_thread::get_id();
    /**
     * We can have a try catch block here.
     */
    try{
        throw std::runtime_error(oss.str());
    } catch (std::runtime_error& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
}

int main(){
    /**
     * If an exception is thrown from a thread, the program will not be
     * able to catch it in a try-catch block. The program will terminate.
     * error for below code:
     * terminating due to uncaught exception of type std::runtime_error: Exception from thread 0x16b1d7000
     */
    // try {
    //     std::thread thr(execptionThread);
    //     thr.join();
    // }catch (std::runtime_error& e) {
    //     std::cout << "Exception caught: " << e.what() << std::endl;
    // }

    std::thread thr(execptionThread);
    thr.join();   
}