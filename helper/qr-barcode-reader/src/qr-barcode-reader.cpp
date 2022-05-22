#include <logger/logger.h>
#include <assertion/assertion.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "qr-barcode-reader/qr-barcode-reader.h"

std::shared_ptr<base::Logger> helper::QrBarcodeReader::m_logger = std::make_shared<base::Logger>();

namespace helper {

enum class ZXingImageFormat : uint32_t {
	None = 0,
	Lum = 0x01000000,
	RGB = 0x03000102,
	BGR = 0x03020100,
	RGBX = 0x04000102,
	XRGB = 0x04010203,
	BGRX = 0x04020100,
	XBGR = 0x04030201,
};

std::vector<ZXing::Result>
QrBarcodeReader::ReadBarcode(const cv::Mat& image, const ZXing::DecodeHints& hints) {

	auto GetMinMaxValues = [&](std::vector<ZXing::ResultPoint>& resPoints) -> cv::Rect {
		cv::Rect rect;
		int x_min = image.size().width * image.size().height;
		int y_min = image.size().width * image.size().height;
		int x_max = 0;
		int y_max = 0;
		for (auto& point : resPoints) {
			if (static_cast<int>(point.x()) < x_min)
				x_min = static_cast<int>(point.x());
			if (static_cast<int>(point.y()) < y_min)
				y_min = static_cast<int>(point.y());
			if (static_cast<int>(point.x()) > x_max)
				x_max = static_cast<int>(point.x());
			if (static_cast<int>(point.y()) > y_max)
				y_max = static_cast<int>(point.y());
		}
		rect.x = x_min;
		rect.y = y_min;
		rect.width = x_max - x_min;
		rect.height = y_max - y_min;
		return rect;
	};

	cv::Mat grayscale;
	cv::cvtColor(image, grayscale, cv::COLOR_BGR2GRAY);
	std::vector<ZXing::Result> retVal;
	
	while (true) {
		auto res = ZXing::ReadBarcode(grayscale.size().width, grayscale.size().height, grayscale.data, grayscale.size().width);
		if (res.isValid()) {
			auto resPoints = res.resultPoints();
			auto rect = GetMinMaxValues(resPoints);
			cv::rectangle(grayscale, rect, cv::Scalar(0, 0, 0), -1);
			retVal.emplace_back(std::move(res));
		}
		else {
			break;
		}
	}
	return retVal;
}

cv::Mat
QrBarcodeReader::DrawResult(const cv::Mat& img, std::vector<ZXing::Result>& res) {
	cv::Mat dest;
	img.copyTo(dest);
	for (auto& r : res) {
		auto pos = r.resultPoints();
		auto zx2cv = [](ZXing::ResultPoint p) { return cv::Point(p.x(), p.y()); };
		auto contour = std::vector<cv::Point>{ zx2cv(pos[0]), zx2cv(pos[1]), zx2cv(pos[2]), zx2cv(pos[3]) };
		const auto* pts = contour.data();
		int npts = contour.size();
		cv::polylines(dest, &pts, &npts, 1, true, CV_RGB(0, 255, 0));
		cv::putText(dest, ZXing::TextUtfEncoding::ToUtf8(r.text()), cv::Point(zx2cv(pos[0]).x - 10, zx2cv(pos[0]).y - 10), cv::FONT_HERSHEY_DUPLEX, 0.5,
			CV_RGB(0, 255, 0));
	}
	return dest;
}

}