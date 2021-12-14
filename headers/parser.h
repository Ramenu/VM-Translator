#ifndef PARSER_H
#define PARSER_H

#include "codewriter.h"
#include <unordered_map>

class Parser
{
    public:
        void openFileOrDirectory(const char* path, bool debug);
    private:
        bool doDebug;
        void readVM(const char* filePath, bool contReading); //Read from the VM file
        void openDirectory(const char* path);
        std::string parseAddress(bool address);
        void parsePushOrPop();
        void parseAndOrAdd();
        void parseNotOrNeg();
        void parseLtOrLabel();
        void parseGtOrGoto();
        void executeCommand(std::string command);
        void findFunctionAddress(const char* filePath, std::string functionName);
        std::ifstream openAndReadFile(const char* filePath);
        std::string line; 
        std::string lineWithSpace;
        void findSegment();
        bool isPush;
        int lastNumPushed;
        std::string currentFunctionIn;
        bool isDirectory;
        std::unordered_map<std::string, std::string> staticMap;


};


#endif