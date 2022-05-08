#pragma once

#include <memory>
#include <vector>
#include <string>

namespace base {

class Logger;

class File {

public:
	static bool FileExists(std::string& file);
	static bool DirectoryExists(std::string& dir);

	static bool Create_Directory(std::string& dir);
	static bool Remove_Directory(std::string& dir);
	static bool Create_File(std::string& file);
	static bool Remove_File(std::string& file);
	
	static std::string GetExecutableDirectory();
	static std::string GetExecutablePath();
	static std::string GetParentPath(std::string& path);
	static std::string GetFileExtension(std::string& file);
	static std::vector<std::pair<std::string, std::string>> GetFilesWithExtension(std::string& dir, std::string ext = ".*");
	static std::vector<std::string> GetDirectories(const std::string& dir);
	
	static std::shared_ptr<base::Logger> GetLogger() { return m_logger; }

private:
	static std::shared_ptr<base::Logger> m_logger;
};

} // namespace base

template<typename T1, typename T2>
std::string operator/(T1 str1, T2 str2);