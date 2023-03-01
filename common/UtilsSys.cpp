#include "UtilsSys.h"
#include <fstream>
#include <iostream>
using namespace std;

void ReadFile(const std::string &path, std::string &content)
{
    ifstream infile;
    infile.open(path);
    string line;
    while (getline(infile, line))
    {
        content += line;
    }
    infile.close();
}
