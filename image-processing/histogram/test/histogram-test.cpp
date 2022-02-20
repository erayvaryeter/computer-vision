#include <catch2/catch.hpp>
#include "Fixture.h"

TEST_CASE_METHOD(Fixture, "Calculate & Normalize & Compare Histograms") {
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
#endif

}