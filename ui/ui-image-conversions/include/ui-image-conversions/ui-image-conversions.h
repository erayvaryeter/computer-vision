#pragma once

#include <memory>
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QtGlobal>
#include <opencv2/imgcodecs.hpp>

namespace base {
	class Logger;
}

namespace ui {

class UiImageConversion {
public:

	static cv::Mat AdjustImageSize(const cv::Mat& inMat, int uiWidth = 800, int uiHeight = 450);

	static QImage CvMatToQImage(const cv::Mat& inMat);
	static QPixmap CvMatToQPixmap(const cv::Mat& inMat);

	static cv::Mat QImageToCvMat(const QImage& inImage, bool inCloneImageData = true);
	static cv::Mat QPixmapToCvMat(const QPixmap& inPixmap, bool inCloneImageData = true);

private:
	static std::shared_ptr<base::Logger> m_logger;
};

}