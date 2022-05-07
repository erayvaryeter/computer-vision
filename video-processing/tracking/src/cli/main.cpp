#include <tracking/tracking.h>
#include <cxxopts.hpp>
#include <file/file.h>
#include <assertion/assertion.h>

int main(int argc, char** argv) {
	cxxopts::Options options("Feature Detector");
	options.add_options()
		("image1", "First image path", cxxopts::value<std::string>()->default_value("../../../../homography/stitcher/resource/1.jpg"))
		("image2", "Second image path", cxxopts::value<std::string>()->default_value("../../../../homography/stitcher/resource/2.jpg"))
		("image3", "Third image path", cxxopts::value<std::string>()->default_value("../../../../homography/stitcher/resource/3.jpg"))
		("stitcher-type", "Types: Panorama - Scans", cxxopts::value<std::string>()->default_value("Regular"))
		("h,help", "Print usage");

	auto result = options.parse(argc, argv);
	if (result.count("help")) {
		std::cout << options.help() << std::endl;
		exit(0);
	}

	std::string image1Path, image2Path, image3Path;

	if (result.count("image1")) {
		image1Path = result["image1"].as<std::string>();
		if (!base::File::FileExists(image1Path)) {
			std::cout << "Image with given path does not exist" << std::endl;
			exit(0);
		}
		auto ext = base::File::GetFileExtension(image1Path);
		if (ext != ".jpg" && ext != ".jpeg" && ext != ".png" && ext != ".bmp") {
			std::cout << "Given image path extension is not valid" << std::endl;
			exit(0);
		}
	}
	else {
		image1Path = result["image1"].as<std::string>();
	}

	if (result.count("image2")) {
		image2Path = result["image2"].as<std::string>();
		if (!base::File::FileExists(image2Path)) {
			std::cout << "Image with given path does not exist" << std::endl;
			exit(0);
		}
		auto ext = base::File::GetFileExtension(image2Path);
		if (ext != ".jpg" && ext != ".jpeg" && ext != ".png" && ext != ".bmp") {
			std::cout << "Given image path extension is not valid" << std::endl;
			exit(0);
		}
	}
	else {
		image2Path = result["image2"].as<std::string>();
	}

	if (result.count("image3")) {
		image3Path = result["image3"].as<std::string>();
		if (!base::File::FileExists(image3Path)) {
			std::cout << "Image with given path does not exist" << std::endl;
			exit(0);
		}
		auto ext = base::File::GetFileExtension(image3Path);
		if (ext != ".jpg" && ext != ".jpeg" && ext != ".png" && ext != ".bmp") {
			std::cout << "Given image path extension is not valid" << std::endl;
			exit(0);
		}
	}
	else {
		image3Path = result["image3"].as<std::string>();
	}

	return 0;
}