#include <iostream>

#include "REGULAR.h"
#include "KNN.h"
#include "RADIUS.h"

enum class MatchingType {
	REGULAR_MATCHING = 0,
	KNN_MATCHING = 1,
	RADIUS_MATCHING = 2
};

MatchingType
ConvertMatchingType(std::string matchingType) {
	if (matchingType == "Regular") {
		return MatchingType::REGULAR_MATCHING;
	}
	else if (matchingType == "Knn") {
		return MatchingType::KNN_MATCHING;
	}
	else if (matchingType == "Radius") {
		return MatchingType::RADIUS_MATCHING;
	}
	else {
		std::cout << "Given matching type string is invalid, exitting" << std::endl;
		exit(0);
	}
}

int main(int argc, char** argv) {
	cxxopts::Options options("Feature Detector");
	options.add_options()
		("image1", "First image path", cxxopts::value<std::string>()->default_value("../../../../features/feature-matcher/resource/1.jpg"))
		("image2", "Second image path", cxxopts::value<std::string>()->default_value("../../../../features/feature-matcher/resource/2.jpg"))
		("matching-type", "Types: Regular - Knn - Radius", cxxopts::value<std::string>()->default_value("Regular"))
		("h,help", "Print usage");

	auto result = options.parse(argc, argv);
	if (result.count("help")) {
		std::cout << options.help() << std::endl;
		exit(0);
	}

	std::string image1Path, image2Path;

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

	MatchingType mt;
	mt = ConvertMatchingType(result["matching-type"].as<std::string>());

	switch (mt) {
	case MatchingType::REGULAR_MATCHING:
	{
		auto matcher = std::make_shared<RegularMatching>(image1Path, image2Path);
		matcher->Run();
		break;
	}
	case MatchingType::KNN_MATCHING:
	{
		auto matcher = std::make_shared<KnnMatching>(image1Path, image2Path);
		matcher->Run();
		break;
	}
	case MatchingType::RADIUS_MATCHING:
	{
		auto matcher = std::make_shared<RadiusMatching>(image1Path, image2Path);
		matcher->Run();
		break;
	}
	default: break;
	}

	return 0;
}