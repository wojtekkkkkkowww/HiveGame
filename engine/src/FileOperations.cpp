#include "FileOperations.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

namespace
{
std::string g_outputDir = "build/tests/games";
bool g_outputCleared = false;

void ensureOutputDirExists()
{
    std::filesystem::path dir(g_outputDir);
    if (!dir.empty() && !std::filesystem::exists(dir))
    {
        std::filesystem::create_directories(dir);
    }
}
} // namespace

std::vector<std::string> hive::loadFromFile(const std::string &path)
{
    std::ifstream file(path);
    std::string line;
    std::vector<std::string> data;
    while (std::getline(file, line))
    {
        data.push_back(line);
    }
    return data;
}

void hive::setOutputDir(const std::string &dir) { g_outputDir = dir; }

std::string hive::getOutputDir() { return g_outputDir; }

void hive::clearOutputFiles()
{
    ensureOutputDirExists();
    for (const auto &entry : std::filesystem::directory_iterator(g_outputDir))
    {
        if (entry.path().extension() == ".txt")
        {
            std::error_code ec;
            std::filesystem::remove(entry.path(), ec);
            if (ec)
            {
                std::cerr << "Failed to remove " << entry.path() << ": " << ec.message() << '\n';
            }
        }
    }
}

void hive::saveToFile(const std::string &path, const std::vector<std::string> &data)
{
    ensureOutputDirExists();
    if (!g_outputCleared)
    {
        clearOutputFiles();
        g_outputCleared = true;
    }

    std::filesystem::path fullPath = std::filesystem::path(g_outputDir) / (path + ".txt");
    std::ofstream file(fullPath);
    for (const auto &line : data)
    {
        file << line << '\n';
    }
    file.close();

    if (std::filesystem::exists(fullPath))
    {
        std::cerr << "Saved game to " << fullPath << '\n';
    }
}
