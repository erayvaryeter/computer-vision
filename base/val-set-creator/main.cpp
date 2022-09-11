#include <iostream>
#include <file/file.h>
#include <string/string.h>
#include <filesystem>

int main() {
	std::string baseDir = "D:/Datasets/Race/";
	std::string trainDir = baseDir + "train/";
	std::string valDir = baseDir + "val/";
	auto classDirectories = base::File::GetDirectories(trainDir);
	
	for (auto& classDirectory : classDirectories) {
		auto splitted = base::String::SplitString(classDirectory, "/");
		auto label = splitted[splitted.size() - 1];
		auto files = base::File::GetFilesWithExtension(classDirectory);
		for (size_t i = 0; i < files.size(); ++i) {
			if (i % 5 == 0) {
				auto path = files[i];
				std::string validationDirectory = valDir + label;
				if (!base::File::DirectoryExists(validationDirectory)) {
					base::File::Create_Directory(validationDirectory);
				}
				std::string validationPath = validationDirectory + "/" + path.second + ".jpg";
				std::filesystem::copy(path.first, validationPath);
				base::File::Remove_File(path.first);
			}
		}
	}

	return 0;
}