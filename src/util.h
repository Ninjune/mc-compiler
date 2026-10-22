// File: util.h
#pragma once
#include <string>


// extracts file name from path
inline std::string extractFileName(std::string path)
{
    std::string fileName;

    int end = path.size()-1;
    while(end >= 0 && path[end] != '.') end--;
    for(int i = 0; i < end; i++) fileName += path[i];

    return fileName;
}
