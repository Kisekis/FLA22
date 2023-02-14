#include <iostream>
#include "TM.h"
using namespace std;

int main(int argc, char *argv[])
{
    bool verbose = false;
    string path;
    string input;
    for (int i = 1; i < argc; i++)
    {
        auto arg = argv[i];
        if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0)
        {
            cout << "usage: turing [-v|--verbose] [-h|--help] <tm> <input>" << endl;
            return 0;
        }
        if (strcmp(arg, "-v") == 0 || strcmp(arg, "--verbose") == 0)
        {
            verbose = true;
            continue;
        }
        if (path.empty())
        {
            path = argv[i];
            continue;
        }
        input = argv[i];
    }
    auto tm = parseFile(path, verbose);
    tm->run(input);
    return 0;
}
