#include <thread>
#include "thread_runner.h"

int main() {

    {
        /**
         * Native handle is a unique identifier for the thread. It is a system specific identifier.
         * It is not portable, and is set to null after the thread is destroyed. 
         * So a null handle means that the thread is not running.
         */
        ThreadRunner tr;
        std::thread thr(tr);
        std::cout << "Thread native handle (Before): " << thr.native_handle() << std::endl;
        thr.join();
        std::cout << "Thread native handle (After): " << thr.native_handle() << std::endl;
    }

    {
        /**
         * std::thread::get_id returns a unique identifier for the thread. It is a system specific identifier.
         * Note that once a thread dies, the id can be reused by another thread.
         */
        std::cout << "Main thread id: " << std::this_thread::get_id() << std::endl;
        ThreadRunner tr;
        std::thread thr(&ThreadRunner::classMethodEntryPoint, &tr);
        std::thread thr2(tr);
        std::cout << "thr id: (Before) " << thr.get_id() << std::endl;
        std::cout << "thr2 id: (Before) " << thr2.get_id() << std::endl;
        thr.join();
        thr2.join();
        std::cout << "thr id: (After) " << thr.get_id() << std::endl;
        std::cout << "thr2 id: (After) " << thr2.get_id() << std::endl;
    }

    {

        /**
         * std::chrono is a library that provides time related functions.
         * std::literals is a namespace that we can use to craete literals for time related functions,
         * eg 2s == 2 seconds.
         */
        ThreadRunner thr;
        std::thread t1(&ThreadRunner::sleepThreadEntryPoint, std::ref(thr));
        std::thread t2(thr);
        t1.join();
        t2.join();
    }
    return EXIT_SUCCESS;
}
