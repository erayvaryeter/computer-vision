#include <qr-barcode-reader/qr-barcode-reader.h>
#include <cxxopts.hpp>
#include <file/file.h>
#include <assertion/assertion.h>

int main(int argc, char** argv) {
	cxxopts::Options options("Feature Detector");
	options.add_options()
		("image1", "First image path", cxxopts::value<std::string>()->default_value("../../../../helper/qr-barcode-reader/resource/Code.jpg"))
		("h,help", "Print usage");

	auto result = options.parse(argc, argv);
	if (result.count("help")) {
		std::cout << options.help() << std::endl;
		exit(0);
	}

	std::string image1Path;

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

	cv::Mat image1 = cv::imread(image1Path.c_str());
	ASSERT(!image1.empty(), "Image could not be read", base::Logger::Severity::Error);
	
	auto reader = std::make_shared<helper::QrBarcodeReader>();
	auto res = reader->ReadBarcode(image1);
	auto resImage = reader->DrawResult(image1, res);
	cv::imshow("Result Image", resImage);
	cv::waitKey(0);

	return 0;
}