#include <file/file.h>
#include <assertion/assertion.h>
#include <histogram/histogram.h>
#include <file-storage/file-storage.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudaarithm.hpp>
#include <opencv2/cudacodec.hpp>
#include <iostream>
#include <string>
#include <vector>

//#define DUMP_RESULTS

class Fixture {
public:
	Fixture() {
        m_resourceDirectory = base::File::GetExecutablePath() / ".." / ".." / ".." / ".." / ".."
            / "image-processing" / "histogram" / "resource";
        
        m_colorImage1 = cv::imread(m_resourceDirectory / "Lena.jpg");
        m_colorImage2 = cv::imread(m_resourceDirectory / "SheldonCooper.jpg");
        m_grayImage1 = cv::imread(m_resourceDirectory / "Lena.jpg", cv::IMREAD_GRAYSCALE);
        m_grayImage2 = cv::imread(m_resourceDirectory / "SheldonCooper.jpg", cv::IMREAD_GRAYSCALE);

        m_colorImage1Gpu.upload(m_colorImage1);
        m_colorImage2Gpu.upload(m_colorImage2);
        m_grayImage1Gpu.upload(m_grayImage1);
        m_grayImage2Gpu.upload(m_grayImage2);

        ASSERT((m_colorImage1.channels() == 3), "Image 1 must be 3 channel BGR image", base::Logger::Severity::Error);
        ASSERT((m_colorImage1Gpu.channels() == 3), "Image 1 must be 3 channel BGR image", base::Logger::Severity::Error);

        ASSERT((m_colorImage2.channels() == 3), "Image 2 must be 3 channel BGR image", base::Logger::Severity::Error);
        ASSERT((m_colorImage2Gpu.channels() == 3), "Image 2 must be 3 channel BGR image", base::Logger::Severity::Error);

        ASSERT((m_grayImage1.channels() == 1), "Gray Image 1 must be 1 channel grayscale image", base::Logger::Severity::Error);
        ASSERT((m_grayImage1Gpu.channels() == 1), "Gray Image 1 must be 1 channel grayscale image", base::Logger::Severity::Error);

        ASSERT((m_grayImage2.channels() == 1), "Gray Image 2 must be 1 channel grayscale image", base::Logger::Severity::Error);
        ASSERT((m_grayImage2Gpu.channels() == 1), "Gray Image 2 must be 1 channel grayscale image", base::Logger::Severity::Error);

        m_histogramHandler = std::make_shared<imgproc::HistogramHandler>();

        auto xml = m_resourceDirectory / "HistogramResults.xml";
#ifdef DUMP_RESULTS
        if (base::File::FileExists(xml))
            m_fileStorage = std::make_shared<imgproc::FileStorage>(xml, cv::FileStorage::APPEND);
        else
            m_fileStorage = std::make_shared<imgproc::FileStorage>(xml, cv::FileStorage::WRITE);
#else
        m_fileStorage = std::make_shared<imgproc::FileStorage>(xml, cv::FileStorage::READ);
#endif
	}

	~Fixture() {
        m_fileStorage.reset();
        m_histogramHandler.reset();
	}

    bool CheckImagesEqual(const cv::Mat Mat1, const cv::Mat Mat2) {
        if (Mat1.channels() == 1 && Mat2.channels() == 1) {
            auto diff = Mat1 != Mat2;
            return cv::countNonZero(diff) == 0;
        }
        else {
            std::vector<cv::Mat> channels1, channels2;
            cv::split(Mat1, channels1);
            cv::split(Mat2, channels2);
            for (size_t i = 0; i < channels1.size(); ++i) {
                auto diff = channels1[i] != channels2[i];
                auto equal = cv::countNonZero(diff) == 0;
                if (!equal)
                    return false;
            }
            return true;
        }
    }

    bool CheckImagesEqual(const cv::cuda::GpuMat Mat1, const cv::cuda::GpuMat Mat2) {
        if (Mat1.channels() == 1 && Mat2.channels() == 1) {
            cv::cuda::GpuMat diff;
            cv::cuda::absdiff(Mat1, Mat2, diff);
            return cv::cuda::countNonZero(diff) == 0;
        }
        else {
            std::vector<cv::cuda::GpuMat> channels1, channels2;
            cv::cuda::split(Mat1, channels1);
            cv::cuda::split(Mat2, channels2);
            for (size_t i = 0; i < channels1.size(); ++i) {
                cv::cuda::GpuMat diff;
                cv::cuda::absdiff(channels1[i], channels2[i], diff);
                auto equal = cv::cuda::countNonZero(diff) == 0;
                if (!equal)
                    return false;
            }
            return true;
        }
    }

    // Member variables
    cv::Mat m_colorImage1;
    cv::Mat m_colorImage2;
    cv::Mat m_grayImage1;
    cv::Mat m_grayImage2;

    cv::cuda::GpuMat m_colorImage1Gpu;
    cv::cuda::GpuMat m_colorImage2Gpu;
    cv::cuda::GpuMat m_grayImage1Gpu;
    cv::cuda::GpuMat m_grayImage2Gpu;

    std::string m_resourceDirectory = "";

    std::shared_ptr<imgproc::FileStorage> m_fileStorage;
    std::shared_ptr<imgproc::HistogramHandler> m_histogramHandler;
};