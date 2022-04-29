#pragma once

#include <memory>
#include <opencv2/core/mat.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

namespace base {
	class Logger;
}

namespace features {

class FeatureDetector {
public:
	FeatureDetector() { 
		m_briskDescriptorExtractor = cv::BRISK::create(); 
		m_siftKeypointDetector = cv::SIFT::create();
	}
	~FeatureDetector() {}

	std::vector<cv::KeyPoint> GetKeypoints() { return m_keypoints; }
	cv::Mat GetDescriptors() { return m_descriptor; }
	cv::Mat GetPrincipalComponents() { return m_principalComponents; }
	cv::Mat GetImageWithKeypoints() { return m_lastImageWithKeypoints; }

	void ApplyHarrisCornerDetection(cv::Mat image, int blockSize = 2, int apertureSize = 3, double k = 0.04, float thresh = 200.f);
	void ApplyShiTomasiCornerDetection(cv::Mat image, int maxCorners = 25, double qualityLevel = 0.01, double minDistance = 10, int blockSize = 3, 
		int gradientSize = 3, bool useHarris = false, double k = 0.04);
	void ApplySIFT(cv::Mat image, int nFeatures = 0, int nOctaveLayers = 3, double contrastThreshold = 0.04, double edgeThreshold = 10.0, 
		double sigma = 1.60);
	void ApplySURF(cv::Mat image, double hessianThreshold = 100, int nOctaves = 4, int nOctaveLayers = 3, bool extended = false, 
		bool upright = false);
	void ApplyFAST(cv::Mat image, int threshold = 10, bool nonmaxSupression = true);
	void ApplyBRIEF(cv::Mat image, int bytes = 32, bool useOrientation = false);
	void ApplyORB(cv::Mat image, int nFeatures = 500, float scaleFactor = 1.2f, int nLevels = 8, int edgeThreshold = 31, int firstLevel = 0,
		int WTA_K = 2, cv::ORB::ScoreType st = cv::ORB::ScoreType::HARRIS_SCORE, int patchSize = 31, int fastThreshold = 20);
	void ApplyBRISK(cv::Mat image, int thresh = 30, int octaves = 3, float patternScale = 1.0f);

private:
	cv::Ptr<cv::BRISK> m_briskDescriptorExtractor;
	cv::Ptr<cv::SIFT> m_siftKeypointDetector;

	std::vector<cv::KeyPoint> m_keypoints;
	cv::Mat m_descriptor;
	cv::Mat m_principalComponents;
	cv::Mat m_lastImageWithKeypoints;
	static std::shared_ptr<base::Logger> m_logger;
};

}