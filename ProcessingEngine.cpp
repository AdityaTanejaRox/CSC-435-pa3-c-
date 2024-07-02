#include "ProcessingEngine.hpp"
#include <thread>
#include <vector>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <iomanip>

namespace fs = std::filesystem;
ProcessingEngine::ProcessingEngine(std::shared_ptr<IndexStore> store, int numWorkerThreads) :
        store(store), numWorkerThreads(numWorkerThreads) {
    // TO-DO implement constructor
}

size_t ProcessingEngine::workerIndexing(const std::vector<std::string>& folders){
        std::unordered_map<std::string, std::unordered_map<std::string, int>> localIndex;
        for(const auto& folder : folders){
                for(const auto& entry : fs::directory_iterator(folder)){
                        if(entry.is_regular_file() && entry.path().extension() == ".txt"){
                                std::ifstream file(entry.path());
                                std::string word, docPath = entry.path().string();
                                while(file >> word){
                                        word.erase(std::remove_if(word.begin(), word.end(), [](char c){ return !std::isalnum(c); }), word.end());
                                        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
                                        if(!word.empty()){
                                                localIndex[word][docPath]++;
                                        }
                                }
                        }
                }
        }
        store->updateIndex(localIndex);
        return 0;
}
void ProcessingEngine::indexFiles(const std::string& datasetPath) {
    // TO-DO implement index files
    // TO-DO crawl dataset and find all folders
    // TO-DO create the worker threads
    // TO-DO each worker thread receives a subset of the folders and runs the following operations:
    // - For each folder crawl the folder and find all documents
    // - For each document extract and count the words, add them to a local hash table and update the index store
    // TO-DO the main thread needs to wait for all of the worker threads to finish executing and join them
        std::vector<std::string> folders;
        size_t dataSize = 0;
        for(const auto& entry : fs::directory_iterator(datasetPath)){
                if(entry.is_directory()){
                        folders.push_back(entry.path().string());
                }
        }
        std::vector<std::thread> workers;
        int foldersPerThread = folders.size() / numWorkerThreads;
        for(int i = 0; i < numWorkerThreads; ++i){
                auto start = folders.begin() + i * foldersPerThread;
                auto end = (i == numWorkerThreads - 1) ? folders.end() : start + foldersPerThread;
                //std::vector<std::string> threadFolders(folders.begin() + i * foldersPerThread, (i == numWorkerThreads - 1) ? folders.end() : folders.begi>                std::vector<std::string> threadFolders(start, end);
                workers.emplace_back(&ProcessingEngine::workerIndexing, this, threadFolders);
        }
        for(auto& worker : workers){
                worker.join();
        }
}
void ProcessingEngine::searchFiles(const std::string& query) {
    // TO-DO implement search files
        std::istringstream queryStream(query);
        std::string term;
        std::vector<std::unordered_map<std::string, int>> results;
        while(queryStream >> term){
                term.erase(std::remove_if(term.begin(), term.end(), [](char c) { return !std::isalnum(c); }), term.end());
                std::transform(term.begin(), term.end(), term.begin(), ::tolower);
                if(!term.empty()){
                        auto result = store->lookupIndex(term);
                        results.push_back(std::move(result));
                }
        }
        std::unordered_map<std::string, int> combinedResults;
        for(const auto& result : results){
                for(const auto& entry : result){
                        combinedResults[entry.first] += entry.second;
                }
        }
        std::vector<std::pair<std::string, int>> sortedResults(combinedResults.begin(), combinedResults.end());
        std::sort(sortedResults.begin(), sortedResults.end(), [](const auto& a, const auto& b){
                return b.second < a.second;
        });
        int count = 0;
        std::cout<<"Search results (top 10):" << std::endl;
        for(const auto& result : sortedResults){
                if(count++ >= 10) break;
                std::cout<<"* " << result.first<<" " <<result.second<<std::endl;
        }
}

int ProcessingEngine::getNumWorkerThread() const{
        return numWorkerThreads;
}
