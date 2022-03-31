#include <feature-detector/feature-detector.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <file/file.h>

int main() {
	auto imagePath = base::File::GetExecutableDirectory() / ".." / ".." / ".." / ".." / "features" / "feature-detector" / "resource" / "Chessboard.jpg";
	cv::Mat image = cv::imread(imagePath);
	auto featureDetector = std::make_shared<features::FeatureDetector>();
	featureDetector->ApplyHarrisCornerDetection(image, 2, 3, 0.04, 125);
	auto features = featureDetector->GetFeatures();
	auto finalImage = featureDetector->GetImageWithFeatures();
	cv::imwrite("FinalImageWithFeatures.jpg", finalImage);
	cv::imshow("Final Image", finalImage);
	return 0;
}