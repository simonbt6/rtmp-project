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

        for (const std::string& line : *lines)
            content.append(line + std::string("\n"));
            
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

        // MemoryManagement::FreeVector<std::string>(lines);

        return bytes;
    }

    std::vector<uint8_t>* FileManager::ReadBinaryFile(std::string filename)
    {
        std::vector<uint8_t>* input_bytes = new std::vector<uint8_t>();
        std::ifstream input(filename, std::ios::binary);
        input.unsetf(std::ios::skipws);

        std::streampos input_size;
        input.seekg(0, std::ios::end);
        input_size = input.tellg();
        input.seekg(0, std::ios::beg);

        input_bytes->reserve(input_size);

        input_bytes->insert(input_bytes->begin(), std::istream_iterator<uint8_t>(input), std::istream_iterator<uint8_t>());

        return input_bytes;
    }
};