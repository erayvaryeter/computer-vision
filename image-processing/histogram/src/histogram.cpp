#include <logger/logger.h>
#include <assertion/assertion.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/base.hpp>

#include "histogram/histogram.h"

std::shared_ptr<base::Logger> imgproc::HistogramHandler::m_logger = std::make_shared<base::Logger>();

namespace imgproc {

cv::Mat
HistogramHandler::CalculateHistogram(cv::Mat& channel, const int histSize, float rangeMin, float rangeMax, bool uniform, bool accumulate) {
	ASSERT(channel.channels() == 1, "Histogram calculation works only on 1 channel image, exitting ...", base::Logger::Severity::Error);
	cv::Mat histogram;
	float range[] = { rangeMin, rangeMax }; 
	const float* histRange[] = { range };
	cv::calcHist(&channel, 1, 0, cv::Mat(), histogram, 1, &histSize, histRange, uniform, accumulate);
	return histogram;
}

cv::Mat 
HistogramHandler::NormalizeHistogram(cv::Mat& channel, double alpha, double beta) {
	ASSERT(channel.channels() == 1, "Input histogram must have 1 channel", base::Logger::Severity::Error);
	cv::Mat normalized;
	cv::normalize(channel, normalized, alpha, beta, cv::NORM_MINMAX, -1, cv::Mat());
	return normalized;
}

double 
HistogramHandler::CompareHistogram(cv::Mat& hist1, cv::Mat& hist2, HistogramComparisonMetrics method) {
	ASSERT(hist1.channels() == 1 && hist2.channels() == 1, "Input histograms must have 1 channel", base::Logger::Severity::Error);
	return cv::compareHist(hist1, hist2, method);
}

cv::Mat 
HistogramHandler::ApplyHistogramEqualization(cv::Mat& image) {
	ASSERT(image.channels() == 1 || image.channels() >= 3, "Input image must have either 1 or 3 channels", base::Logger::Severity::Error);
	cv::Mat dst;
	// gray image
	if (image.channels() == 1) {
		cv::equalizeHist(image, dst);
	}
	// color image
	else {
		cv::Mat ycrcb;
		cv::cvtColor(image, ycrcb, cv::COLOR_BGR2YCrCb);
		std::vector<cv::Mat> channels;
		cv::split(ycrcb, channels);
		cv::equalizeHist(channels[0], channels[0]);
		cv::merge(channels, ycrcb);
		cvtColor(ycrcb, dst, cv::COLOR_YCrCb2BGR);
	}
	return dst;
}

cv::Mat 
HistogramHandler::ApplyHistogramEqualization(cv::Mat& image, double clipLimit) {
	ASSERT(image.channels() == 1 || image.channels() >= 3, "Input image must have either 1 or 3 channels", base::Logger::Severity::Error);
	cv::Mat dst;
	cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
	clahe->setClipLimit(clipLimit);

	// gray image
	if (image.channels() == 1) {
		clahe->apply(image, dst);
	}
	else {
		cv::Mat labImage;
		cv::cvtColor(image, labImage, cv::COLOR_BGR2Lab);
		// Extract the L channel
		std::vector<cv::Mat> lab_planes(3);
		cv::split(labImage, lab_planes);
		// apply the CLAHE algorithm to the L channel
		cv::Mat destination;
		clahe->apply(lab_planes[0], destination);
		// Merge the the color planes back into an Lab image
		destination.copyTo(lab_planes[0]);
		cv::merge(lab_planes, labImage);
		cv::cvtColor(labImage, dst, cv::COLOR_Lab2BGR);
	}
	return dst;
}

}
