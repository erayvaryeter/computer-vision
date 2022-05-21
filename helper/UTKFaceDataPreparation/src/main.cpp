#include <file/file.h>
#include <string/string.h>
#include <iostream>
#include <filesystem>

std::string
CreateAgeDirectories(std::string targetDir) {
	auto ageDir = base::String::CombineStrings(targetDir, "/Age");
	base::File::Create_Directory(ageDir);
	// 1 - 116
	for (int i = 1; i <= 116; ++i) {
		auto ageDirectory = base::String::CombineStrings(ageDir + "/" + std::to_string(i));
		base::File::Create_Directory(ageDirectory);
	}
	return ageDir;
}

std::string
CreateGenderDirectories(std::string targetDir) {
	auto genderDir = base::String::CombineStrings(targetDir, "/Gender");
	base::File::Create_Directory(genderDir);
	auto maleDir = base::String::CombineStrings(genderDir, "/Male");
	auto femaleDir = base::String::CombineStrings(genderDir, "/Female");
	base::File::Create_Directory(maleDir);
	base::File::Create_Directory(femaleDir);
	return genderDir;
}

std::string
CreateRaceDirectories(std::string targetDir) {
	auto raceDir = base::String::CombineStrings(targetDir, "/Race");
	base::File::Create_Directory(raceDir);
	auto whiteDir = base::String::CombineStrings(raceDir, "/White");
	auto blackDir = base::String::CombineStrings(raceDir, "/Black");
	auto asianDir = base::String::CombineStrings(raceDir, "/Asian");
	auto indianDir = base::String::CombineStrings(raceDir, "/Indian");
	auto otherDir = base::String::CombineStrings(raceDir, "/Other");
	base::File::Create_Directory(whiteDir);
	base::File::Create_Directory(blackDir);
	base::File::Create_Directory(asianDir);
	base::File::Create_Directory(indianDir);
	base::File::Create_Directory(otherDir);
	return raceDir;
}

int main() {
	std::string completeDataset = "D:/Repositories/UTKFace";
	std::string targetDirectory = "D:/Repositories/FaceDataset";
	base::File::Remove_Directory(targetDirectory);
	base::File::Create_Directory(targetDirectory);
	std::string ageRootDir = CreateAgeDirectories(targetDirectory);
	std::string genderRootDir = CreateGenderDirectories(targetDirectory);
	std::string raceRootDir = CreateRaceDirectories(targetDirectory);

	auto files = base::File::GetFilesWithExtension(completeDataset);
	for (auto& file : files) {
		auto oldName = file.second;
		auto newName = base::String::ReplaceAll(oldName, ".chip", "");
		// split with _ 
		auto separated = base::String::SplitString(file.second, "_");
		// retrieve age
		auto ageStr = separated[0];
		std::filesystem::copy_file(file.first, ageRootDir + "/" + ageStr + "/" + newName);
		// retrieve gender
		auto genderStr = separated[1];
		if (genderStr == "0") {
			std::filesystem::copy_file(file.first, genderRootDir + "/Male/" + newName);
		}
		else if (genderStr == "1") {
			std::filesystem::copy_file(file.first, genderRootDir + "/Female/" + newName);
		}
		// retrieve race
		auto raceStr = separated[2];
		if (raceStr == "0") {
			std::filesystem::copy_file(file.first, raceRootDir + "/White/" + newName);
		}
		else if (raceStr == "1") {
			std::filesystem::copy_file(file.first, raceRootDir + "/Black/" + newName);
		}
		else if (raceStr == "2") {
			std::filesystem::copy_file(file.first, raceRootDir + "/Asian/" + newName);
		}
		else if (raceStr == "3") {
			std::filesystem::copy_file(file.first, raceRootDir + "/Indian/" + newName);
		}
		else if (raceStr == "4") {
			std::filesystem::copy_file(file.first, raceRootDir + "/Other/" + newName);
		}
	}
	return 0;
}