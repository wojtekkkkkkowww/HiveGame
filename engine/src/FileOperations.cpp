#include "FileOperations.hpp"
#include <fstream>
#include <iostream>

std::vector<std::string> hive::loadFromFile(const std::string &path)
{
    std::ifstream file(path);
    std::string line;
    std::vector<std::string> data;
    while (std::getline(file, line))
    {
        data.push_back(line);
    }
    for(auto move : data){
        std::cerr << move << std::endl;
    }
    return data;
}

void hive::saveToFile(const std::string &path, const std::vector<std::string> &data)
{   
    std::ofstream file(path + ".txt");
    for(auto line : data){
        file << line << std::endl;
    }
    file.close();
}
