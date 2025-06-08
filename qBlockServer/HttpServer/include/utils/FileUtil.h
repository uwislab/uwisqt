#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

#include <muduo/base/Logging.h>

class FileUtil {
public:
    explicit FileUtil(const std::string& filePath)
        : filePath_(filePath)
    {
        // 使用 C++17 文件系统库检查文件存在性
        if (std::filesystem::exists(filePath_)) {
            openForRead();
        }
    }

    ~FileUtil() {
        close();
    }

    bool openForRead() {
        file_.open(filePath_, std::ios::in | std::ios::binary);
        return file_.is_open();
    }

    bool openForWrite(bool append = false) {
        auto mode = std::ios::out | std::ios::binary;
        if (append) mode |= std::ios::app;
        else mode |= std::ios::trunc;

        file_.open(filePath_, mode);
        return file_.is_open();
    }

    void close() {
        if (file_.is_open()) {
            file_.close();
        }
    }

    bool exists() const {
        return std::filesystem::exists(filePath_);
    }

    uint64_t size() const {
        return std::filesystem::file_size(filePath_);
    }

    bool read(std::vector<char>& buffer) {
        if (!exists()) return false;
        if (!file_.is_open() && !openForRead()) return false;

        buffer.resize(size());
        file_.seekg(0);
        return !!file_.read(buffer.data(), buffer.size());
    }

    bool write(const std::vector<char>& buffer, bool append = false) {
        close();  // 关闭可能存在的读模式

        if (!openForWrite(append)) {
            LOG_ERROR << "Cannot create/write to file: " << filePath_ << "\n";
            return false;
        }

        return !!file_.write(buffer.data(), buffer.size());
    }

    bool createIfNotExist() {
        if (exists()) return true;
        std::ofstream temp(filePath_);
        return temp.is_open();
    }

private:
    std::string filePath_;
    std::fstream file_;
};