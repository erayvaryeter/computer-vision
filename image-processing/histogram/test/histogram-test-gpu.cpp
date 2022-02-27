#include <catch2/catch.hpp>
#include <logger/logger.h>
#include "Fixture.h"

auto histogramLoggerGpu = std::make_shared<base::Logger>();

TEST_CASE_METHOD(Fixture, "Calculate & Normalize & Compare Histograms - GPU") {
	histogramLoggerGpu << MESSAGE("Calculate & Normalize & Compare Histograms Test - GPU", base::Logger::Severity::Info);
	std::vector<cv::cuda::GpuMat> planes1;
	std::vector<cv::cuda::GpuMat> planes2;
	cv::cuda::split(m_colorImage1Gpu, planes1);
	cv::cuda::split(m_colorImage2Gpu, planes2);
	
	auto b_hist_1_gpu = m_histogramHandler->CalculateHistogram(planes1[0]);
	auto g_hist_1_gpu = m_histogramHandler->CalculateHistogram(planes1[1]);
	auto r_hist_1_gpu = m_histogramHandler->CalculateHistogram(planes1[2]);
	auto gray_1_gpu = m_histogramHandler->CalculateHistogram(m_grayImage1Gpu);

	auto b_hist_2_gpu = m_histogramHandler->CalculateHistogram(planes2[0]);
	auto g_hist_2_gpu = m_histogramHandler->CalculateHistogram(planes2[1]);
	auto r_hist_2_gpu = m_histogramHandler->CalculateHistogram(planes2[2]);
	auto gray_2_gpu = m_histogramHandler->CalculateHistogram(m_grayImage2Gpu);
	
	auto b_hist_1_norm_gpu = m_histogramHandler->NormalizeHistogram(b_hist_1_gpu);
	auto g_hist_1_norm_gpu = m_histogramHandler->NormalizeHistogram(g_hist_1_gpu);
	auto r_hist_1_norm_gpu = m_histogramHandler->NormalizeHistogram(r_hist_1_gpu);
	auto gray_1_norm_gpu = m_histogramHandler->NormalizeHistogram(gray_1_gpu);

	auto b_hist_2_norm_gpu = m_histogramHandler->NormalizeHistogram(b_hist_2_gpu);
	auto g_hist_2_norm_gpu = m_histogramHandler->NormalizeHistogram(g_hist_2_gpu);
	auto r_hist_2_norm_gpu = m_histogramHandler->NormalizeHistogram(r_hist_2_gpu);
	auto gray_2_norm_gpu = m_histogramHandler->NormalizeHistogram(gray_2_gpu);

	auto CheckVectorElements = [&](std::vector<cv::cuda::GpuMat>& vector) -> void {
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

	std::vector<cv::cuda::GpuMat> histograms = {
		b_hist_1_gpu, g_hist_1_gpu, r_hist_1_gpu, gray_1_gpu,
		b_hist_2_gpu, g_hist_2_gpu, r_hist_2_gpu, gray_2_gpu,
	};

	std::vector<cv::cuda::GpuMat> normalizedHistograms = {
		b_hist_1_norm_gpu, g_hist_1_norm_gpu, r_hist_1_norm_gpu, gray_1_norm_gpu,
		b_hist_2_norm_gpu, g_hist_2_norm_gpu, r_hist_2_norm_gpu, gray_2_norm_gpu
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
	m_fileStorage->AppendNode("Color1_B_GPU", b_hist_1_gpu);
	m_fileStorage->AppendNode("Color2_B_GPU", b_hist_2_gpu);
	m_fileStorage->AppendNode("Color1_G_GPU", g_hist_1_gpu);
	m_fileStorage->AppendNode("Color2_G_GPU", g_hist_2_gpu);
	m_fileStorage->AppendNode("Color1_R_GPU", r_hist_1_gpu);
	m_fileStorage->AppendNode("Color2_R_GPU", r_hist_2_gpu);
	m_fileStorage->AppendNode("Gray_1_GPU", gray_1_gpu);
	m_fileStorage->AppendNode("Gray_2_GPU", gray_2_gpu);

	m_fileStorage->AppendNode("Normalized_Color1_B_GPU", b_hist_1_norm_gpu);
	m_fileStorage->AppendNode("Normalized_Color2_B_GPU", b_hist_2_norm_gpu);
	m_fileStorage->AppendNode("Normalized_Color1_G_GPU", g_hist_1_norm_gpu);
	m_fileStorage->AppendNode("Normalized_Color2_G_GPU", g_hist_2_norm_gpu);
	m_fileStorage->AppendNode("Normalized_Color1_R_GPU", r_hist_1_norm_gpu);
	m_fileStorage->AppendNode("Normalized_Color2_R_GPU", r_hist_2_norm_gpu);
	m_fileStorage->AppendNode("Normalized_Gray_1_GPU", gray_1_norm_gpu);
	m_fileStorage->AppendNode("Normalized_Gray_2_GPU", gray_2_norm_gpu);

	m_fileStorage->AppendNode("ComparisonResults_GPU", comparisonResults);
#else
	CHECK(CheckImagesEqual(b_hist_1_gpu, m_fileStorage->ReadNode<cv::cuda::GpuMat>("Color1_B_GPU")));
	CHECK(CheckImagesEqual(b_hist_2_gpu, m_fileStorage->ReadNode<cv::cuda::GpuMat>("Color2_B_GPU")));
	CHECK(CheckImagesEqual(g_hist_1_gpu, m_fileStorage->ReadNode<cv::cuda::GpuMat>("Color1_G_GPU")));
	CHECK(CheckImagesEqual(g_hist_2_gpu, m_fileStorage->ReadNode<cv::cuda::GpuMat>("Color2_G_GPU")));
	CHECK(CheckImagesEqual(r_hist_1_gpu, m_fileStorage->ReadNode<cv::cuda::GpuMat>("Color1_R_GPU")));
	CHECK(CheckImagesEqual(r_hist_2_gpu, m_fileStorage->ReadNode<cv::cuda::GpuMat>("Color2_R_GPU")));
	CHECK(CheckImagesEqual(gray_1_gpu, m_fileStorage->ReadNode<cv::cuda::GpuMat>("Gray_1_GPU")));
	CHECK(CheckImagesEqual(gray_2_gpu, m_fileStorage->ReadNode<cv::cuda::GpuMat>("Gray_2_GPU")));

	CHECK(CheckImagesEqual(b_hist_1_norm_gpu, m_fileStorage->ReadNode<cv::cuda::GpuMat>("Normalized_Color1_B_GPU")));
	CHECK(CheckImagesEqual(b_hist_2_norm_gpu, m_fileStorage->ReadNode<cv::cuda::GpuMat>("Normalized_Color2_B_GPU")));
	CHECK(CheckImagesEqual(g_hist_1_norm_gpu, m_fileStorage->ReadNode<cv::cuda::GpuMat>("Normalized_Color1_G_GPU")));
	CHECK(CheckImagesEqual(g_hist_2_norm_gpu, m_fileStorage->ReadNode<cv::cuda::GpuMat>("Normalized_Color2_G_GPU")));
	CHECK(CheckImagesEqual(r_hist_1_norm_gpu, m_fileStorage->ReadNode<cv::cuda::GpuMat>("Normalized_Color1_R_GPU")));
	CHECK(CheckImagesEqual(r_hist_2_norm_gpu, m_fileStorage->ReadNode<cv::cuda::GpuMat>("Normalized_Color2_R_GPU")));
	CHECK(CheckImagesEqual(gray_1_norm_gpu, m_fileStorage->ReadNode<cv::cuda::GpuMat>("Normalized_Gray_1_GPU")));
	CHECK(CheckImagesEqual(gray_2_norm_gpu, m_fileStorage->ReadNode<cv::cuda::GpuMat>("Normalized_Gray_2_GPU")));

	CHECK(CheckImagesEqual(comparisonResults, m_fileStorage->ReadNode<cv::Mat>("ComparisonResults_GPU")));
#endif

}

TEST_CASE_METHOD(Fixture, "Histogram Equalization Test - GPU") {
	histogramLoggerGpu << MESSAGE("Histogram Equalization Test - GPU", base::Logger::Severity::Info);
	auto equResultColor1Gpu = m_histogramHandler->ApplyHistogramEqualization(m_colorImage1Gpu);
	auto equResultColor2Gpu = m_histogramHandler->ApplyHistogramEqualization(m_colorImage2Gpu);
	auto equGray1Gpu = m_histogramHandler->ApplyHistogramEqualization(m_grayImage1Gpu);
	auto equGray2Gpu = m_histogramHandler->ApplyHistogramEqualization(m_grayImage2Gpu);
	auto adaptiveEquResultColor1Gpu = m_histogramHandler->ApplyAdaptiveHistogramEqualization(m_colorImage1Gpu);
	auto adaptiveEquResultColor2Gpu = m_histogramHandler->ApplyAdaptiveHistogramEqualization(m_colorImage2Gpu);
	auto adaptiveEquGray1Gpu = m_histogramHandler->ApplyAdaptiveHistogramEqualization(m_grayImage1Gpu);
	auto adaptiveEquGray2Gpu = m_histogramHandler->ApplyAdaptiveHistogramEqualization(m_grayImage2Gpu);

#ifdef DUMP_RESULTS
	m_fileStorage->AppendNode("equResultColor1Gpu", equResultColor1Gpu);
	m_fileStorage->AppendNode("equResultColor2Gpu", equResultColor2Gpu);
	m_fileStorage->AppendNode("equGray1Gpu", equGray1Gpu);
	m_fileStorage->AppendNode("equGray2Gpu", equGray2Gpu);
	m_fileStorage->AppendNode("adaptiveEquResultColor1Gpu", adaptiveEquResultColor1Gpu);
	m_fileStorage->AppendNode("adaptiveEquResultColor2Gpu", adaptiveEquResultColor2Gpu);
	m_fileStorage->AppendNode("adaptiveEquGray1Gpu", adaptiveEquGray1Gpu);
	m_fileStorage->AppendNode("adaptiveEquGray2Gpu", adaptiveEquGray2Gpu);
#else
	CHECK(CheckImagesEqual(equResultColor1Gpu, m_fileStorage->ReadNode<cv::cuda::GpuMat>("equResultColor1Gpu")));
	CHECK(CheckImagesEqual(equResultColor2Gpu, m_fileStorage->ReadNode<cv::cuda::GpuMat>("equResultColor2Gpu")));
	CHECK(CheckImagesEqual(equGray1Gpu, m_fileStorage->ReadNode<cv::cuda::GpuMat>("equGray1Gpu")));
	CHECK(CheckImagesEqual(equGray2Gpu, m_fileStorage->ReadNode<cv::cuda::GpuMat>("equGray2Gpu")));
	CHECK(CheckImagesEqual(adaptiveEquResultColor1Gpu, m_fileStorage->ReadNode<cv::cuda::GpuMat>("adaptiveEquResultColor1Gpu")));
	CHECK(CheckImagesEqual(adaptiveEquResultColor2Gpu, m_fileStorage->ReadNode<cv::cuda::GpuMat>("adaptiveEquResultColor2Gpu")));
	CHECK(CheckImagesEqual(adaptiveEquGray1Gpu, m_fileStorage->ReadNode<cv::cuda::GpuMat>("adaptiveEquGray1Gpu")));
	CHECK(CheckImagesEqual(adaptiveEquGray2Gpu, m_fileStorage->ReadNode<cv::cuda::GpuMat>("adaptiveEquGray2Gpu")));
#endif
}