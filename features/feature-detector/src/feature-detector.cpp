#include <logger/logger.h>
#include <assertion/assertion.h>
#include <file/file.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/core_c.h>

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
    m_keypoints.clear();

    for (int i = 0; i < destinationNormalized.rows; i++) {
        for (int j = 0; j < destinationNormalized.cols; j++) {
            if (destinationNormalized.at<float>(i, j) > thresh) {
                auto feature = cv::KeyPoint(static_cast<float>(j), static_cast<float>(i), 1.0f);
                m_keypoints.emplace_back(std::move(feature));
            }
        }
    }

    if (m_keypoints.size() <= 2000 && m_keypoints.size() >= 1) {
        m_descriptor.release();
        m_principalComponents.release();
        m_briskDescriptorExtractor->compute(image, m_keypoints, m_descriptor);
        if (!m_descriptor.empty())
            cv::reduce(m_descriptor, m_principalComponents, 0, CV_REDUCE_AVG, CV_32F);
    }

    m_lastImageWithKeypoints.release();
    cv::drawKeypoints(image, m_keypoints, m_lastImageWithKeypoints);
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

    m_keypoints.clear();

    for (const auto& corner : corners) {
        auto feature = cv::KeyPoint(corner.x, corner.y, 1.0f);
        m_keypoints.emplace_back(std::move(feature));
    }

    if (m_keypoints.size() <= 2000 && m_keypoints.size() >= 1) {
        m_descriptor.release();
        m_principalComponents.release();
        m_briskDescriptorExtractor->compute(image, m_keypoints, m_descriptor);
        if (!m_descriptor.empty())
            cv::reduce(m_descriptor, m_principalComponents, 0, CV_REDUCE_AVG, CV_32F);
    }

    m_lastImageWithKeypoints.release();
    cv::drawKeypoints(image, m_keypoints, m_lastImageWithKeypoints);
}

void
FeatureDetector::ApplySIFT(cv::Mat image, int nFeatures, int nOctaveLayers, double contrastThreshold, double edgeThreshold, double sigma) {
    auto siftPtr = cv::SIFT::create(nFeatures, nOctaveLayers, contrastThreshold, edgeThreshold, sigma);
    m_keypoints.clear();
    m_descriptor.release();
    m_principalComponents.release();
    siftPtr->detectAndCompute(image, cv::Mat(), m_keypoints, m_descriptor);
    if (!m_descriptor.empty())
        cv::reduce(m_descriptor, m_principalComponents, 0, CV_REDUCE_AVG, CV_32F);
    m_lastImageWithKeypoints.release();
    cv::drawKeypoints(image, m_keypoints, m_lastImageWithKeypoints);
}

void
FeatureDetector::ApplySURF(cv::Mat image, double hessianThreshold, int nOctaves, int nOctaveLayers, bool extended, bool upright) {
    auto surfPtr = cv::xfeatures2d::SURF::create(hessianThreshold, nOctaves, nOctaveLayers, extended, upright);
    m_keypoints.clear();
    m_descriptor.release();
    surfPtr->detectAndCompute(image, cv::Mat(), m_keypoints, m_descriptor);
    m_lastImageWithKeypoints.release();
    cv::drawKeypoints(image, m_keypoints, m_lastImageWithKeypoints);
}

void 
FeatureDetector::ApplyFAST(cv::Mat image, int threshold, bool nonmaxSupression) {
    auto fastPtr = cv::FastFeatureDetector::create(threshold, false);
    auto briskPtr = cv::BRISK::create(); // only for computing descriptor
    m_keypoints.clear();
    fastPtr->detect(image, m_keypoints);
    m_lastImageWithKeypoints.release();
    cv::drawKeypoints(image, m_keypoints, m_lastImageWithKeypoints);
    m_descriptor.release();
    briskPtr->compute(image, m_keypoints, m_descriptor);
}

void 
FeatureDetector::ApplyBRIEF(cv::Mat image, int bytes, bool useOrientation) {
    auto briefPtr = cv::xfeatures2d::BriefDescriptorExtractor::create(bytes, useOrientation);
    m_keypoints.clear();
    m_siftKeypointDetector->detect(image, m_keypoints);
    if (m_keypoints.size() <= 2000) {
        m_descriptor.release();
        briefPtr->compute(image, m_keypoints, m_descriptor);
    }
    m_lastImageWithKeypoints.release();
    cv::drawKeypoints(image, m_keypoints, m_lastImageWithKeypoints);
}

void 
FeatureDetector::ApplyORB(cv::Mat image, int nFeatures, float scaleFactor, int nLevels, int edgeThreshold, int firstLevel, 
    int WTA_K, cv::ORB::ScoreType st, int patchSize, int fastThreshold) {
    auto orbPtr = cv::ORB::create(nFeatures, scaleFactor, nLevels, edgeThreshold, firstLevel, WTA_K, st, patchSize, fastThreshold);
    m_keypoints.clear();
    m_descriptor.release();
    orbPtr->detectAndCompute(image, cv::Mat(), m_keypoints, m_descriptor);
    m_lastImageWithKeypoints.release();
    cv::drawKeypoints(image, m_keypoints, m_lastImageWithKeypoints);
}

void 
FeatureDetector::ApplyBRISK(cv::Mat image, int thresh, int octaves, float patternScale) {
    auto briskPtr = cv::BRISK::create(thresh, octaves, patternScale);
    m_keypoints.clear();
    m_descriptor.release();
    briskPtr->detectAndCompute(image, cv::Mat(), m_keypoints, m_descriptor);
    m_lastImageWithKeypoints.release();
    cv::drawKeypoints(image, m_keypoints, m_lastImageWithKeypoints);
}

}