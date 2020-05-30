#include "FileParser.h"

vector<string> FileParser::parseFile(char* filePath) {
    FILE* file = fopen(filePath, "r");
    vector<string> result = {string()};
    for(char letter = fgetc(file); letter != EOF; letter = fgetc(file)){
        result[result.size()-1] += letter;
        if(letter == '\n'){
            result.push_back(string());
        }
    }
    fclose(file);
    return result;
}

FileParser::FileParser() {}

FileParser::~FileParser() {}