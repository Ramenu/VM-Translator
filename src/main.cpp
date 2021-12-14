#include "parser.h"
int main(int argc, char* argv[])
{
    std::string doDebug {}; 
    if (argc == 3)
        doDebug = argv[2];
    Parser parse; 
    if (doDebug == "-d")
        parse.openFileOrDirectory(argv[1], true);
    else
        parse.openFileOrDirectory(argv[1], false);

    return 0;
}