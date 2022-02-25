#include <file/file.h>
#include <assertion/assertion.h>
#include <file-storage/file-storage.h>
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <string>
#include <vector>

class Fixture {
public:
	
	Fixture() {
		m_resourceDirectory = base::File::GetExecutablePath() / ".." / ".." / ".." / ".." / ".."
			/ "image-processing" / "file-storage" / "resource";
		m_image = cv::imread((m_resourceDirectory / "Lena.jpg").c_str());
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

	std::string m_resourceDirectory = "";
	// values
	int m_integerValue = 10;
	double m_doubleValue = 20.0;
	float m_floatValue = 30.0f;
	std::string m_stringValue = "test-string";
	cv::Mat m_image;
};