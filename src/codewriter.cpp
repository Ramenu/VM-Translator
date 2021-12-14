#include "codewriter.h"

std::ofstream CodeWriter::out;
//Opens the output file, and depending on the filename will open the file to that path, if no filename matches the options given below..
//it will open the file in the current working directory
std::ofstream CodeWriter::openOutputFile(bool debug)
{
    std::string absolutePath {};
    if (!debug)
        if (outputFileName == "SimpleAdd")
            absolutePath = "C:/Users/ramen/Desktop/nand2tetris/nand2tetris/projects/07/StackArithmetic/SimpleAdd/";
        else if (outputFileName == "StackTest")
            absolutePath = "C:/Users/ramen/Desktop/nand2tetris/nand2tetris/projects/07/StackArithmetic/StackTest/";
        else if (outputFileName == "BasicTest")
            absolutePath = "C:/Users/ramen/Desktop/nand2tetris/nand2tetris/projects/07/MemoryAccess/BasicTest/";
        else if (outputFileName == "StaticTest")
            absolutePath = "C:/Users/ramen/Desktop/nand2tetris/nand2tetris/projects/07/MemoryAccess/StaticTest/";
        else if (outputFileName == "PointerTest")
            absolutePath = "C:/Users/ramen/Desktop/nand2tetris/nand2tetris/projects/07/MemoryAccess/PointerTest/";
        else if (outputFileName == "BasicLoop")
            absolutePath = "C:/Users/ramen/Desktop/nand2tetris/nand2tetris/projects/08/ProgramFlow/BasicLoop/";
        else if (outputFileName == "FibonacciSeries")
            absolutePath = "C:/Users/ramen/Desktop/nand2tetris/nand2tetris/projects/08/ProgramFlow/FibonacciSeries/";
        else if (outputFileName == "SimpleFunction")
            absolutePath = "C:/Users/ramen/Desktop/nand2tetris/nand2tetris/projects/08/FunctionCalls/SimpleFunction/";
        else if (outputFileName == "NestedCall")
            absolutePath = "C:/Users/ramen/Desktop/nand2tetris/nand2tetris/projects/08/FunctionCalls/NestedCall/";
        else if (outputFileName == "FibonacciElement")
            absolutePath = "C:/Users/ramen/Desktop/nand2tetris/nand2tetris/projects/08/FunctionCalls/FibonacciElement/";
        else if (outputFileName == "StaticsTest")
            absolutePath = "C:/Users/ramen/Desktop/nand2tetris/nand2tetris/projects/08/FunctionCalls/StaticsTest/";
    std::ofstream fileOpener;
    //Append the filename to the absolute path, and add the .asm extension to it
    absolutePath.append(outputFileName);
    absolutePath += ".asm"; 
    fileOpener.open(absolutePath, std::ofstream::out | std::ofstream::trunc); //Open the file, and clear whatever was written in it

    if (fileOpener.fail()) //If it fails to open the file, prints an error message
    {
        printf("ERROR: Failed to write to %s!\n", absolutePath.c_str());
        fileOpener.close();
    }
    return fileOpener;
}

//Writes a logical command to the output file, stack = -1 if true, otherwise is 0
void CodeWriter::writeLogical()
{
    boolCount++;
    boolLabel = "(CHECKTRUE" + std::to_string(boolCount) + ")";
    atBool = "@CHECKTRUE" + std::to_string(boolCount);
    stack = 0; //KEEP IN MIND THIS MIGHT HAVE A PROBLEM <--
    linesCounted += 16;
    if (argument == "eq")
        out << "\n@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nD=D-M\nM=-1\n" << atBool << "\nD;JEQ\n@SP\nA=M\nM=0\n" << boolLabel << "\n@SP\nM=M+1\n";
    else if (argument == "lt")
        out << "\n@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nD=M-D\nM=-1\n" << atBool << "\nD;JLT\n@SP\nA=M\nM=0\n" << boolLabel << "\n@SP\nM=M+1\n";
    else if (argument == "gt")
        out << "\n@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nD=M-D\nM=-1\n" << atBool << "\nD;JGT\n@SP\nA=M\nM=0\n" << boolLabel << "\n@SP\nM=M+1\n";

    //For and, or, and not
    else if (argument == "and")
    {
        linesCounted -= 6;
        out  << "\n@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nM=D&M\n@SP\nM=M+1\n";
    }
    else if (argument == "or")
    {
        linesCounted -= 6;
        out << "\n@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nM=D|M\n@SP\nM=M+1\n";
    }
    else if (argument == "not")
    {
        linesCounted -= 10;
        out << "\n@SP\nM=M-1\nA=M\nM=!M\n@SP\nM=M+1\n";
    }
    
    //prevStack = stack; //Update the previous stack and make it the current stack, since the logical cmd updated the stack
    
}

void CodeWriter::writeFunction(std::string functionName, unsigned int nVars)
{
    std::string funcLoop {functionName}; 
    for (unsigned int i {}; i < funcLoop.length(); i++)
        if (funcLoop[i] == ')')
            funcLoop.erase(i);
    
    numOfVars = nVars;
    funcLoop.append(".init)");
    std::string atFuncLoop {funcLoop};
    atFuncLoop.erase(0, 1);
    atFuncLoop.pop_back();
    std::string endF {atFuncLoop};
    endF.append(".end");
    std::string endFLabel {endF};
    endFLabel = "(" + endFLabel + ")";
    atFunc = functionName;
    atFunc.erase(0, 1);
    atFunc.pop_back();
    atFunc = "@" + atFunc + "_RETURN";
    out << "\n" << functionName << "\n@" << nVars << "\nD=A\n@" << atFuncLoop << "\nD;JNE\n@" << endF << "\n0;JMP\n" << funcLoop
        << "\n@SP\nA=M\nM=0\n@SP\nM=M+1\nD=D-1\n@" << atFuncLoop << "\nD;JNE\n" << endFLabel << "\n";
    linesCounted += 14;
}

void CodeWriter::writeCall(std::string functionName, unsigned int numArgs)
{
    linesCounted += 42; //wrong has to go b4
    didCall = true;
    out << "\n@" << linesCounted << "\nD=A\n@SP\nA=M\nM=D\n@LCL\nD=M\n@SP\nM=M+1\nA=M\nM=D\n@ARG\nD=M\n@SP\nM=M+1\nA=M\nM=D\n"
        << "@THIS\nD=M\n@SP\nM=M+1\nA=M\nM=D\n@THAT\nD=M\n@SP\nM=M+1\nA=M\nM=D\n@SP\nM=M+1\nD=M\n"
        <<"@LCL\nM=D\n@5\nD=D-A\n@" << numArgs << "\nD=D-A\n@ARG\nM=D\n" << functionName << "\n0;JMP\n(" << linesCounted << ")\n";
}

void CodeWriter::outputReturnValues()
{
    for (unsigned int i {}; i < 1000; i++)
        if (!currentReturn[i].empty())
            std::cout << currentReturn[i] << " [" << i << "]\n";
}

void CodeWriter::writeInit()
{
    out << "\n@261\nD=A\n@SP\nM=D\n"; //Initialize @SP
    linesCounted += 4;
}

void CodeWriter::writeReturn()
{   

    if (didCall)
    {
        /*out << "\n@ARG\nD=M\n@13\nM=D\n@SP\nM=M-1\nA=M\nD=M\n@ARG\nA=M\nM=D\nD=M\n@13\nD=M\n@SP\nM=D+1\n"
            << "@LCL\nD=M\n@13\nM=D\nA=D-1\nD=M\n@THAT\nM=D\n@13\nD=M\n@2\nA=D-A\nD=M\n@THIS\nM=D\n@13\nD=M\n@3\nA=D-A\nD=M\n@ARG\nM=D\n@13\n"
            << "D=M\n@4\nA=D-A\nD=M\n@LCL\nM=D\n@13\nD=M\n@5\nA=D-A\nD=M\n@14\nM=D\nA=M\n0;JMP\n";*/

        out << "\n@LCL\nD=M\n@13\nM=D\n@5\nA=D-A\nD=M\n@14\nM=D\n@SP\nA=M-1\nD=M\n@ARG\nA=M\nM=D\n@ARG\nD=M\n@SP\nM=D+1\n"
            << "@13\nA=M-1\nD=M\n@THAT\nM=D\n@2\nD=A\n@13\nA=M-D\nD=M\n@THIS\nM=D\n@3\nD=A\n@13\nA=M-D\nD=M\n@ARG\nM=D\n"
            << "@4\nD=A\n@13\nA=M-D\nD=M\n@LCL\nM=D\n@14\nA=M\n0;JMP\n";
        
        linesCounted += 48;
    }
    else
    {
        /*out << "\n@ARG\nD=M\n@13\nM=D\n@SP\nM=M-1\nA=M\nD=M\n@ARG\nA=M\nM=D\nD=M\n@13\nD=M\n@SP\nM=D+1\n"
            << "@LCL\nD=M\n@13\nM=D\nA=D-1\nD=M\n@THAT\nM=D\n@13\nD=M\n@2\nA=D-A\nD=M\n@THIS\nM=D\n@13\nD=M\n@3\nA=D-A\nD=M\n@ARG\nM=D\n@13\n"
            << "D=M\n@4\nA=D-A\nD=M\n@LCL\nM=D\n@13\nD=M\n@5\nA=D-A\nD=M\n@14\nM=D\n";*/
        out << "\n@LCL\nD=M\n@13\nM=D\n@5\nA=D-A\nD=M\n@14\nM=D\n@SP\nA=M-1\nD=M\n@ARG\nA=M\nM=D\n@ARG\nD=M\n@SP\nM=D+1\n"
            << "@13\nA=M-1\nD=M\n@THAT\nM=D\n@2\nD=A\n@13\nA=M-D\nD=M\n@THIS\nM=D\n@3\nD=A\n@13\nA=M-D\nD=M\n@ARG\nM=D\n"
            << "@4\nD=A\n@13\nA=M-D\nD=M\n@LCL\nM=D\n";
        //linesCounted += 51;
        linesCounted += 45;
    }
}

//Pushes the number into the stack
void CodeWriter::writePush()
{
    if (segment[0] == 'C')
    {
        out << "\n" << address << "\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"; 
        linesCounted += 7;
    }
    else if (referencePointer)
    {
        out << "\n" << segment << "\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
        linesCounted += 7;
    }
    else if (!isdigit(segment[1]))
    {
        int referToAddress {getSegmentAddress()};
        std::string referAddressStr {"@" + std::to_string(referToAddress)};
        out << "\n" << segment << "\nD=M\n" << address << "\nA=D+A\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
        linesCounted += 10;
    }
    else
    {
        out << "\n" << segment << "\nD=A\n" << address << "\nA=D+A\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
        linesCounted += 10;
    }
} 


//Gets the segment address, makes it much more efficient this way so instead of writing the ASM code, the program automatically knows which
//address it is
int CodeWriter::getSegmentAddress()
{
    //push local 3 <- stack
    int referToAddress {stack + segmentAddress}; //3 + 300 = 303
    return referToAddress;
}

//Pops the number into the stack
void CodeWriter::writePop()
{
    int referToAddress {getSegmentAddress()};
    std::string referAddressStr {"@" + std::to_string(referToAddress)};
    linesCounted += 18;
    if (!referencePointer && !isdigit(segment[1])) //If referencePointer == false 
        out << segment << "\nD=M\n" << address << "\nD=D+A\n@SP\nA=M\nM=D\n@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M+1\nA=M\nA=M\nM=D\n@SP\nM=M-1\n";
    else if (!referencePointer)
        out << segment << "\nD=A\n" << address << "\nD=D+A\n@SP\nA=M\nM=D\n@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M+1\nA=M\nA=M\nM=D\n@SP\nM=M-1\n";
    else
    {
        linesCounted -= 12;
        out << "\n@SP\nM=M-1\nA=M\nD=M\n" << segment << "\nM=D\n"; // <- @THIS/@THAT
    }
}

//Writes an arithmetic operation, depending on the argument will select the appropriate operation
void CodeWriter::writeArithmetic()
{
    std::string updatedAddress {};
    std::string stackNo {};
    linesCounted += 10;
    if (argument == "add")
    {
        out << "\n@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nM=M+D\n@SP\nM=M+1\n";
        updatedAddress += std::to_string(prevStack + stack); //Updated address stores resulted number
        //NOTE: It is is very important to update the address here, otherwise the program will not print the correct address
    }
    else if (argument == "sub")
    {
        out << "\n@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nM=M-D\n@SP\nM=M+1\n";
        updatedAddress += std::to_string(prevStack - stack);
    }
    else if (argument == "neg")
    {
        linesCounted -= 4;
        out << "\n@SP\nM=M-1\nA=M\nM=-M\n@SP\nM=M+1\n"; 
        updatedAddress += std::to_string(-stack);
    }

    //Update the current address, stack, previous stack, and previous address, maybe redundant but it works
    std::string at {"@"};
    prevAddress = address;
    prevStack = std::stoi(address.erase(0, 1));
    stack = std::stoi(updatedAddress);
    address = at.append(updatedAddress);
}

void CodeWriter::writeLabel(const char* label)
{
    //linesCounted++;
    out << "\n" << label << "\n";
}

void CodeWriter::writeGoto(const char* address)
{
    //linesCounted += 2;
    linesCounted += 2;
    out << "\n" << address << "\n0;JMP\n";
}

void CodeWriter::writeIfGoto(const char* address)
{
    linesCounted += 6;
    out << "\n@SP\nM=M-1\nA=M\nD=M\n" << address << "\nD;JNE\n";
}

//Writes an endless loop to the output file, is called when the .vm instructions are complete
void CodeWriter::infiniteLoop()
{
    //linesCounted += 3;
    linesCounted += 2;
    out << "\n(END)\n@END\n0;JMP";
}