#include <catch2/catch.hpp>
#include <logger/logger.h>
#include "Fixture.h"

auto histogramLogger = std::make_shared<base::Logger>();

TEST_CASE_METHOD(Fixture, "Calculate & Normalize & Compare Histograms") {
	histogramLogger << MESSAGE("Calculate & Normalize & Compare Histograms Test", base::Logger::Severity::Info);
	std::vector<cv::Mat> planes1;
	std::vector<cv::Mat> planes2;
	cv::split(m_colorImage1, planes1);
	cv::split(m_colorImage2, planes2);
	
	auto b_hist_1 = m_histogramHandler->CalculateHistogram(planes1[0], 256, 0, 256, true, false);
	auto g_hist_1 = m_histogramHandler->CalculateHistogram(planes1[1], 256, 0, 256, true, false);
	auto r_hist_1 = m_histogramHandler->CalculateHistogram(planes1[2], 256, 0, 256, true, false);
	auto gray_1 = m_histogramHandler->CalculateHistogram(m_grayImage1, 256, 0, 256, true, false);

	auto b_hist_2 = m_histogramHandler->CalculateHistogram(planes2[0], 256, 0, 256, true, false);
	auto g_hist_2 = m_histogramHandler->CalculateHistogram(planes2[1], 256, 0, 256, true, false);
	auto r_hist_2 = m_histogramHandler->CalculateHistogram(planes2[2], 256, 0, 256, true, false);
	auto gray_2 = m_histogramHandler->CalculateHistogram(m_grayImage2, 256, 0, 256, true, false);
	
	auto b_hist_1_norm = m_histogramHandler->NormalizeHistogram(b_hist_1);
	auto g_hist_1_norm = m_histogramHandler->NormalizeHistogram(g_hist_1);
	auto r_hist_1_norm = m_histogramHandler->NormalizeHistogram(r_hist_1);
	auto gray_1_norm = m_histogramHandler->NormalizeHistogram(gray_1);

	auto b_hist_2_norm = m_histogramHandler->NormalizeHistogram(b_hist_2);
	auto g_hist_2_norm = m_histogramHandler->NormalizeHistogram(g_hist_2);
	auto r_hist_2_norm = m_histogramHandler->NormalizeHistogram(r_hist_2);
	auto gray_2_norm = m_histogramHandler->NormalizeHistogram(gray_2);

	auto CheckVectorElements = [&](std::vector<cv::Mat>& vector) -> void {
		for (std::size_t i = 0; i < vector.size(); ++i) {
			for (std::size_t j = 0; j < vector.size(); ++j) {
				if (i == j) {
					CHECK(CheckImagesEqual(vector[i], vector[j]) == true);
				}
				else {
					CHECK(CheckImagesEqual(vector[i], vector[j]) == false);
				}
			}
		}
	};

	std::vector<cv::Mat> histograms = {
		b_hist_1, g_hist_1, r_hist_1, gray_1,
		b_hist_2, g_hist_2, r_hist_2, gray_2,
	};

	std::vector<cv::Mat> normalizedHistograms = {
		b_hist_1_norm, g_hist_1_norm, r_hist_1_norm, gray_1_norm,
		b_hist_2_norm, g_hist_2_norm, r_hist_2_norm, gray_2_norm
	};

	CheckVectorElements(histograms);
	CheckVectorElements(normalizedHistograms);

	cv::Mat comparisonResults = cv::Mat(1, 256, CV_64FC1);
	int counter = 0;
	for (std::size_t i = 0; i < normalizedHistograms.size(); ++i) {
		for (std::size_t j = 0; j < normalizedHistograms.size(); ++j) {
			comparisonResults.at<double>(0, counter) = 
				m_histogramHandler->CompareHistogram(normalizedHistograms[i], normalizedHistograms[j], 
					imgproc::HistogramComparisonMetrics::CORRELATION);
			comparisonResults.at<double>(0, counter) =
				m_histogramHandler->CompareHistogram(normalizedHistograms[i], normalizedHistograms[j],
					imgproc::HistogramComparisonMetrics::CHI_SQUARE);
			comparisonResults.at<double>(0, counter) =
				m_histogramHandler->CompareHistogram(normalizedHistograms[i], normalizedHistograms[j],
					imgproc::HistogramComparisonMetrics::INTERSECTION);
			comparisonResults.at<double>(0, counter) =
				m_histogramHandler->CompareHistogram(normalizedHistograms[i], normalizedHistograms[j],
					imgproc::HistogramComparisonMetrics::BHATTACHARYYA_DISTANCE);
			counter++;
		}
	}

#ifdef DUMP_RESULTS
	m_fileStorage->AppendNode("Color1_B", b_hist_1);
	m_fileStorage->AppendNode("Color2_B", b_hist_2);
	m_fileStorage->AppendNode("Color1_G", g_hist_1);
	m_fileStorage->AppendNode("Color2_G", g_hist_2);
	m_fileStorage->AppendNode("Color1_R", r_hist_1);
	m_fileStorage->AppendNode("Color2_R", r_hist_2);
	m_fileStorage->AppendNode("Gray_1", gray_1);
	m_fileStorage->AppendNode("Gray_2", gray_2);

	m_fileStorage->AppendNode("Normalized_Color1_B", b_hist_1_norm);
	m_fileStorage->AppendNode("Normalized_Color2_B", b_hist_2_norm);
	m_fileStorage->AppendNode("Normalized_Color1_G", g_hist_1_norm);
	m_fileStorage->AppendNode("Normalized_Color2_G", g_hist_2_norm);
	m_fileStorage->AppendNode("Normalized_Color1_R", r_hist_1_norm);
	m_fileStorage->AppendNode("Normalized_Color2_R", r_hist_2_norm);
	m_fileStorage->AppendNode("Normalized_Gray_1", gray_1_norm);
	m_fileStorage->AppendNode("Normalized_Gray_2", gray_2_norm);

	m_fileStorage->AppendNode("ComparisonResults", comparisonResults);
#else
	CHECK(CheckImagesEqual(b_hist_1, m_fileStorage->ReadNode<cv::Mat>("Color1_B")));
	CHECK(CheckImagesEqual(b_hist_2, m_fileStorage->ReadNode<cv::Mat>("Color2_B")));
	CHECK(CheckImagesEqual(g_hist_1, m_fileStorage->ReadNode<cv::Mat>("Color1_G")));
	CHECK(CheckImagesEqual(g_hist_2, m_fileStorage->ReadNode<cv::Mat>("Color2_G")));
	CHECK(CheckImagesEqual(r_hist_1, m_fileStorage->ReadNode<cv::Mat>("Color1_R")));
	CHECK(CheckImagesEqual(r_hist_2, m_fileStorage->ReadNode<cv::Mat>("Color2_R")));
	CHECK(CheckImagesEqual(gray_1, m_fileStorage->ReadNode<cv::Mat>("Gray_1")));
	CHECK(CheckImagesEqual(gray_2, m_fileStorage->ReadNode<cv::Mat>("Gray_2")));

	CHECK(CheckImagesEqual(b_hist_1_norm, m_fileStorage->ReadNode<cv::Mat>("Normalized_Color1_B")));
	CHECK(CheckImagesEqual(b_hist_2_norm, m_fileStorage->ReadNode<cv::Mat>("Normalized_Color2_B")));
	CHECK(CheckImagesEqual(g_hist_1_norm, m_fileStorage->ReadNode<cv::Mat>("Normalized_Color1_G")));
	CHECK(CheckImagesEqual(g_hist_2_norm, m_fileStorage->ReadNode<cv::Mat>("Normalized_Color2_G")));
	CHECK(CheckImagesEqual(r_hist_1_norm, m_fileStorage->ReadNode<cv::Mat>("Normalized_Color1_R")));
	CHECK(CheckImagesEqual(r_hist_2_norm, m_fileStorage->ReadNode<cv::Mat>("Normalized_Color2_R")));
	CHECK(CheckImagesEqual(gray_1_norm, m_fileStorage->ReadNode<cv::Mat>("Normalized_Gray_1")));
	CHECK(CheckImagesEqual(gray_2_norm, m_fileStorage->ReadNode<cv::Mat>("Normalized_Gray_2")));

	CHECK(CheckImagesEqual(comparisonResults, m_fileStorage->ReadNode<cv::Mat>("ComparisonResults")));
#endif

}

TEST_CASE_METHOD(Fixture, "Histogram Equalization Test") {
	histogramLogger << MESSAGE("Histogram Equalization Test", base::Logger::Severity::Info);
	auto equResultColor1 = m_histogramHandler->ApplyHistogramEqualization(m_colorImage1);
	auto equResultColor2 = m_histogramHandler->ApplyHistogramEqualization(m_colorImage2);
	auto equGray1 = m_histogramHandler->ApplyHistogramEqualization(m_grayImage1);
	auto equGray2 = m_histogramHandler->ApplyHistogramEqualization(m_grayImage2);
	auto adaptiveEquResultColor1 = m_histogramHandler->ApplyAdaptiveHistogramEqualization(m_colorImage1);
	auto adaptiveEquResultColor2 = m_histogramHandler->ApplyAdaptiveHistogramEqualization(m_colorImage2);
	auto adaptiveEquGray1 = m_histogramHandler->ApplyAdaptiveHistogramEqualization(m_grayImage1);
	auto adaptiveEquGray2 = m_histogramHandler->ApplyAdaptiveHistogramEqualization(m_grayImage2);

	cv::imwrite(m_resourceDirectory / "equResultColor1.jpg", equResultColor1);
	cv::imwrite(m_resourceDirectory / "equResultColor2.jpg", equResultColor2);
	cv::imwrite(m_resourceDirectory / "equGray1.jpg", equGray1);
	cv::imwrite(m_resourceDirectory / "equGray2.jpg", equGray2);
	cv::imwrite(m_resourceDirectory / "adaptiveEquResultColor1.jpg", adaptiveEquResultColor1);
	cv::imwrite(m_resourceDirectory / "adaptiveEquResultColor2.jpg", adaptiveEquResultColor2);
	cv::imwrite(m_resourceDirectory / "adaptiveEquGray1.jpg", adaptiveEquGray1);
	cv::imwrite(m_resourceDirectory / "adaptiveEquGray2.jpg", adaptiveEquGray2);

#ifdef DUMP_RESULTS
	m_fileStorage->AppendNode("equResultColor1", equResultColor1);
	m_fileStorage->AppendNode("equResultColor2", equResultColor2);
	m_fileStorage->AppendNode("equGray1", equGray1);
	m_fileStorage->AppendNode("equGray2", equGray2);
	m_fileStorage->AppendNode("adaptiveEquResultColor1", adaptiveEquResultColor1);
	m_fileStorage->AppendNode("adaptiveEquResultColor2", adaptiveEquResultColor2);
	m_fileStorage->AppendNode("adaptiveEquGray1", adaptiveEquGray1);
	m_fileStorage->AppendNode("adaptiveEquGray2", adaptiveEquGray2);
#else
	CHECK(CheckImagesEqual(equResultColor1, m_fileStorage->ReadNode<cv::Mat>("equResultColor1")));
	CHECK(CheckImagesEqual(equResultColor2, m_fileStorage->ReadNode<cv::Mat>("equResultColor2")));
	CHECK(CheckImagesEqual(equGray1, m_fileStorage->ReadNode<cv::Mat>("equGray1")));
	CHECK(CheckImagesEqual(equGray2, m_fileStorage->ReadNode<cv::Mat>("equGray2")));
	CHECK(CheckImagesEqual(adaptiveEquResultColor1, m_fileStorage->ReadNode<cv::Mat>("adaptiveEquResultColor1")));
	CHECK(CheckImagesEqual(adaptiveEquResultColor2, m_fileStorage->ReadNode<cv::Mat>("adaptiveEquResultColor2")));
	CHECK(CheckImagesEqual(adaptiveEquGray1, m_fileStorage->ReadNode<cv::Mat>("adaptiveEquGray1")));
	CHECK(CheckImagesEqual(adaptiveEquGray2, m_fileStorage->ReadNode<cv::Mat>("adaptiveEquGray2")));
#endif
}