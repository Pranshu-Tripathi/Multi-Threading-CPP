#include <thread>
#include "thread_runner.h"

void captureThread(std::thread&& thr){
    /**
     * Now the function owns the thread object and so is responsible for joining it.
     * This is because the thread object is passed as an rvalue reference.
     */
    std::cout << "Thread received with id: " << thr.get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    thr.join();
}

int main() {
    ThreadRunner tr;
    std::thread t1(&ThreadRunner::classMethodEntryPoint, &tr);
    std::thread t2(tr);
    std::cout << "t1 id: " << t1.get_id() << std::endl;
    std::cout << "t2 id: " << t2.get_id() << std::endl;
    captureThread(std::move(t1)); /** passed as a rvalue */
    captureThread(std::move(t2)); /** passed as a rvalue */
    // t1.join(); // This will cause a crash as the thread object is moved to captureThread function.
    return EXIT_SUCCESS;
}