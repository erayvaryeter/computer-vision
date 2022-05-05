#include <rtrees/rtrees.h>
#include <file/file.h>

// #define METHOD_PCA
// #define METHOD_HARRIS_CORNERS
// #define METHOD_SHI_TOMASI_CORNERS
// #define METHOD_SIFT
// #define METHOD_SURF
#define METHOD_FAST
// #define METHOD_BRIEF
// #define METHOD_ORB
// #define METHOD_BRISK

int main(int argc, char** argv) {
	std::string trainDir = "../../../../machine-learning/resource/train/";
	std::string testDir = "../../../../machine-learning/resource/test/";

	std::string trainDir_0 = trainDir + "0";
	std::string trainDir_1 = trainDir + "1";
	std::string trainDir_2 = trainDir + "2";
	std::string trainDir_3 = trainDir + "3";
	std::string trainDir_4 = trainDir + "4";
	std::string trainDir_5 = trainDir + "5";
	std::string trainDir_6 = trainDir + "6";
	std::string trainDir_7 = trainDir + "7";
	std::string trainDir_8 = trainDir + "8";
	std::string trainDir_9 = trainDir + "9";

	std::string testDir_0 = testDir + "0";
	std::string testDir_1 = testDir + "1";
	std::string testDir_2 = testDir + "2";
	std::string testDir_3 = testDir + "3";
	std::string testDir_4 = testDir + "4";
	std::string testDir_5 = testDir + "5";
	std::string testDir_6 = testDir + "6";
	std::string testDir_7 = testDir + "7";
	std::string testDir_8 = testDir + "8";
	std::string testDir_9 = testDir + "9";

	#ifdef METHOD_PCA
	auto rtreesPtr = std::make_shared<ml::RTrees>(5, 20, ml::DataExtractionMethod::PCA);
	#endif 
	#ifdef METHOD_HARRIS_CORNERS
	auto rtreesPtr = std::make_shared<ml::RTrees>(5, 50, ml::DataExtractionMethod::HARRIS_CORNERS);
	rtreesPtr->SetHarrisCornerParams(1, 1, 0.005);
	#endif
	#ifdef METHOD_SHI_TOMASI_CORNERS
	auto rtreesPtr = std::make_shared<ml::RTrees>(3, 40, ml::DataExtractionMethod::SHI_TOMASI_CORNERS);
	rtreesPtr->SetShiTomasiCornerParams(25, 0.01, 1.0, 3, 3, false, 0.01);
	#endif
	#ifdef METHOD_SIFT
	auto rtreesPtr = std::make_shared<ml::RTrees>(3, 50, ml::DataExtractionMethod::SIFT);
	rtreesPtr->SetSiftParams(0, 3, 0.04, 5.0, 2.8);
	#endif
	#ifdef METHOD_SURF
	auto rtreesPtr = std::make_shared<ml::RTrees>(5, 50, ml::DataExtractionMethod::SURF);
	rtreesPtr->SetSurfParams(75.0, 4, 3, false, false);
	#endif
	#ifdef METHOD_FAST
	auto rtreesPtr = std::make_shared<ml::RTrees>(7, 50, ml::DataExtractionMethod::FAST);
	rtreesPtr->SetFastParams(6, true);
	#endif
	#ifdef METHOD_BRIEF
	auto rtreesPtr = std::make_shared<ml::RTrees>(7, 50, ml::DataExtractionMethod::BRIEF);
	rtreesPtr->SetBriefParams(64, true);
	#endif
	#ifdef METHOD_ORB
	auto rtreesPtr = std::make_shared<ml::RTrees>(7, 50, ml::DataExtractionMethod::ORB);
	rtreesPtr->SetOrbParams(500, 1.1f, 10, 10, 0, 2, cv::ORB::HARRIS_SCORE, 31, 20);
	#endif
	#ifdef METHOD_BRISK
	auto rtreesPtr = std::make_shared<ml::RTrees>(5, 50, ml::DataExtractionMethod::BRISK);
	rtreesPtr->SetBriskParams(13, 3, 0.5f);
	#endif

	rtreesPtr->ClearTrainData();
	rtreesPtr->ClearTestData();

	rtreesPtr->AppendTrainData(trainDir_0, 0, ".jpg");
	rtreesPtr->AppendTrainData(trainDir_1, 1, ".jpg");
	rtreesPtr->AppendTrainData(trainDir_2, 2, ".jpg");
	rtreesPtr->AppendTrainData(trainDir_3, 3, ".jpg");
	rtreesPtr->AppendTrainData(trainDir_4, 4, ".jpg");
	rtreesPtr->AppendTrainData(trainDir_5, 5, ".jpg");
	rtreesPtr->AppendTrainData(trainDir_6, 6, ".jpg");
	rtreesPtr->AppendTrainData(trainDir_7, 7, ".jpg");
	rtreesPtr->AppendTrainData(trainDir_8, 8, ".jpg");
	rtreesPtr->AppendTrainData(trainDir_9, 9, ".jpg");

	rtreesPtr->AppendTestData(testDir_0, 0, ".jpg");
	rtreesPtr->AppendTestData(testDir_1, 1, ".jpg");
	rtreesPtr->AppendTestData(testDir_2, 2, ".jpg");
	rtreesPtr->AppendTestData(testDir_3, 3, ".jpg");
	rtreesPtr->AppendTestData(testDir_4, 4, ".jpg");
	rtreesPtr->AppendTestData(testDir_5, 5, ".jpg");
	rtreesPtr->AppendTestData(testDir_6, 6, ".jpg");
	rtreesPtr->AppendTestData(testDir_7, 7, ".jpg");
	rtreesPtr->AppendTestData(testDir_8, 8, ".jpg");
	rtreesPtr->AppendTestData(testDir_9, 9, ".jpg");

	rtreesPtr->Train();
	rtreesPtr->Test();
}