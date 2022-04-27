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
PCA::Normalize(cv::Mat& src) {
    cv::Mat dst;
    switch (src.channels()) {
    case 1:
        cv::normalize(src, dst, -1, 1, cv::NORM_MINMAX);
        break;
    case 3:
        cv::normalize(src, dst, -1, 1, cv::NORM_MINMAX);
        break;
    default:
        src.copyTo(dst);
        break;
    }
    return dst;
}

cv::Mat
PCA::FormatImagesForPCA(const std::vector<cv::Mat>& data) {
    cv::Mat dst(static_cast<int>(data.size()), data[0].rows * data[0].cols, CV_32F);
    for (unsigned int i = 0; i < data.size(); i++) {
        cv::Mat imageRow = data[i].clone().reshape(1, 1);
        cv::Mat row = dst.row(i);
        imageRow.convertTo(row, CV_32F);
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
    
    m_dataMatrix = FormatImagesForPCA(m_grayscaleImages).t();
    cv::PCA pca(m_dataMatrix, cv::Mat(), CV_PCA_DATA_AS_COL, numComponents);

    // And copy the PCA results:
    cv::Mat mean = pca.mean.clone();
    cv::Mat eigenvalues = pca.eigenvalues.clone();
    cv::Mat eigenvectors = pca.eigenvectors.clone();
    eigenvectors.copyTo(m_transformationMatrix);
}

std::vector<cv::Mat>
PCA::GetPrincipalComponents() {
    std::vector<cv::Mat> principalComponents;
    for (size_t i = 0; i < m_dataMatrix.size().width; ++i) {
        cv::Mat data = m_dataMatrix.col(i);
        cv::Mat y = m_transformationMatrix * data;
        y = Normalize(y);
        principalComponents.emplace_back(std::move(y));
    }
    return principalComponents;
}

}