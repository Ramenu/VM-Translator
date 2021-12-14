#ifndef BLOAT_H
#define BLOAT_H

#include <iostream>
#include <algorithm>

extern std::string removeWhiteSpace(std::string codeLine);
extern std::string removeLineComments(std::string codeLine);
extern std::string seperateNameFromVal(std::string codeLine, bool removeVal);
extern std::string removeCommandFromString(std::string codeLine);
extern const char* rmCommandGetName(std::string codeLine, bool isLabel);

#endif