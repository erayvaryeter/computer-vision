#include <catch2/catch.hpp>
#include <file/file.h>

TEST_CASE("Get Executable Path") {
	auto exeDir = base::File::GetExecutableDirectory();
	CHECK(base::File::DirectoryExists(exeDir) == true);
	auto exePath = base::File::GetExecutablePath();
	CHECK(base::File::FileExists(exePath) == true);
	auto exeDir2 = exePath / "..";
	CHECK(exeDir == exeDir2);
	CHECK(exeDir / ".." == exeDir2 / "..");
	CHECK(exeDir / ".." / "test" / "test" == exeDir2 / ".." / "test" / "test");
	auto ext = base::File::GetFileExtension(exePath);
	CHECK(ext == ".exe");
	auto files = base::File::GetFilesWithExtension(exeDir);
	CHECK(files.size() > 0);
}

TEST_CASE("Add & Remove Files and Directories") {
	auto newDir = base::File::GetExecutableDirectory() / "TestDirectory";
	CHECK(base::File::Create_Directory(newDir) == true);
	CHECK(base::File::DirectoryExists(newDir) == true);
	auto newFile = newDir / "file.txt";
	CHECK(base::File::Create_File(newFile) == true);
	CHECK(base::File::FileExists(newFile) == true);
	CHECK(base::File::Remove_File(newFile) == true);
	CHECK(base::File::FileExists(newFile) == false);
	CHECK(base::File::Remove_Directory(newDir) == true);
	CHECK(base::File::DirectoryExists(newDir) == false);
}