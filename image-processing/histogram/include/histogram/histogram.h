#pragma once

#include <memory>
#include <opencv2/core/mat.hpp>

namespace base {
	class Logger;
}

namespace imgproc {

enum HistogramComparisonMetrics {
	CORRELATION = 0,
	CHI_SQUARE = 1,
	INTERSECTION = 2,
	BHATTACHARYYA_DISTANCE = 3
};

class HistogramHandler {
public:

	cv::Mat CalculateHistogram(cv::Mat& channel, int histSize = 256, float rangeMin = 0, float rangeMax = 255, bool uniform = true, bool accumulate = false);
	cv::Mat NormalizeHistogram(cv::Mat& channel, double alpha = 0, double beta = 1);
	double CompareHistogram(cv::Mat& hist1, cv::Mat& hist2, HistogramComparisonMetrics method = CORRELATION);
	cv::Mat ApplyHistogramEqualization(cv::Mat& image);

private:
	static std::shared_ptr<base::Logger> m_logger;
};

}