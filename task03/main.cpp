
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "index.h"

using namespace std;

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim))
        elems.push_back(item);
    return elems;
}

int main(int argc, char *argv[])
{
    string root;
    if (argc == 1)
        root = ".";
    else
        root = argv[1];

    // Build deep file index
    FileIndexer index(root);
    index.Build();

    while (1) {
        cout << "> ";
        string line;
        if (!getline(cin, line))
            break;

        vector<string> cmd = split(line, ' ');
        if (cmd.size() == 0)
            continue;
        else if (cmd[0] == "print") {
            if (cmd.size() == 1)
                index.PrintFiles();
            else if (cmd[1] == "name")
                index.PrintFilesSorted(FileIndexer::SortingType::Name);
            else if (cmd[1] == "size")
                index.PrintFilesSorted(FileIndexer::SortingType::Size);
            else if (cmd[1] == "time")
                index.PrintFilesSorted(FileIndexer::SortingType::Time);
        }
        else if (cmd[0] == "find" && cmd.size() == 2) {
            index.FindFiles(cmd[1]);
        }
        else if (cmd[0] == "move" && cmd.size() == 3) {
            if (!index.MoveFile(cmd[1], cmd[2]))
                cout << "File not found" << endl;
        }
        else if (cmd[0] == "del" && cmd.size() == 2) {
            if (!index.DeleteFile(cmd[1]))
                cout << "File not found" << endl;
        }
        else if (cmd[0] == "exit")
            break;
        else
            cout << "Incorrect command!" << endl;
    }

    return 0;
}

