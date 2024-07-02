#include "IndexStore.hpp"
#include <string>
#include <mutex>
IndexStore::IndexStore() {
    // TO-DO implement constructor
}

void IndexStore::updateIndex(const std::unordered_map<std::string, std::unordered_map<std::string, int>>& localIndex) {
    // TO-DO implement index insert
    // TO-DO you need to protect the global hash table from concurrent access
        std::lock_guard<std::mutex> lock(indexMutex);
        for(const auto& entry : localIndex){
                const std::string& term = entry.first;
                const std::unordered_map<std::string, int>& docMap = entry.second;
                for(const auto& docEntry : docMap){
                        const std::string& doc = docEntry.first;
                        int count = docEntry.second;
                        index[term][doc] += count;
                }
        }
}

std::unordered_map<std::string, int> IndexStore::lookupIndex(const std::string& term) {
    // TO-DO implement index lookup
        std::lock_guard<std::mutex> lock(indexMutex);
        auto it = index.find(term);
        if(it != index.end()){
                return it->second;
        }
        //if(index.find(term) != index.end()){
        //      return index[term];
        //}
        return index[term];
}