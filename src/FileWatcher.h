#pragma once

#include <filesystem>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <string>
#include <functional>

using namespace std;
enum class FileStatus {created, modified, erased};


class FileWatcher {
public:
    string path_to_watch;
    // Time interval at which we check the base folder for changes
    chrono::duration<int, std::milli> delay;

    // Keep a record of files from the base directory and their last modification time
    FileWatcher(string path_to_watch, chrono::duration<int, milli> delay) : path_to_watch{path_to_watch}, delay{delay} {
        for(auto &file : filesystem::recursive_directory_iterator(path_to_watch)) {
            paths_[file.path().string()] = filesystem::last_write_time(file);
        }
    }

    void start(const function<void (string, FileStatus)> &action) {
         while(running_) {
             // Wait for "delay" milliseconds
             this_thread::sleep_for(delay);
 
             auto it = paths_.begin();
             while (it != paths_.end()) {
                 if (!filesystem::exists(it->first)) {
                     action(it->first, FileStatus::erased);
                     it = paths_.erase(it);
                 }
                 else {
                     it++;
                 }
             }
 
             // Check if a file was created or modified
             for(auto &file : std::filesystem::recursive_directory_iterator(path_to_watch)) {
                 auto current_file_last_write_time = std::filesystem::last_write_time(file);
 
                 // File creation
                 if(!contains(file.path().string())) {
                     paths_[file.path().string()] = current_file_last_write_time;
                     action(file.path().string(), FileStatus::created);
                 // File modification
                 } else {
                     if(paths_[file.path().string()] != current_file_last_write_time) {
                         paths_[file.path().string()] = current_file_last_write_time;
                         action(file.path().string(), FileStatus::modified);
                     }
                 }
             }
         }
     }

private:
    unordered_map<std::string, filesystem::file_time_type> paths_;
    bool running_ = true;
    
    // Check if "paths_" contains a given key
    bool contains(const string &key) {
        auto el = paths_.find(key);
        return el != paths_.end();
    }

};