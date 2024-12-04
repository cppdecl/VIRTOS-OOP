#ifndef UTILS_IO_HH
#define UTILS_IO_HH

#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

namespace IO 
{
    inline bool IsExistingFile(std::string_view fileName) 
    {
        struct stat buffer;
        return (stat(fileName.data(), &buffer) == 0);
    }

    inline bool IsExistingDirectory(std::string_view dirName) 
    {
        return std::filesystem::exists(dirName);
    }

    inline bool CreateDirectoryRecursive(std::string_view dirName) 
    {
        std::error_code err;
        if (!std::filesystem::create_directories(dirName.data(), err)) 
        {
            if (std::filesystem::exists(dirName.data())) 
                return true;

            return false;
        }
        return true;
    }

    inline bool CreateFile(std::string_view fileName) 
    {
        FILE* fp = nullptr;
        fp = fopen(fileName.data(), "wb");

        if (!fp)
            return false;

        fclose(fp);
        return true;
    }

    inline bool DeleteFile(std::string_view fileName) 
    {
        int res = remove(fileName.data());
        return res == 0;
    }

	// Save binary data to a file
    inline bool SaveFile(uint8_t* data, size_t size, const std::string& path) 
    {
		std::filesystem::path p = path;
		IO::CreateDirectoryRecursive(p.parent_path().string());

        FILE* fp = nullptr;
#ifdef _WIN32
        fopen_s(&fp, path.c_str(), "wb");
#else
        fp = fopen(path.c_str(), "wb");
#endif
        if (!fp) 
            return false;

        fwrite(data, 1, size, fp);
        fflush(fp);
        fclose(fp);

        return true;
    }

    // Load binary data from a file
    inline uint8_t* LoadFile(size_t& size, const std::string& path)
    {
        size = std::filesystem::file_size(path);
        void* data = malloc(size);

        if (!data)
            return nullptr;

        FILE* fp = nullptr;
#ifdef _WIN32
        fopen_s(&fp, path.c_str(), "rb");
#else
        fp = fopen(path.c_str(), "rb");
#endif

        if (!fp)
            return nullptr;

        fread(data, 1, size, fp);
        fclose(fp);

        return static_cast<uint8_t*>(data);
    }

    // Save string content to a file
    inline bool SaveFile(const std::string& content, const std::string& path) 
    {
		std::filesystem::path p = path;
		IO::CreateDirectoryRecursive(p.parent_path().string());

        std::ofstream file(path, std::ios::out | std::ios::trunc);
        if (!file.is_open())
            return false;

        file << content;
        file.close();
        return true;
    }

    // Load string content from a file
    inline bool LoadFile(std::string& content, const std::string& path) 
    {
        std::ifstream file(path, std::ios::in);
        if (!file.is_open())
            return false;

        std::ostringstream oss;
        oss << file.rdbuf();
        content = oss.str();
        file.close();
        return true;
    }
}

#endif // UTILS_IO_HH
