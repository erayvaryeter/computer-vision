#include <catch2/catch.hpp>
#include <assertion/assertion.h>
#include <file/file.h>
#include <opencv2/imgcodecs.hpp>
#include <histogram/histogram.h>
#include <vector>

std::string resourceDir = base::File::GetExecutablePath() / ".." / ".." / ".." / ".." / ".." / "resource";

TEST_CASE("Calculate BGR Histogram") {
	auto histogramHandler = std::make_shared<imgproc::HistogramHandler>();
	cv::Mat image = cv::imread(resourceDir / "Lena.jpg");
	ASSERT((image.channels() == 3), "Image must be 3 channel BGR image", base::Logger::Severity::Error);
	std::vector<cv::Mat> planes;
	cv::split(image, planes);
	auto b_hist = histogramHandler->CalculateHistogram(planes[0], 256, 0, 256, true, false);
	auto g_hist = histogramHandler->CalculateHistogram(planes[1], 256, 0, 256, true, false);
	auto r_hist = histogramHandler->CalculateHistogram(planes[2], 256, 0, 256, true, false);
}