#include "parser.h"
#include "bloat.h"
#include <filesystem>

namespace fs = std::filesystem;
CodeWriter codeWrite;

//Method for opening and reading from file with ifstream, parameter is the filepath
std::ifstream Parser::openAndReadFile(const char* filePath)
{
    fs::path pathToFile = filePath;
    std::ifstream readFile;
    bool successfullyOpened {};

    //Checks if the file is a .vm file, if it is the ifstream will successfully open the file   
    if (pathToFile.extension().string() != ".vm")
        printf("ERROR: File %s is not a .vm file!\n", filePath);
    else
    {
        readFile.open(filePath);

        //In case of failing to read the file, print an error message
        if (readFile.fail())
        {
            printf("ERROR: Failed to open %s!\n", filePath);
            readFile.close();
        }
        else
            return readFile;
    }
    return readFile;
}

//Checks whether or not the path given is a file or directory, and whether it exists or not
void Parser::openFileOrDirectory(const char* path, bool debug)
{
    if (debug)
        doDebug = true;
    if (fs::exists(path)) //If the file/path exists..
        //Check if the path/file is a file, if not its a path, performs the appropriate command depending on what it is
        if (fs::is_regular_file(path))
        {
            readVM(path, false);
        }
        else
            openDirectory(path);
    else
        printf("ERROR: No file or path at %s was found!\n", path); //Output an error at the path given
}

//Method that opens the directory, with the pathname as a parameter
void Parser::openDirectory(const char* path)
{
    isDirectory = true;
    //Initialize variables
    std::string filePath {}; //<-- create a string for the path since <filesystem> does not support const char
    codeWrite.outputFileName = fs::path(path).filename().string();
    unsigned int count {};
    unsigned int statBaseAddress {16};
    bool checkedSysVm {};
    while (true) 
    {   
        //Go into loop and keep iterating through every file in the directory
        for (const auto& entry: fs::directory_iterator(path))
        {
            if (entry.path().filename().extension() == ".vm" && count == 0)
            {
                std::string baseAddressStr {"@" + std::to_string(statBaseAddress)};
                staticMap[entry.path().filename().replace_extension().string()] = baseAddressStr;
                statBaseAddress += 2;
            }
            //First check if "Sys.vm" is found, since it must be outputted to the .asm file first
            if (entry.path().filename().string() == "Sys.vm" && !checkedSysVm)
            {
                filePath = entry.path().string();
                readVM(filePath.c_str(), true);
                checkedSysVm = true;
            }
            //Otherwise if the extension is .vm and "Sys.vm" was already checked, and file is not Sys.vm (since Sys.vm was already outputted)
            //it would be outputted twice
            else if (entry.path().filename().extension().string() == ".vm" && checkedSysVm && entry.path().filename().string() != "Sys.vm")
            {
                filePath = entry.path().string();
                readVM(filePath.c_str(), true);
            }
        }
        count++;
        if (count == 2)
            break;
    }
    codeWrite.infiniteLoop();
    CodeWriter::out.close();
    //If "Sys.vm" was not found, output an error at the directory path
    if (!checkedSysVm)
        printf("ERROR: There is no Sys.vm file found at %s!\n", path);
}


//This method reads the .vm file line by line and parses it, sending each line to the appropriate function
void Parser::readVM(const char* filePath, bool contReading)
{
    std::ifstream fromVM {openAndReadFile(filePath)}; //Open the file
    //If opening the file was successful
    if (!fromVM.fail())
    {
        std::string filePathStr {filePath};
        //Iterate through the string and find the file name, remove the .vm extension, useful for writing to output
        if (!contReading)
        {
            codeWrite.outputFileName = filePath;
            for (unsigned int i {}; i < codeWrite.outputFileName.length(); i++)
                if (codeWrite.outputFileName[i] == '.')
                    codeWrite.outputFileName.erase(i, codeWrite.outputFileName.back());
        }
        if (!CodeWriter::out.is_open())
        {
            CodeWriter::out = codeWrite.openOutputFile(doDebug); //Call openOutputFile and write to the file
            if (fs::path(filePath).has_parent_path())
                codeWrite.writeInit(); //Initialize the segments}
        }
        codeWrite.memoryToThis = 0;
        codeWrite.memoryToThat = 0;
        //While the file contains lines
        while (std::getline(fromVM, line))
        {
            //If the line is not empty and is not a comment..
            if (!line.empty())
            {
                lineWithSpace = line;
                line = removeWhiteSpace(line); //Remove any white space between the lines
                if (line[0] != '/')
                {
                    if (doDebug)
                        CodeWriter::out << "\n" << line << "\n-----------------------------------\n";
                    line = removeLineComments(line); //Remove any comments if the line contains them
                    lineWithSpace = removeLineComments(lineWithSpace);

                    //Set referencepointer and push to false, in case they were set to true previously
                    codeWrite.referencePointer = false; 
                    isPush = false;
    
                    switch (line[0])
                    {
                        case 'p':  parsePushOrPop(); break;
                        case 'a':  parseAndOrAdd(); break;
                        case 's':  executeCommand("sub"); break;
                        case 'e':  executeCommand("eq"); break;
                        case 'g':  parseGtOrGoto(); break;
                        case 'l':  parseLtOrLabel(); break;
                        case 'o':  executeCommand("or"); break;
                        case 'n':  parseNotOrNeg(); break;
                        case 'i':  codeWrite.writeIfGoto(rmCommandGetName(lineWithSpace, false)); break;
                        case 'f': 
                            currentFunctionIn = rmCommandGetName(lineWithSpace, true); 
                            codeWrite.writeFunction(currentFunctionIn, std::stoi(seperateNameFromVal(lineWithSpace, false))); 
                            break;
                        case 'r':  codeWrite.writeReturn(); break;
                        case 'c':  codeWrite.writeCall(rmCommandGetName(lineWithSpace, false), std::stoi(seperateNameFromVal(lineWithSpace, false))); break;
                    }
                }
            }
        }
        if (!contReading)
        {
            codeWrite.infiniteLoop(); //After the file is empty, write an infinite loop to the output file
            CodeWriter::out.close(); //Close the output file
        }
        codeWrite.outputReturnValues();
    }
}



void Parser::parseLtOrLabel()
{
    if (line.find("label") != std::string::npos)
        executeCommand("label");
    else
        executeCommand("lt");
}


void Parser::parseGtOrGoto()
{
    if (line.find("goto") != std::string::npos)
        executeCommand("goto");
    else
        executeCommand("gt");
}

//Checks if the command is and or add..
void Parser::parseAndOrAdd()
{
    if (line.find("and") != std::string::npos)
        executeCommand("and"); 
    else
        executeCommand("add");
}

//Checks if the command is push or pop..
void Parser::parsePushOrPop()
{
    //Update the previous address to make it the current address, since the stack moves to the top, including stack#
    codeWrite.prevAddress = codeWrite.address;
    codeWrite.prevStack = codeWrite.stack;

    codeWrite.stack = std::stoi(parseAddress(false)); //Call to parseAddress, parameter means to not include "@"
    codeWrite.address = parseAddress(true); //Parses the address within the line, and includes the @ which indicates an address

     
    if (line.find("push") != std::string::npos)
    {
        isPush = true;
        executeCommand("push");
        //If the command is not a push pointer, then update the last number pushed
        //Reason to not include "push pointer" is because the pointer values 
        if (line.find("pointer") == std::string::npos)
            lastNumPushed = std::stoi(parseAddress(false));
    }
    else
    {
        isPush = false;
        executeCommand("pop");
    }
}

//Checks if the command is not or neg..
void Parser::parseNotOrNeg()
{
    if (line.find("not") != std::string::npos)
        executeCommand("not");
    else
        executeCommand("neg");
}

//Returns a string that contains the address within the line, the bool parameter lets the function know if its a address or not
//If it is.. will return something like @21, if not.. just 21 instead
std::string Parser::parseAddress(bool address) 
{
    std::string addressStr {};
    if (address)
        addressStr += '@';

    //Iterate through the line, and check where the number is located, append that to the string
    for (int i = 0; i < line.length(); i++)
        if (isdigit(line[i]))
            addressStr += line[i];

    return addressStr;
}

//Finds the segment within the line
void Parser::findSegment()
{
    if (line.find("local") != std::string::npos)
    {
        codeWrite.segment = "@LCL";
        codeWrite.segmentAddress = 300;
    }
    else if (line.find("argument") != std::string::npos)
    {
        codeWrite.segment = "@ARG";
        codeWrite.segmentAddress = 400;
    }
    else if (line.find("this") != std::string::npos)
    {
        codeWrite.segment = "@THIS";
        codeWrite.segmentAddress = codeWrite.memoryToThis;
    }
    else if (line.find("that") != std::string::npos)
    {
        codeWrite.segment = "@THAT";
        codeWrite.segmentAddress = codeWrite.memoryToThat;
    }
    else if (line.find("static") != std::string::npos)
    {
        if (!isDirectory)
        {
            codeWrite.segment = "@16";
            codeWrite.segmentAddress = 16;
        }
        else
        {
            for (auto&i: staticMap)
            {
                if (currentFunctionIn.find(i.first) != std::string::npos)
                    codeWrite.segment = i.second.c_str();
            }
        }
    }
    else if (line.find("temp") != std::string::npos)
    {
        codeWrite.segment = "@5";
        codeWrite.segmentAddress = 5;
    }
    else if (line.find("pointer") != std::string::npos)
    {
        if (codeWrite.stack == 0)
        {
            codeWrite.segment = "@THIS";
            if (!isPush) //Make sure that the command is not push, and is pop, otherwise @THIS/THAT will store incorrect memory addresses
                codeWrite.memoryToThis = lastNumPushed; //ITS BECAUSE ITS NOT POP...
        }
        else
        {
            codeWrite.segment = "@THAT";  
            if (!isPush)
                codeWrite.memoryToThat = lastNumPushed;
        }
        codeWrite.referencePointer = true; //Set referencepointer to true (useful for when writing the ASM code to the output file)
    }
    else
        codeWrite.segment = "C"; //Is constant, only 1 character because it is a const char*

}


//Executes the appropriate command depending on the parameter
void Parser::executeCommand(std::string command)
{
    findSegment(); //Finds the segment within the line, and updates the segment to the approrpiate value (e.g. "constant", "local", etc)
    codeWrite.argument = command;
    if (command == "push")
        codeWrite.writePush();
    else if (command == "pop")
        codeWrite.writePop();
    else if (command == "add" || command == "sub" || command == "neg")
        codeWrite.writeArithmetic();
    else if (command == "label")
        codeWrite.writeLabel(rmCommandGetName(lineWithSpace, true));
    else if (command == "goto")
        codeWrite.writeGoto(rmCommandGetName(lineWithSpace, false));
    else
        codeWrite.writeLogical();
}
