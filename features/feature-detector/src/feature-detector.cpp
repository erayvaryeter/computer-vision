#include <logger/logger.h>
#include <assertion/assertion.h>
#include <file/file.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include "feature-detector/feature-detector.h"

std::shared_ptr<base::Logger> features::FeatureDetector::m_logger = std::make_shared<base::Logger>();

namespace features {

void 
FeatureDetector::ApplyHarrisCornerDetection(cv::Mat image, int blockSize, int apertureSize, double k, float thresh) {
    cv::Mat destination = cv::Mat::zeros(image.size(), CV_32FC1);
    cv::Mat grayImage;
    if (image.channels() != 1)
        cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
    else
        image.copyTo(grayImage);
    cv::cornerHarris(grayImage, destination, blockSize, apertureSize, k);
    cv::Mat destinationNormalized;
    cv::normalize(destination, destinationNormalized, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat());
    m_features.clear();

    for (int i = 0; i < destinationNormalized.rows; i++) {
        for (int j = 0; j < destinationNormalized.cols; j++) {
            if (destinationNormalized.at<float>(i, j) > thresh) {
                auto feature = cv::KeyPoint(static_cast<float>(j), static_cast<float>(i), 1.0f);
                m_features.emplace_back(std::move(feature));
            }
        }
    }

    m_lastImageWithFeatures.release();
    cv::drawKeypoints(image, m_features, m_lastImageWithFeatures);
}

void 
FeatureDetector::ApplyShiTomasiCornerDetection(cv::Mat image, int maxCorners, double qualityLevel, double minDistance, int blockSize, int gradientSize, 
    bool useHarris, double k) {
    cv::Mat destination = cv::Mat::zeros(image.size(), CV_32FC1);
    cv::Mat grayImage;
    if (image.channels() != 1)
        cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
    else
        image.copyTo(grayImage);

    std::vector<cv::Point2f> corners;
    cv::goodFeaturesToTrack(
        grayImage,
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
        auto feature = cv::KeyPoint(corner.x, corner.y, 1.0f);
        m_features.emplace_back(std::move(feature));
    }

    m_lastImageWithFeatures.release();
    cv::drawKeypoints(image, m_features, m_lastImageWithFeatures);
}

void
FeatureDetector::ApplySIFT(cv::Mat image, int nFeatures, int nOctaveLayers, double contrastThreshold, double edgeThreshold, double sigma) {
    cv::Ptr<cv::SIFT> siftPtr = cv::SIFT::create(nFeatures, nOctaveLayers, contrastThreshold, edgeThreshold, sigma);
    m_features.clear();
    siftPtr->detect(image, m_features);
    m_lastImageWithFeatures.release();
    cv::drawKeypoints(image, m_features, m_lastImageWithFeatures);
}

void
FeatureDetector::ApplySURF(cv::Mat image, double hessianThreshold, int nOctaves, int nOctaveLayers, bool extended, bool upright) {
    cv::Ptr<cv::xfeatures2d::SURF> surfPtr = cv::xfeatures2d::SURF::create(hessianThreshold, nOctaves, nOctaveLayers, extended, upright);
    m_features.clear();
    surfPtr->detect(image, m_features);
    m_lastImageWithFeatures.release();
    cv::drawKeypoints(image, m_features, m_lastImageWithFeatures);
}

}