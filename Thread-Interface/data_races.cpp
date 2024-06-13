#include <thread>
#include <iostream>

static int var = 0;
static int counter = 100000;
void func(int _counter) {
    for(int i = 0 ; i < _counter; i++){
        var++;
    }
}

int main() {
    std::thread t1(func, counter); // var = 100000
    std::thread t2(func, counter); // var = 200000
    t1.join();
    t2.join();
    /**
     * On running this code, we will see that the value of var is not 200000.
     * This is because the threads are trying to access the same memory location
     * and are not properly synchronized. This is called a data race. 
     * Example: var = 200, t1 reads it. t2 interupts and reads it.
     * t1 increments it -> 201.
     * t2 increments it -> 201. 
     * So overalapped incrementation is happening.
     */
    std::cout << "Value of var: " << var << std::endl;
    return EXIT_SUCCESS;
}