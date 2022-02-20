#include <file/file.h>
#include <assertion/assertion.h>
#include <histogram/histogram.h>
#include <file-storage/file-storage.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string>
#include <vector>

// #define DUMP_RESULTS

class Fixture {
public:
	Fixture() {
        m_resourceDirectory = base::File::GetExecutablePath() / ".." / ".." / ".." / ".." / ".." / "resource";
        m_colorImage1 = cv::imread(m_resourceDirectory / "Lena.jpg");
        m_colorImage2 = cv::imread(m_resourceDirectory / "SheldonCooper.jpg");
        m_grayImage1 = cv::imread(m_resourceDirectory / "Lena.jpg", cv::IMREAD_GRAYSCALE);
        m_grayImage2 = cv::imread(m_resourceDirectory / "SheldonCooper.jpg", cv::IMREAD_GRAYSCALE);
        ASSERT((m_colorImage1.channels() == 3), "Image 1 must be 3 channel BGR image", base::Logger::Severity::Error);
        ASSERT((m_colorImage2.channels() == 3), "Image 2 must be 3 channel BGR image", base::Logger::Severity::Error);
        ASSERT((m_grayImage1.channels() == 1), "Gray Image 1 must be 1 channel grayscale image", base::Logger::Severity::Error);
        ASSERT((m_grayImage2.channels() == 1), "Gray Image 2 must be 1 channel grayscale image", base::Logger::Severity::Error);
        m_histogramHandler = std::make_shared<imgproc::HistogramHandler>();

#ifdef DUMP_RESULTS
        m_fileStorage = std::make_shared<imgproc::FileStorage>(m_resourceDirectory / "HistogramResults.xml", cv::FileStorage::WRITE);
#else
        m_fileStorage = std::make_shared<imgproc::FileStorage>(m_resourceDirectory / "HistogramResults.xml", cv::FileStorage::READ);
#endif
	}

	~Fixture() {
        m_fileStorage.reset();
        m_histogramHandler.reset();
	}

    bool CheckImagesEqual(const cv::Mat Mat1, const cv::Mat Mat2) {
        if (Mat1.dims == Mat2.dims && Mat1.size == Mat2.size && Mat1.elemSize() == Mat2.elemSize()) {
            if (Mat1.isContinuous() && Mat2.isContinuous()) {
                return 0 == memcmp(Mat1.ptr(), Mat2.ptr(), Mat1.total() * Mat1.elemSize());
            }
            else {
                const cv::Mat* arrays[] = { &Mat1, &Mat2, 0 };
                uchar* ptrs[2];
                cv::NAryMatIterator it(arrays, ptrs, 2);
                for (unsigned int p = 0; p < it.nplanes; p++, ++it)
                    if (0 != memcmp(it.ptrs[0], it.ptrs[1], it.size * Mat1.elemSize()))
                        return false;
                return true;
            }
        }
        return false;
    }

    // Member variables
    cv::Mat m_colorImage1;
    cv::Mat m_colorImage2;
    cv::Mat m_grayImage1;
    cv::Mat m_grayImage2;

    std::string m_resourceDirectory = "";

    std::shared_ptr<imgproc::FileStorage> m_fileStorage;
    std::shared_ptr<imgproc::HistogramHandler> m_histogramHandler;
};