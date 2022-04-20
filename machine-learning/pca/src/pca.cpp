#include <logger/logger.h>
#include <assertion/assertion.h>
#include <file/file.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/core_c.h>
#include "pca/pca.h"

std::shared_ptr<base::Logger> ml::PCA::m_logger = std::make_shared<base::Logger>();

namespace ml {

void
PCA::AppendImage(cv::Mat image) {
    m_inputImages.emplace_back(std::move(image));
}

void
PCA::ClearImages() {
    m_inputImages.clear();
}

cv::Mat
PCA::ConvertToRowMatrix(std::vector<cv::Mat>& src, int rtype, double alpha, double beta) {
    size_t n = src.size();
    if (n == 0)
        return cv::Mat();
    size_t d = src[0].total();
    cv::Mat data(n, d, rtype);
    for (int i = 0; i < n; i++) {
        if (src[i].empty()) {
            std::string errorMessage = cv::format("Image number %d was empty, please check your input data.", i);
            ASSERT(true, errorMessage.c_str(), base::Logger::Severity::Error);
        }
        if (src[i].total() != d) {
            std::string errorMessage = cv::format("Wrong number of elements in matrix #%d! Expected %d was %d.", i, d, src[i].total());
            ASSERT(true, errorMessage.c_str(), base::Logger::Severity::Error);
        }
        cv::Mat xi = data.row(i);
        if (src[i].isContinuous())
            src[i].reshape(1, 1).convertTo(xi, rtype, alpha, beta);
        else
            src[i].clone().reshape(1, 1).convertTo(xi, rtype, alpha, beta);
     }
    return data;
}

cv::Mat
PCA::Normalize(cv::Mat& src) {
    cv::Mat dst;
    switch (src.channels()) {
    case 1:
        cv::normalize(src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC1);
        break;
    case 3:
        cv::normalize(src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC3);
        break;
    default:
        src.copyTo(dst);
        break;
    }
    return dst;
}

void
PCA::ApplyPCA(int numComponents) {
    // Prepare grayscale images
    m_grayscaleImages.clear();
    for (auto im : m_inputImages) {
        if (im.channels() == 1) {
            m_grayscaleImages.emplace_back(std::move(im));
        }
        else {
            cv::Mat grayscale;
            cv::cvtColor(im, grayscale, cv::COLOR_BGR2GRAY);
            m_grayscaleImages.emplace_back(std::move(grayscale));
        }
    }
    // Get data as row
    m_rowMatrix.release();
    m_rowMatrix = ConvertToRowMatrix(m_grayscaleImages, CV_32FC1);
    // Perform a PCA:
    cv::PCA pca(m_rowMatrix, cv::Mat(), CV_PCA_DATA_AS_ROW, numComponents);

    // And copy the PCA results:
    cv::Mat mean = pca.mean.clone();
    cv::Mat eigenvalues = pca.eigenvalues.clone();
    cv::Mat eigenvectors = pca.eigenvectors.clone();
    m_transformationMatrix.release();
    cv::transpose(eigenvectors, m_transformationMatrix);
}

std::vector<cv::Mat>
PCA::GetPrincipalComponents() {
    std::vector<cv::Mat> principalComponents;
    for (size_t i = 0; i < m_rowMatrix.size().height; ++i) {
        cv::Mat data = m_rowMatrix.row(i);
        cv::Mat y = data * m_transformationMatrix;
        principalComponents.emplace_back(std::move(y));
    }
    return principalComponents;
}

}