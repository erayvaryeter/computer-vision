#include <logger/logger.h>
#include <assertion/assertion.h>
#include <file/file.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "stitcher/stitcher.h"

std::shared_ptr<base::Logger> homography::Stitcher::m_logger = std::make_shared<base::Logger>();

namespace homography {

void
Stitcher::ClearInputImages() {
	m_inputImages.clear();
}

void
Stitcher::AppendInputImage(cv::Mat image) {
	m_inputImages.emplace_back(std::move(image));
}

bool
Stitcher::StitchImages() {
	m_stitchedImage.release();
	auto res = m_stitcher->stitch(m_inputImages, m_stitchedImage);
	if (res == cv::Stitcher::OK)
		return true;
	else
		return false;
}

}