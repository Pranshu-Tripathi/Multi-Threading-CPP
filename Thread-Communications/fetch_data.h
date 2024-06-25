#pragma once
#include <string>
#include <random>
#include <iostream>

class FetchData {
    std::string data {"$"};
    bool update_progress;
    bool is_data_fetched;
    
    std::mt19937 gen;

public:
    FetchData() : update_progress(false), is_data_fetched(false) {}
    ~FetchData() = default;
    void updateFetch() {
        std::uniform_int_distribution<int> dist(1, 100);
        std::string byte_buffer;
        for(int i = 0 ; i < 3 ; i++) {
            byte_buffer += std::to_string(dist(gen)) + "#";
        }
        std::cout << "Data fetched : " << byte_buffer << std::endl;
        data += byte_buffer;
        update_progress = true;
    }
    void notifyDownloadComplete() {
        is_data_fetched = true;
        update_progress = true;
    }

    void progressIncorporated() {
        update_progress = false;
    }

    bool isProgressMade() {
        return update_progress;
    }

    bool isDownloadCompleted() {
        return is_data_fetched;
    }
    std::string getData() const {
        return data;
    }
};