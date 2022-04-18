#include <logger/logger.h>
#include <assertion/assertion.h>
#include <file/file.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "feature-matcher/feature-matcher.h"

std::shared_ptr<base::Logger> features::FeatureMatcher::m_logger = std::make_shared<base::Logger>();

namespace features {

void 
FeatureMatcher::ApplyMatching(cv::Mat queryDescriptors, cv::Mat trainDescriptors) {
    m_matches.clear();
    m_matcher->match(queryDescriptors, trainDescriptors, m_matches);
}

void
FeatureMatcher::ApplyKnnMatching(cv::Mat queryDescriptors, cv::Mat trainDescriptors, int k, float ratioThreshold) {
	std::vector<std::vector<cv::DMatch>> knnMatches;
	m_matcher->knnMatch(queryDescriptors, trainDescriptors, knnMatches, k);
    m_matches.clear();
    for (size_t i = 0; i < knnMatches.size(); i++) {
        if (knnMatches[i][0].distance < ratioThreshold * knnMatches[i][1].distance) {
            m_matches.push_back(knnMatches[i][0]);
        }
    }
}

void 
FeatureMatcher::ApplyRadiusMatching(cv::Mat queryDescriptors, cv::Mat trainDescriptors, float maxHammingDistance, float ratioThreshold) {
    std::vector<std::vector<cv::DMatch>> radiusMatches;
    if (maxHammingDistance == -1.0f)
        maxHammingDistance = static_cast<float>(trainDescriptors.size().width) * 0.05f;
    m_matcher->radiusMatch(queryDescriptors, trainDescriptors, radiusMatches, maxHammingDistance);
    m_matches.clear();
    for (size_t i = 0; i < radiusMatches.size(); i++) {
        if (radiusMatches[i][0].distance < ratioThreshold * radiusMatches[i][1].distance) {
            m_matches.push_back(radiusMatches[i][0]);
        }
    }
}

}