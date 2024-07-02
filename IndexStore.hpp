#ifndef INDEX_STORE_H
#define INDEX_STORE_H

#include <unordered_map>
#include <vector>
#include <string>
#include <mutex>
class IndexStore {
    // TO-DO use a search structure to keep track of the index
    private:
        std::unordered_map<std::string, std::unordered_map<std::string, int>> index;
        std::mutex indexMutex;
    public:
        // constructor
        IndexStore();

        // default virtual destructor
        virtual ~IndexStore() = default;

        // TO-DO re-declare index update and index lookup methods
        void updateIndex(const std::unordered_map<std::string, std::unordered_map<std::string, int>>& newIndex);
        //std::vector<std::pair<std::string, int>> lookupIndex(const std::string& term);
        std::unordered_map<std::string, int> lookupIndex(const std::string& term);
};

#endif