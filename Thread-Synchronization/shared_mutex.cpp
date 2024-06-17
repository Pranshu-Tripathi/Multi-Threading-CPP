#include <thread>
#include <iostream>
#include <mutex>
#include <shared_mutex>

constexpr static int THREADS = 500;

class Timer {
    std::chrono::time_point<std::chrono::steady_clock> start, end;
    public:
        Timer() {
            start = std::chrono::steady_clock::now();
        }
        ~Timer() {
            end = std::chrono::steady_clock::now();
            std::cout << "Time elapsed: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;
        }
};

class Computations {
    std::string name;
    public:
        Computations(const std::string& _name): name(_name) {}
        void read() {
            /**
             * Simulating some computation on read that might take some time.
             */
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        void write(const std::string& data) {
            name = data;
        }
};

void execute_reader_writer_problem() {
    Timer timer;
    std::mutex mtx;
    Computations computations("Hello World");
    std::vector<std::thread> threads;
    for(int i = 0 ; i <= THREADS ; i++) {
        if(i % 5 == 0 && i) {
            threads.push_back(std::thread([&computations, &mtx, i](){
                std::unique_lock<std::mutex> lock(mtx);
                computations.write("Hello World {" + std::to_string(i) + "}");
            }));
        } else {
            threads.push_back(std::thread([&computations, &mtx](){
                std::unique_lock<std::mutex> lock(mtx);
                computations.read();
            }));
        }
    }

    for(auto& thread: threads) {
        thread.join();
    }
}

void execute_reader_writer_optimized() {
    Timer timer;
    std::shared_mutex mtx;
    Computations computations("Hello World");
    std::vector<std::thread> threads;
    for(int i = 0 ; i <= THREADS ; i++) {
        if(i % 5 == 0 && i) {
            threads.push_back(std::thread([&computations, &mtx, i](){
                std::unique_lock<std::shared_mutex> lock(mtx);
                computations.write("Hello World {" + std::to_string(i) + "}");
            }));
        } else {
            threads.push_back(std::thread([&computations, &mtx](){
                std::shared_lock<std::shared_mutex> lock(mtx);
                computations.read();
            }));
        }
    }
    for(auto& thread: threads) {
        thread.join();
    }
}

int main() {
    std::cout << "Problem Time => ";
    execute_reader_writer_problem();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Optimized Time => ";
    execute_reader_writer_optimized();
    return EXIT_SUCCESS;
}