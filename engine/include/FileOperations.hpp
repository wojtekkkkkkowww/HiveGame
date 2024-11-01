#pragma once
#include <string>
#include <vector>
namespace hive 
{
    std::vector<std::string> loadFromFile(const std::string& path);
    void saveToFile(const std::string& path, const std::vector<std::string>& data);

}