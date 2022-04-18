#include <logger/logger.h>
#include <assertion/assertion.h>
#include <file/file.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "homography-calculator/homography-calculator.h"

std::shared_ptr<base::Logger> homography::HomographyCalculator::m_logger = std::make_shared<base::Logger>();

namespace homography {

void
HomographyCalculator::AddPoints(std::vector<cv::KeyPoint> srcKeypoints, std::vector<cv::KeyPoint> dstKeypoints,
	std::vector<cv::DMatch> goodMatches) {
	ASSERT((srcKeypoints.size() >= 4), "Number of source keypoints must be at least 4", base::Logger::Severity::Error);
	ASSERT((dstKeypoints.size() >= 4), "Number of destination keypoints must be at least 4", base::Logger::Severity::Error);
	ASSERT((goodMatches.size() >= 4), "Number of good matches must be at least 4", base::Logger::Severity::Error);
	m_sourcePoints.clear();
	m_destinationPoints.clear();
	for (auto& match : goodMatches) {
		ASSERT((srcKeypoints.size() >= match.queryIdx), "Query index out of range", base::Logger::Severity::Error);
		ASSERT((dstKeypoints.size() >= match.trainIdx), "Train index out of range", base::Logger::Severity::Error);
		auto srcPt = srcKeypoints[match.queryIdx].pt;
		auto dstPt = dstKeypoints[match.trainIdx].pt;
		m_sourcePoints.emplace_back(std::move(srcPt));
		m_destinationPoints.emplace_back(std::move(dstPt));
	}
}

void
HomographyCalculator::AddPoints(std::vector<cv::Point2f> srcPts, std::vector<cv::Point2f> dstPts) {
	ASSERT((m_sourcePoints.size() >= 4), "Number of source points must be at least 4", base::Logger::Severity::Error);
	ASSERT((m_destinationPoints.size() >= 4), "Number of destination points must be at least 4", base::Logger::Severity::Error);
	m_sourcePoints.clear();
	m_destinationPoints.clear();
	m_sourcePoints = srcPts;
	m_destinationPoints = dstPts;
}

void 
HomographyCalculator::FindHomography() {
	m_homography.release();
	ASSERT((m_sourcePoints.size() >= 4), "Size of source points must be at least 4", base::Logger::Severity::Error);
	ASSERT((m_destinationPoints.size() >= 4), "Size of destination points must be at least 4", base::Logger::Severity::Error);
	m_homography = cv::findHomography(m_sourcePoints, m_destinationPoints);
}

cv::Mat 
HomographyCalculator::FindHomography(cv::Mat image1, cv::Mat image2) {
	cv::Mat retVal;
	m_homography.release();
	ASSERT((m_sourcePoints.size() >= 4), "Size of source points must be at least 4", base::Logger::Severity::Error);
	ASSERT((m_destinationPoints.size() >= 4), "Size of destination points must be at least 4", base::Logger::Severity::Error);
	m_homography = cv::findHomography(m_sourcePoints, m_destinationPoints);
	cv::warpPerspective(image1, retVal, m_homography, image2.size());
	return retVal;
}

}