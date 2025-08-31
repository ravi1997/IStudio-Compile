#ifndef FS_FILE_HPP_
#define FS_FILE_HPP_

#include "Types_Compiler.hpp"
#include "Exception.hpp"

class File
{
private:
    std::fstream fileStream;

public:
    File() = default;

    File(const fs::path &filePath)
    {
        open(filePath);
    }

    ~File()
    {
        close();
    }
    // Opens the file for reading
    bool open(const fs::path &filePath)
    {
        if (fs::exists(filePath))
        {
            fileStream.open(filePath);
            return fileStream.is_open();
        }
        else
        {
            throw IStudio::Exception::FileNotFoundException{std::string{"File not existing in the path : "} + filePath.string()};
        }
    }

    void close()
    {
        if (fileStream.is_open())
        {
            fileStream.close();
        }
    }
};

#endif // FS_FILE_HPP_