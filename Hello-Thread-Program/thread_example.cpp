#include <thread>
#include <iostream>

void hello() {
    std::cout << "HI! I am a thread." << std::endl;
}

int main() {
    std::thread thr(hello);
    
    // Wait for the thread to finish
    /**
     * If we don't wait for the thread to finish, the program will terminate.
     * C++ will call std::terminate() if the main thread finishes before the other threads.
     * This is because the main thread is the one that calls std::terminate(). It is a neucler bomb on
     * all the children threads to avoid the 'zombie' threads. So, we need to wait for the thread to finish.
     * Join - basically obstruct the main thread until the thread is finished.
     */
    thr.join();
}