// File: util.h
#pragma once
#include <string>
#include <iostream>


// extracts file name from path
inline std::string extractFileName(std::string path)
{
    std::string fileName;

    int end = path.size()-1;
    while(end >= 0 && path[end] != '.') end--;
    for(int i = 0; i < end; i++) fileName += path[i];

    return fileName;
}


inline void error(std::string s, int line)
{
    s = "error: line " + std::to_string(line) + ": " + s + "\n";

    std::cerr << s << std::endl;
    /*std::string errorFilePath = extractFileName(inputFilePath) + ".err";

    std::ofstream file(errorFilePath);

    if(!file.is_open())
        std::cerr << "Could not open the input file! Path: " << inputFilePath;

    file << s;

    file.close();*/
}



