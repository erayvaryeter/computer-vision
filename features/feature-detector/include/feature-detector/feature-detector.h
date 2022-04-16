#pragma once

#include <memory>
#include <opencv2/core/mat.hpp>
#include <opencv2/features2d/features2d.hpp>

namespace base {
	class Logger;
}

namespace features {

class FeatureDetector {
public:
	FeatureDetector() {}
	~FeatureDetector() {}

	std::vector<cv::KeyPoint> GetFeatures() { return m_features; }
	cv::Mat GetImageWithFeatures() { return m_lastImageWithFeatures; }

	void ApplyHarrisCornerDetection(cv::Mat image, int blockSize = 2, int apertureSize = 3, double k = 0.04, float thresh = 200.f);
	void ApplyShiTomasiCornerDetection(cv::Mat image, int maxCorners = 25, double qualityLevel = 0.01, double minDistance = 10, int blockSize = 3, 
		int gradientSize = 3, bool useHarris = false, double k = 0.04);
	void ApplySIFT(cv::Mat image, int nFeatures = 0, int nOctaveLayers = 3, double contrastThreshold = 0.04, double edgeThreshold = 10.0, 
		double sigma = 1.60);

private:
	std::vector<cv::KeyPoint> m_features;
	cv::Mat m_lastImageWithFeatures;
	static std::shared_ptr<base::Logger> m_logger;
};

}