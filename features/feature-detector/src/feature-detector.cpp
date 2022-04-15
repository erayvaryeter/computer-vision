#include <logger/logger.h>
#include <assertion/assertion.h>
#include <file/file.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "feature-detector/feature-detector.h"

std::shared_ptr<base::Logger> features::FeatureDetector::m_logger = std::make_shared<base::Logger>();

namespace features {

void 
FeatureDetector::ApplyHarrisCornerDetection(cv::Mat image, int blockSize, int apertureSize, double k, float thresh) {
    cv::Mat destination = cv::Mat::zeros(image.size(), CV_32FC1);
    m_lastImageWithFeatures = image.clone();
    if (image.channels() != 1)
        cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
    cv::cornerHarris(image, destination, blockSize, apertureSize, k);
    cv::Mat destinationNormalized;
    cv::normalize(destination, destinationNormalized, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat());
    m_features.clear();

    for (int i = 0; i < destinationNormalized.rows; i++) {
        for (int j = 0; j < destinationNormalized.cols; j++) {
            if (destinationNormalized.at<float>(i, j) > thresh) {
                cv::circle(m_lastImageWithFeatures, cv::Point(j, i), 2, cv::Scalar(255), 2, 8, 0);
                auto feature = cv::KeyPoint(static_cast<float>(i), static_cast<float>(j), 1.0f);
                m_features.emplace_back(std::move(feature));
            }
        }
    }
}

void 
FeatureDetector::ApplyShiTomasiCornerDetection(cv::Mat image, int maxCorners, double qualityLevel, double minDistance, int blockSize, int gradientSize, 
    bool useHarris, double k) {
    cv::Mat destination = cv::Mat::zeros(image.size(), CV_32FC1);
    m_lastImageWithFeatures = image.clone();
    if (image.channels() != 1)
        cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
    
    std::vector<cv::Point2f> corners;
    cv::goodFeaturesToTrack(
        image,
        corners,
        maxCorners,
        qualityLevel,
        minDistance,
        cv::Mat(),
        blockSize,
        gradientSize,
        useHarris,
        k
    );

    m_features.clear();

    for (const auto& corner : corners) {
        cv::circle(m_lastImageWithFeatures, cv::Point(corner.x, corner.y), 2, cv::Scalar(255), 2, 8, 0);
        auto feature = cv::KeyPoint(corner.x, corner.y, 1.0f);
        m_features.emplace_back(std::move(feature));
    }
}

}