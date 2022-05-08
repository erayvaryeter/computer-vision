#include <logger/logger.h>
#include <string/string.h>
#include <assertion/assertion.h>
#include <filesystem>
#include <fstream>
#include <Windows.h>

#include "file/file.h"

namespace base {

std::shared_ptr<base::Logger> File::m_logger = std::make_shared<base::Logger>();

bool 
File::FileExists(std::string& file) {
    return std::filesystem::exists(file);
}

bool 
File::DirectoryExists(std::string& dir) {
    if (std::filesystem::exists(dir)) {
        if (std::filesystem::is_directory(dir)) {
            return true;
        }
    }
    return false;
}

bool 
File::Create_Directory(std::string& dir) {
    auto exist = base::File::DirectoryExists(dir);
    if (exist) {
        m_logger << MESSAGE("Given directory already exists", base::Logger::Severity::Error);
        return false;
    }
    else {
        return std::filesystem::create_directory(dir);
    }
}

bool 
File::Remove_Directory(std::string& dir) {
    auto exist = base::File::DirectoryExists(dir);
    if (!exist) {
        m_logger << MESSAGE("Given directory does not exist", base::Logger::Severity::Error);
        return false;
    }
    else {
        return std::filesystem::remove_all(dir);
    }
}

bool 
File::Create_File(std::string& file) {
    auto exist = base::File::FileExists(file);
    if (exist) {
        m_logger << MESSAGE("Given file already exists", base::Logger::Severity::Error);
        return false;
    }
    else {
        std::ofstream ofs(file.c_str());
        bool success = ofs.is_open();
        ofs.close();
        return success;
    }
}

bool 
File::Remove_File(std::string& file) {
    auto exist = base::File::FileExists(file);
    if (!exist) {
        m_logger << MESSAGE("Given directory does not exist", base::Logger::Severity::Error);
        return false;
    }
    else {
        return std::filesystem::remove(file);
    }
}

std::string
File::GetExecutableDirectory() {
	auto exeDir = base::String::ReplaceAll(std::filesystem::current_path().string(), "\\", "/");
    ASSERT(!exeDir.empty(), "Replacing current executable directory string's backslashes with forward slashes failed", base::Logger::Severity::Error);
    return exeDir;
}

std::string 
File::GetExecutablePath() {
    char buf[MAX_PATH];
    GetModuleFileNameA(nullptr, buf, MAX_PATH);
    auto exePath = base::String::ReplaceAll((const char*)buf, "\\", "/");
    ASSERT(!exePath.empty(), "Replacing current executable path string's backslashes with forward slashes failed", base::Logger::Severity::Error);
    return exePath;
}

std::string 
File::GetParentPath(std::string& path) {
    std::filesystem::path p(path);
    ASSERT(p.has_parent_path(), "Given path has no parent directory", base::Logger::Severity::Error);
    return p.parent_path().string();
}

std::string 
File::GetFileExtension(std::string& file) {
    auto exist = base::File::FileExists(file);
    std::filesystem::path p(file);
    if (p.has_extension())
        return p.extension().string();
    else
        return "";
}

std::vector<std::pair<std::string, std::string>>
File::GetFilesWithExtension(std::string& dir, std::string ext) {
    std::vector<std::pair<std::string, std::string>> pair;
    for (auto& p : std::filesystem::recursive_directory_iterator(dir)) {
        if (ext == ".*")
            pair.emplace_back(p.path().string(), p.path().stem().string());
        else
            if (p.path().extension() == ext) {
                pair.emplace_back(p.path().string(), p.path().stem().string());
            }
    }
    return pair;
}

std::vector<std::string>
File::GetDirectories(const std::string& dir) {
    std::vector<std::string> directories;
    for (auto& p : std::filesystem::recursive_directory_iterator(dir))
        if (p.is_directory())
            directories.push_back(p.path().string());
    return directories;
}

}

template<typename T1, typename T2>
std::string operator/(T1 str1, T2 str2) {
    if ((std::is_same<T1, std::string>::value || std::is_same<T1, const char*>::value) &&
        (std::is_same<T2, std::string>::value || std::is_same<T2, const char*>::value)) {
        std::string input1(str1);
        std::string input2(str2);
        if (input2 == "..") {
            return base::File::GetParentPath(input1);
        }
        else {
            return base::String::CombineStrings(input1, "/", input2);
        }
    }
    else {
        base::File::GetLogger() << MESSAGE("Input arguments must be either type of std::string or const char*", base::Logger::Severity::Error);
    }
}

void TempFunc() {
    std::string str1 = "";
    const char* str2 = "";

    auto x1 = str1 / str1;
    auto x2 = str1 / str2;
    auto x3 = str2 / str1;
}