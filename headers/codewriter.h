#ifndef CODEWRITER_H
#define CODEWRITER_H

#include <fstream>
#include <ostream>
#include <iostream>


class CodeWriter
{
   public: 
    void writePush();
    void writePop();
    void writeArithmetic();
    void writeLogical();
    void infiniteLoop();
    std::string argument;
    const char* segment;
    int segmentAddress;
    std::string address;
    std::string prevAddress;
    int stack;
    int prevStack;
    static std::ofstream out;
    std::string outputFileName;
    std::ofstream openOutputFile(bool debug);
    bool referencePointer;
    int memoryToThis;
    int memoryToThat;
    int getSegmentAddress();
    void writeInit(); //Writes ASM instructions that effects bootstrap code, placed at beginning of generated .asm file
    void writeLabel(const char* label);
    void writeGoto(const char* address);
    void writeIfGoto(const char* address);
    void writeFunction(std::string functionName, unsigned int numVars);
    void writeCall(std::string functionName, unsigned int numArgs);
    void writeReturn();
    void outputReturnValues();
    unsigned int linesCounted;
    private:
     bool didCall;
     unsigned int boolCount;
     std::string boolLabel;
     std::string atBool;
     unsigned int returnCounter;
     std::string atFunc;
     unsigned int numOfVars;
     std::string currentReturn[1000];
   
};

#endif