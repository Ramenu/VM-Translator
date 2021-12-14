#include "bloat.h"

std::string removeWhiteSpace(std::string codeLine)
{
    codeLine.erase(std::remove_if(codeLine.begin(), codeLine.end(), isspace), codeLine.end()); 
    return codeLine;
}

std::string removeLineComments(std::string codeLine)
{
    for (unsigned int i = 0; i < codeLine.length(); i++)
        if (codeLine[i] == '/')
            codeLine.erase(i, codeLine.back());            

    return codeLine;
}

std::string seperateNameFromVal(std::string codeLine, bool removeVal)
{
    for (unsigned int i {}; i < codeLine.length(); i++)
        if (isspace(codeLine[i]) && isdigit(codeLine[i + 1]))
            if (removeVal)
                codeLine.erase(i, codeLine.back());
            else
                codeLine.erase(0, i);

    return codeLine;
}

std::string removeCommandFromString(std::string codeLine)
{
    unsigned int charsBeforeSpace {};
    for (unsigned int i {}; i < codeLine.length(); i++)
        if (isspace(codeLine[i]))
        {
            codeLine.erase(0, charsBeforeSpace);
            break;
        }
        else
            charsBeforeSpace++;
    
    return codeLine;
}

const char* rmCommandGetName(std::string codeLine, bool isLabel)
{
    codeLine = seperateNameFromVal(codeLine, true);
    codeLine = removeCommandFromString(codeLine);
    codeLine = removeWhiteSpace(codeLine);

    if (isLabel)
        codeLine = "(" + codeLine + ")";
    else
        codeLine = "@" + codeLine;

    return codeLine.c_str();
}