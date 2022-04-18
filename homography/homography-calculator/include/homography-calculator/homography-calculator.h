#pragma once

#include <feature-matcher/feature-matcher.h>
#include <opencv2/opencv.hpp>

namespace base {
	class Logger;
}

namespace homography {

class HomographyCalculator {
public:
	HomographyCalculator() {}
	~HomographyCalculator() {}

	void AddPoints(std::vector<cv::KeyPoint> srcKeypoints, std::vector<cv::KeyPoint> dstKeypoints,
		std::vector<cv::DMatch> goodMatches);
	void AddPoints(std::vector<cv::Point2f> srcPts, std::vector<cv::Point2f> dstPts);
	void FindHomography();
	cv::Mat FindHomography(cv::Mat image1, cv::Mat image2);

private:
	cv::Mat m_homography;
	std::vector<cv::Point2f> m_sourcePoints;
	std::vector<cv::Point2f> m_destinationPoints;
	static std::shared_ptr<base::Logger> m_logger;
};

}