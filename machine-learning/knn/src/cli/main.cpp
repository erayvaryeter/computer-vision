#include <knn/knn.h>
#include <file/file.h>

// #define METHOD_PCA
// #define METHOD_HARRIS_CORNERS
// #define METHOD_SHI_TOMASI_CORNERS
// #define METHOD_SIFT
// #define METHOD_SURF
#define METHOD_FAST

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
	auto knnPtr = std::make_shared<ml::KNN>(5, 20, ml::DataExtractionMethod::PCA);
	#endif 
	#ifdef METHOD_HARRIS_CORNERS
	auto knnPtr = std::make_shared<ml::KNN>(5, 50, ml::DataExtractionMethod::HARRIS_CORNERS);
	knnPtr->SetHarrisCornerParams(1, 1, 0.005);
	#endif
	#ifdef METHOD_SHI_TOMASI_CORNERS
	auto knnPtr = std::make_shared<ml::KNN>(3, 40, ml::DataExtractionMethod::SHI_TOMASI_CORNERS);
	knnPtr->SetShiTomasiCornerParams(25, 0.01, 1.0, 3, 3, false, 0.01);
	#endif
	#ifdef METHOD_SIFT
	auto knnPtr = std::make_shared<ml::KNN>(3, 50, ml::DataExtractionMethod::SIFT);
	knnPtr->SetSiftParams(0, 3, 0.04, 5.0, 2.8);
	#endif
	#ifdef METHOD_SURF
	auto knnPtr = std::make_shared<ml::KNN>(5, 50, ml::DataExtractionMethod::SURF);
	knnPtr->SetSurfParams(75.0, 4, 3, false, false);
	#endif
	#ifdef METHOD_FAST
	auto knnPtr = std::make_shared<ml::KNN>(7, 50, ml::DataExtractionMethod::FAST);
	knnPtr->SetFastParams(6, true);
	#endif

	knnPtr->ClearTrainData();
	knnPtr->ClearTestData();

	knnPtr->AppendTrainData(trainDir_0, 0, ".jpg");
	knnPtr->AppendTrainData(trainDir_1, 1, ".jpg");
	knnPtr->AppendTrainData(trainDir_2, 2, ".jpg");
	knnPtr->AppendTrainData(trainDir_3, 3, ".jpg");
	knnPtr->AppendTrainData(trainDir_4, 4, ".jpg");
	knnPtr->AppendTrainData(trainDir_5, 5, ".jpg");
	knnPtr->AppendTrainData(trainDir_6, 6, ".jpg");
	knnPtr->AppendTrainData(trainDir_7, 7, ".jpg");
	knnPtr->AppendTrainData(trainDir_8, 8, ".jpg");
	knnPtr->AppendTrainData(trainDir_9, 9, ".jpg");

	knnPtr->AppendTestData(testDir_0, 0, ".jpg");
	knnPtr->AppendTestData(testDir_1, 1, ".jpg");
	knnPtr->AppendTestData(testDir_2, 2, ".jpg");
	knnPtr->AppendTestData(testDir_3, 3, ".jpg");
	knnPtr->AppendTestData(testDir_4, 4, ".jpg");
	knnPtr->AppendTestData(testDir_5, 5, ".jpg");
	knnPtr->AppendTestData(testDir_6, 6, ".jpg");
	knnPtr->AppendTestData(testDir_7, 7, ".jpg");
	knnPtr->AppendTestData(testDir_8, 8, ".jpg");
	knnPtr->AppendTestData(testDir_9, 9, ".jpg");

	knnPtr->Train();
	knnPtr->Test();
}