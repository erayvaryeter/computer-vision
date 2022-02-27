#include <logger/logger.h>
#include <assertion/assertion.h>

#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/base.hpp>

#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudaarithm.hpp>
#include <opencv2/cudacodec.hpp>

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

cv::cuda::GpuMat 
HistogramHandler::CalculateHistogram(cv::cuda::GpuMat& channel) {
	ASSERT(channel.channels() == 1, "Histogram calculation works only on 1 channel image, exitting ...", base::Logger::Severity::Error);
	cv::cuda::GpuMat histogram;
	cv::cuda::calcHist(channel, histogram);
	return histogram;
}

cv::Mat 
HistogramHandler::NormalizeHistogram(cv::Mat& channel, double alpha, double beta) {
	ASSERT(channel.channels() == 1, "Input histogram must have 1 channel", base::Logger::Severity::Error);
	cv::Mat normalized;
	cv::normalize(channel, normalized, alpha, beta, cv::NORM_MINMAX, -1, cv::Mat());
	return normalized;
}

cv::cuda::GpuMat 
HistogramHandler::NormalizeHistogram(cv::cuda::GpuMat& channel, double alpha, double beta) {
	ASSERT(channel.channels() == 1, "Input histogram must have 1 channel", base::Logger::Severity::Error);
	cv::cuda::GpuMat normalized;
	cv::cuda::normalize(channel, normalized, alpha, beta, cv::NORM_MINMAX, -1, cv::Mat());
	return normalized;
}

double 
HistogramHandler::CompareHistogram(cv::Mat& hist1, cv::Mat& hist2, HistogramComparisonMetrics method) {
	ASSERT(hist1.channels() == 1 && hist2.channels() == 1, "Input histograms must have 1 channel", base::Logger::Severity::Error);
	return cv::compareHist(hist1, hist2, method);
}

double 
HistogramHandler::CompareHistogram(cv::cuda::GpuMat& hist1, cv::cuda::GpuMat& hist2, HistogramComparisonMetrics method) {
	ASSERT(hist1.channels() == 1 && hist2.channels() == 1, "Input histograms must have 1 channel", base::Logger::Severity::Error);
	cv::Mat cpuHist1, cpuHist2;
	hist1.download(cpuHist1);
	hist2.download(cpuHist2);
	cpuHist1.convertTo(cpuHist1, CV_32FC1);
	cpuHist2.convertTo(cpuHist2, CV_32FC1);
	return cv::compareHist(cpuHist1, cpuHist2, method);
}

cv::Mat 
HistogramHandler::ApplyHistogramEqualization(cv::Mat& image) {
	ASSERT((image.channels() == 1 || image.channels() == 3), "Input image must have either 1 or 3 channels", base::Logger::Severity::Error);
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
		cv::cvtColor(ycrcb, dst, cv::COLOR_YCrCb2BGR);
	}
	return dst;
}

cv::cuda::GpuMat 
HistogramHandler::ApplyHistogramEqualization(cv::cuda::GpuMat& image) {
	ASSERT((image.channels() == 1 || image.channels() == 3), "Input image must have either 1 or 3 channels", base::Logger::Severity::Error);
	cv::cuda::GpuMat dst;
	// gray image
	if (image.channels() == 1) {
		cv::cuda::equalizeHist(image, dst);
	}
	// color image
	else {
		cv::cuda::GpuMat ycrcb;
		cv::cuda::cvtColor(image, ycrcb, cv::COLOR_BGR2YCrCb);
		std::vector<cv::cuda::GpuMat> channels;
		cv::cuda::split(ycrcb, channels);
		cv::cuda::equalizeHist(channels[0], channels[0]);
		cv::cuda::merge(channels, ycrcb);
		cv::cuda::cvtColor(ycrcb, dst, cv::COLOR_YCrCb2BGR);
	}
	return dst;
}

cv::Mat 
HistogramHandler::ApplyAdaptiveHistogramEqualization(cv::Mat& image, double clipLimit) {
	ASSERT((image.channels() == 1 || image.channels() == 3), "Input image must have either 1 or 3 channels", base::Logger::Severity::Error);
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

cv::cuda::GpuMat 
HistogramHandler::ApplyAdaptiveHistogramEqualization(cv::cuda::GpuMat& image, double clipLimit) {
	ASSERT((image.channels() == 1 || image.channels() == 3), "Input image must have either 1 or 3 channels", base::Logger::Severity::Error);
	cv::cuda::GpuMat dst;
	cv::Ptr<cv::cuda::CLAHE> clahe = cv::cuda::createCLAHE();
	clahe->setClipLimit(clipLimit);

	// gray image
	if (image.channels() == 1) {
		clahe->apply(image, dst);
	}
	else {
		cv::cuda::GpuMat labImage;
		cv::cuda::cvtColor(image, labImage, cv::COLOR_BGR2Lab);
		// Extract the L channel
		std::vector<cv::cuda::GpuMat> lab_planes(3);
		cv::cuda::split(labImage, lab_planes);
		// apply the CLAHE algorithm to the L channel
		cv::cuda::GpuMat destination;
		clahe->apply(lab_planes[0], destination);
		// Merge the the color planes back into an Lab image
		destination.copyTo(lab_planes[0]);
		cv::cuda::merge(lab_planes, labImage);
		cv::cuda::cvtColor(labImage, dst, cv::COLOR_Lab2BGR);
	}
	return dst;
}

}