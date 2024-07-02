#include "AppInterface.hpp"

#include <iostream>
#include <string>
#include <chrono>

AppInterface::AppInterface(std::shared_ptr<ProcessingEngine> engine) : engine(engine) {
    // TO-DO implement constructor
}

void AppInterface::readCommands() {
    // TO-DO implement the read commands method
    std::string command;

    while (true) {
        std::cout << "> ";

        // read from command line
        std::cin >> command;

        // if the command is quit, terminate the program
        if (command == "quit") {
            break;
        }

        // if the command begins with index, index the files from the specified directory
        if (command.size() >= 5 && command.substr(0, 5) == "index") {
            // TO-DO parse command and call index operation
            std::string path = command.substr(5);
                auto start = std::chrono::high_resolution_clock::now();
                engine->indexFiles(path);
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> elapsed = end - start;
                //std::cout<<"Completed indexing using " << engine->getNumWorkerThreads() << " worker threads" <<std::endl;
                std::cout<<"Completed indexing in " << elapsed.count() << " seconds" << std::endl;
            continue;
        }

        // if the command begins with search, search for files that matches the query
        if (command.size() >= 6 && command.substr(0, 6) == "search") {
            // TO-DO parse command and call search operation
            std::string query = command.substr(6);
                auto start = std::chrono::high_resolution_clock::now();
                engine->searchFiles(query);
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> elapsed = end - start;
                std::cout<<"Search completed in  " << elapsed.count() << " seconds" << std::endl;
                continue;
        }

        std::cout << "unrecognized command!" << std::endl;
    }
}