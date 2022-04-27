#include <pca/pca.h>
#include <file/file.h>

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

	auto pcaPtr = std::make_shared<ml::PCA>();

	auto trainImages_0 = base::File::GetFilesWithExtension(trainDir_0, ".jpg");
	auto trainImages_1 = base::File::GetFilesWithExtension(trainDir_1, ".jpg");
	auto trainImages_2 = base::File::GetFilesWithExtension(trainDir_2, ".jpg");
	auto trainImages_3 = base::File::GetFilesWithExtension(trainDir_3, ".jpg");
	auto trainImages_4 = base::File::GetFilesWithExtension(trainDir_4, ".jpg");
	auto trainImages_5 = base::File::GetFilesWithExtension(trainDir_5, ".jpg");
	auto trainImages_6 = base::File::GetFilesWithExtension(trainDir_6, ".jpg");
	auto trainImages_7 = base::File::GetFilesWithExtension(trainDir_7, ".jpg");
	auto trainImages_8 = base::File::GetFilesWithExtension(trainDir_8, ".jpg");
	auto trainImages_9 = base::File::GetFilesWithExtension(trainDir_9, ".jpg");

	auto testImages_0 = base::File::GetFilesWithExtension(testDir_0, ".jpg");
	auto testImages_1 = base::File::GetFilesWithExtension(testDir_1, ".jpg");
	auto testImages_2 = base::File::GetFilesWithExtension(testDir_2, ".jpg");
	auto testImages_3 = base::File::GetFilesWithExtension(testDir_3, ".jpg");
	auto testImages_4 = base::File::GetFilesWithExtension(testDir_4, ".jpg");
	auto testImages_5 = base::File::GetFilesWithExtension(testDir_5, ".jpg");
	auto testImages_6 = base::File::GetFilesWithExtension(testDir_6, ".jpg");
	auto testImages_7 = base::File::GetFilesWithExtension(testDir_7, ".jpg");
	auto testImages_8 = base::File::GetFilesWithExtension(testDir_8, ".jpg");
	auto testImages_9 = base::File::GetFilesWithExtension(testDir_9, ".jpg");

	auto GetComponents = [&](std::vector<std::pair<std::string, std::string>>& images, int numComponents) -> std::vector<cv::Mat> {
		pcaPtr->ClearImages();
		for (auto im : images) {
			cv::Mat image = cv::imread(im.first.c_str());
			pcaPtr->AppendImage(image);
		}
		pcaPtr->ApplyPCA(numComponents);
		return pcaPtr->GetPrincipalComponents();
	};

	auto AddDataVector = [](cv::Mat& data, cv::Mat& labels, std::vector<cv::Mat>& components, int label) {
		for (auto component : components) {
			data.push_back(component.t());
			labels.push_back(label);
		}
	};

	int numberOfComponents = 50;
	int k = 7;

	cv::Mat train_data, train_labels;
	// principal components for train images
	auto train0 = GetComponents(trainImages_0, numberOfComponents);
	auto train1 = GetComponents(trainImages_1, numberOfComponents);
	auto train2 = GetComponents(trainImages_2, numberOfComponents);
	auto train3 = GetComponents(trainImages_3, numberOfComponents);
	auto train4 = GetComponents(trainImages_4, numberOfComponents);
	auto train5 = GetComponents(trainImages_5, numberOfComponents);
	auto train6 = GetComponents(trainImages_6, numberOfComponents);
	auto train7 = GetComponents(trainImages_7, numberOfComponents);
	auto train8 = GetComponents(trainImages_8, numberOfComponents);
	auto train9 = GetComponents(trainImages_9, numberOfComponents);

	AddDataVector(train_data, train_labels, train0, 0);
	AddDataVector(train_data, train_labels, train1, 1);
	AddDataVector(train_data, train_labels, train2, 2);
	AddDataVector(train_data, train_labels, train3, 3);
	AddDataVector(train_data, train_labels, train4, 4);
	AddDataVector(train_data, train_labels, train5, 5);
	AddDataVector(train_data, train_labels, train6, 6);
	AddDataVector(train_data, train_labels, train7, 7);
	AddDataVector(train_data, train_labels, train8, 8);
	AddDataVector(train_data, train_labels, train9, 9);

	cv::Mat test_data, test_labels;
	// principal components for test images
	auto test0 = GetComponents(testImages_0, numberOfComponents); 
	auto test1 = GetComponents(testImages_1, numberOfComponents); 
	auto test2 = GetComponents(testImages_2, numberOfComponents); 
	auto test3 = GetComponents(testImages_3, numberOfComponents); 
	auto test4 = GetComponents(testImages_4, numberOfComponents); 
	auto test5 = GetComponents(testImages_5, numberOfComponents); 
	auto test6 = GetComponents(testImages_6, numberOfComponents); 
	auto test7 = GetComponents(testImages_7, numberOfComponents); 
	auto test8 = GetComponents(testImages_8, numberOfComponents); 
	auto test9 = GetComponents(testImages_9, numberOfComponents); 

	AddDataVector(test_data, test_labels, test0, 0);
	AddDataVector(test_data, test_labels, test1, 1);
	AddDataVector(test_data, test_labels, test2, 2);
	AddDataVector(test_data, test_labels, test3, 3);
	AddDataVector(test_data, test_labels, test4, 4);
	AddDataVector(test_data, test_labels, test5, 5);
	AddDataVector(test_data, test_labels, test6, 6);
	AddDataVector(test_data, test_labels, test7, 7);
	AddDataVector(test_data, test_labels, test8, 8);
	AddDataVector(test_data, test_labels, test9, 9);

	cv::Ptr<cv::ml::KNearest> knnPtr = cv::ml::KNearest::create();
	knnPtr->train(train_data, cv::ml::ROW_SAMPLE, train_labels);

	int correctClassify = 0;
	int wrongClassify = 0;

	for (int i = 0; i < test_data.rows; i++) {
		cv::Mat result;
		cv::Mat neighbors;
		knnPtr->findNearest(test_data.row(i), k, result, neighbors);
		int expected = test_labels.at<int>(i);
		int predicted = (int)result.at<float>(0);
		if (expected == predicted)
			correctClassify++;
		else
			wrongClassify++;
		std::cout << "Expected: " << expected << " - Predicted: " << predicted << " Neighbors: " << neighbors << std::endl;
	}

	std::cout << "-------------------------------------------------------------------------------" << std::endl;

	std::cout << "Correctly classified % " << static_cast<float>(correctClassify) / static_cast<float>(correctClassify + wrongClassify) 
		* 100 << std::endl;

	std::cout << "Wrongly classified % " << static_cast<float>(wrongClassify) / static_cast<float>(correctClassify + wrongClassify)
		* 100 << std::endl;

	return 0;
}