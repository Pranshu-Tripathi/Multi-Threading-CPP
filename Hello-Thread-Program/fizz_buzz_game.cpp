#include <iostream>
#include <vector>
#include <string>
#include <thread>

void game(const std::vector<std::string>& players, int rounds) {
    std::cout << "\nFizzBuzz Game Started!" << std::endl;
    auto say = [](const std::string& name, int number) {
        std::string result;
        if(number % 15 == 0) {
            result += "FizzBuzz";
        }
        else if(number % 3 == 0) {
            result += "Fizz";
        }
        else if(number % 5 == 0) {
            result += "Buzz";
        }
        else {
            result += std::to_string(number);
        }
        std::cout << name << " says:" << result << std::endl;
    };

    for(int counter = 1 ; counter <= rounds; ++counter) {
        for(const auto& player: players) {
            say(player, counter);
        }
    }
    std::cout << "\nFizzBuzz Game Ended!" << std::endl;
}

int main() {
    std::cout << "\nWelcome to FizzBuzz Game!" << std::endl;
    static std::vector<std::string> players = {
        "Abdul",
        "Bart",
        "Claudia",
        "Divya",
    };
    std::thread fizz_buzz1(game, std::ref(players), 10);
    std::thread fizz_buzz2(game, std::ref(players), 20);
    fizz_buzz1.join();
    fizz_buzz2.join();
    std::cout << "\nThank you for playing FizzBuzz Game!" << std::endl;
    return EXIT_SUCCESS;
}