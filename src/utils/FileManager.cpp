#include "FileManager.hpp"

namespace Utils
{
    std::vector<std::string>* FileManager::ReadLinesFromFile(std::string filename)
    {
        std::vector<std::string>* lines = new std::vector<std::string>();
        std::ifstream file (filename, std::ios::in);

        std::string line;

        while (std::getline (file, line))
            lines->push_back(line);
            
        file.close();

        return lines;
    }

    std::string FileManager::ReadStringFromFile(std::string filename)
    {
        std::string content = "";
        std::vector<std::string>* lines = ReadLinesFromFile(filename);

        for (std::string& line : *lines)
            content += line + "\n";

        return content;
    }

    std::vector<unsigned char>* FileManager::ReadBytesFromFile(std::string filename)
    {
        std::vector<std::string>* lines = ReadLinesFromFile(filename);
        std::vector<unsigned char>* bytes = new std::vector<unsigned char>();

        for (std::string& line : *lines)
        {
            std::string buffer;
            std::stringstream ss;
            ss << line;

            while (getline(ss, buffer))
                bytes->push_back(stoul(buffer, nullptr, 16));
        }
        return bytes;
    }
};