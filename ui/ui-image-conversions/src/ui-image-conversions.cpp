#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "ui-image-conversions/ui-image-conversions.h"

namespace ui {

cv::Mat
UiImageConversion::AdjustImageSize(const cv::Mat& inMat, int uiWidth, int uiHeight) {
    cv::Mat outMat;
    double ratioWidth = static_cast<double>(uiWidth) / static_cast<double>(inMat.size().width);
    double ratioHeight = static_cast<double>(uiHeight) / static_cast<double>(inMat.size().height);
    // CASE 1: If both image sizes are less than ui, just skip
    if (ratioWidth >= 1.0 && ratioHeight >= 1.0)
        outMat = inMat.clone();
    // CASE 2: If even one image size is bigger than ui, adapt accordingly both
    else if (ratioWidth < 1.0 || ratioHeight < 1.0) {
        if (ratioWidth <= ratioHeight) {
            cv::resize(inMat, outMat, cv::Size(static_cast<int>(inMat.size().width * ratioWidth), 
                static_cast<int>(inMat.size().height * ratioWidth)));
        }
        else {
            cv::resize(inMat, outMat, cv::Size(static_cast<int>(inMat.size().width * ratioHeight),
                static_cast<int>(inMat.size().height * ratioHeight)));
        }
    }
    return outMat;
}

QImage
UiImageConversion::CvMatToQImage(const cv::Mat& inMat) {
    switch (inMat.type()) {
    // 8-bit, 4 channel
    case CV_8UC4:
    {
        QImage image(inMat.data,
            inMat.cols, inMat.rows,
            static_cast<int>(inMat.step),
            QImage::Format_ARGB32);
        return image;
    }

    // 8-bit, 3 channel
    case CV_8UC3:
    {
        QImage image(inMat.data,
            inMat.cols, inMat.rows,
            static_cast<int>(inMat.step),
            QImage::Format_RGB888);
        return image.rgbSwapped();
    }

    // 8-bit, 1 channel
    case CV_8UC1:
    {
        QImage image(inMat.data,
            inMat.cols, inMat.rows,
            static_cast<int>(inMat.step),
            QImage::Format_Grayscale8);
        return image;
    }

    default:
        qWarning() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
        break;
    }

    return QImage();
}

QPixmap 
UiImageConversion::CvMatToQPixmap(const cv::Mat& inMat) {
    return QPixmap::fromImage(UiImageConversion::CvMatToQImage(inMat));
}

cv::Mat
UiImageConversion::QImageToCvMat(const QImage& inImage, bool inCloneImageData) {
    switch (inImage.format()) {
        // 8-bit, 4 channel
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
    {
        cv::Mat  mat(inImage.height(), inImage.width(),
            CV_8UC4,
            const_cast<uchar*>(inImage.bits()),
            static_cast<size_t>(inImage.bytesPerLine())
        );

        return (inCloneImageData ? mat.clone() : mat);
    }

    // 8-bit, 3 channel
    case QImage::Format_RGB32:
    {
        if (!inCloneImageData)
        {
            qWarning() << "ASM::QImageToCvMat() - Conversion requires cloning so we don't modify the original QImage data";
        }

        cv::Mat  mat(inImage.height(), inImage.width(),
            CV_8UC4,
            const_cast<uchar*>(inImage.bits()),
            static_cast<size_t>(inImage.bytesPerLine())
        );

        cv::Mat  matNoAlpha;

        cv::cvtColor(mat, matNoAlpha, cv::COLOR_BGRA2BGR);   // drop the all-white alpha channel

        return matNoAlpha;
    }

    // 8-bit, 3 channel
    case QImage::Format_RGB888:
    {
        if (!inCloneImageData)
        {
            qWarning() << "ASM::QImageToCvMat() - Conversion requires cloning so we don't modify the original QImage data";
        }

        QImage   swapped = inImage.rgbSwapped();

        return cv::Mat(swapped.height(), swapped.width(),
            CV_8UC3,
            const_cast<uchar*>(swapped.bits()),
            static_cast<size_t>(swapped.bytesPerLine())
        ).clone();
    }

    // 8-bit, 1 channel
    case QImage::Format_Indexed8:
    {
        cv::Mat  mat(inImage.height(), inImage.width(),
            CV_8UC1,
            const_cast<uchar*>(inImage.bits()),
            static_cast<size_t>(inImage.bytesPerLine())
        );

        return (inCloneImageData ? mat.clone() : mat);
    }

    default:
        qWarning() << "ASM::QImageToCvMat() - QImage format not handled in switch:" << inImage.format();
        break;
    }

    return cv::Mat();
}

cv::Mat
UiImageConversion::QPixmapToCvMat(const QPixmap& inPixmap, bool inCloneImageData) {
    return UiImageConversion::QImageToCvMat(inPixmap.toImage(), inCloneImageData);
}

}