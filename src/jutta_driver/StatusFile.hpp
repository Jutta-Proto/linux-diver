#pragma once

#include <filesystem>
#include <vector>

//---------------------------------------------------------------------------
namespace jutta_driver {
//---------------------------------------------------------------------------
class StatusFile {
 private:
    std::filesystem::path path;

 public:
    explicit StatusFile(std::filesystem::path&& path);
    StatusFile(StatusFile&&) = default;
    StatusFile(const StatusFile&) = default;
    StatusFile& operator=(StatusFile&&) = delete;
    StatusFile& operator=(const StatusFile&) = delete;
    ~StatusFile();

    /**
     * Returns the path to the FIFO file.
     **/
    [[nodiscard]] const std::filesystem::path& get_path() const;

    /**
     * Writes the given buffer to the file, replacing existing content.
     **/
    void replace_contents(const char* buffer, size_t len) const;
    /**
     * Writes the given buffer to the file, replacing existing content.
     **/
    void replace_contents(const std::vector<uint8_t>& buffer) const;
    /**
     * Writes the given string to the file, replacing existing content.
     **/
    void replace_contents(const std::string& buffer) const;

 private:
    void create_file();
    void remove_file();
    /**
     * Checks if the file contains the given string and returns true if it does.
     **/
    [[nodiscard]] bool check_file_contents(const char* str, size_t len) const;
};
//---------------------------------------------------------------------------
}  // namespace jutta_driver
//---------------------------------------------------------------------------
