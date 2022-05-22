#pragma once

#include <ZXing/ReadBarcode.h>
#include <ZXing/TextUtfEncoding.h>
#include <ZXing/DecodeHints.h>
#include <ZXing/BarcodeFormat.h>
#include <ZXing/Reader.h>
#include <ZXing/LuminanceSource.h>
#include <ZXing/ResultPoint.h>

#include <opencv2/opencv.hpp>

namespace base {
	class Logger;
}

namespace helper {

class QrBarcodeReader {
public:
	QrBarcodeReader() {};
	~QrBarcodeReader() {};
	std::vector<ZXing::Result> ReadBarcode(const cv::Mat& image, const ZXing::DecodeHints& hints = {});
	cv::Mat DrawResult(const cv::Mat& img, std::vector<ZXing::Result>& res);
private:
	static std::shared_ptr<base::Logger> m_logger;
};

}