#include <vector>
#include <string>
#include<iostream>

using namespace std;

class FileParser{
public:
    FileParser();
    ~FileParser();
    vector<string> parseFile(char* filePath);
};