#include "StatusFile.hpp"
#include "logger/Logger.hpp"
#include <cstring>
#include <filesystem>
#include <fstream>
#include <ostream>
#include <spdlog/spdlog.h>

//---------------------------------------------------------------------------
namespace jutta_driver {
//---------------------------------------------------------------------------
StatusFile::StatusFile(std::filesystem::path&& path) : path(std::move(path)) {
    create_file();
}

StatusFile::~StatusFile() {
    remove_file();
}

const std::filesystem::path& StatusFile::get_path() const {
    return path;
}

void StatusFile::replace_contents(const char* buffer, size_t len) const {
    // Reduce strain on the file system by first checking if the file already contains the correct value:
    if (!check_file_contents(buffer, len)) {
        std::ofstream file(path, std::ofstream::trunc | std::ofstream::out);
        if (len > 0) {
            file.write(buffer, len);
        }
        file.close();
    }
}

void StatusFile::replace_contents(const std::vector<uint8_t>& buffer) const {
    // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
    replace_contents(reinterpret_cast<const char*>(buffer.data()), buffer.size());
}

void StatusFile::replace_contents(const std::string& buffer) const {
    replace_contents(buffer.data(), buffer.size());
}

void StatusFile::create_file() {
    std::ofstream file(path, std::ofstream::trunc | std::ofstream::out);
    file.close();
}

void StatusFile::remove_file() {
    if (std::filesystem::exists(path)) {
        if (!std::filesystem::remove(path)) {
            SPDLOG_WARN("Failed to remove file at: {}", path.c_str());
        }
    }
    SPDLOG_DEBUG("File '{}' removed.", path.c_str());
}

bool StatusFile::check_file_contents(const char* str, size_t len) const {
    std::ifstream file(path);
    file.seekg(0, std::ios::end);
    size_t tmpLen = file.tellg();
    if (tmpLen != len) {
        return false;
    }

    std::string tmp;
    tmp.reserve(tmpLen);
    file.seekg(0, std::ios::beg);
    tmp.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return std::strcmp(tmp.c_str(), str) == 0;
}
//---------------------------------------------------------------------------
}  // namespace jutta_driver
//---------------------------------------------------------------------------
