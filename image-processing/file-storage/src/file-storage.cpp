#include <logger/logger.h>
#include <assertion/assertion.h>
#include <file/file.h>
#include <opencv2/core.hpp>
#include <opencv2/cudacodec.hpp>
#include <type_traits>

#include "file-storage/file-storage.h"

std::shared_ptr<base::Logger> imgproc::FileStorage::m_logger = std::make_shared<base::Logger>();

cv::FileStorage&
operator<<(cv::FileStorage& fs, cv::cuda::GpuMat& gpuMat) {
	cv::Mat cpuMat;
	gpuMat.download(cpuMat);
	fs << cpuMat;
	return fs;
}

void 
operator>> (const cv::FileNode& n, cv::cuda::GpuMat& gpuMat) {
	cv::Mat cpuMat;
	n >> cpuMat;
	gpuMat.upload(cpuMat);
}

namespace imgproc {

FileStorage::FileStorage(std::string fileName, cv::FileStorage::Mode storageMode) {
	m_storageMode = storageMode;
	auto extension = base::File::GetFileExtension(fileName);
	ASSERT((extension == ".xml" || extension == ".yml"), "File must have XML or YML extension", base::Logger::Severity::Error);
	if (m_storageMode == cv::FileStorage::READ || m_storageMode == cv::FileStorage::APPEND) {
		ASSERT(base::File::FileExists(fileName), "File does not exist, reading/appending not possible", base::Logger::Severity::Error);
		m_fileName = fileName;
	}
	else if (m_storageMode == cv::FileStorage::WRITE) {
		auto parentPath = base::File::GetParentPath(fileName);
		ASSERT(base::File::DirectoryExists(parentPath), "Given path's parent directory does not exist",
			base::Logger::Severity::Error);
		m_fileName = fileName;
	}
	m_fileStorage = cv::FileStorage(m_fileName, m_storageMode);
	ASSERT(m_fileStorage.isOpened(), "File storage could not be opened", base::Logger::Severity::Error);
}

template<typename T>
void
FileStorage::AppendNode(std::string nodeName, T object) {
	try {
		m_fileStorage << nodeName << object;
	}
	catch (cv::Exception& e) {
		std::string exceptionMsg = "Appending node failed: " + std::string(e.what());
		ASSERT(true, exceptionMsg.c_str(), base::Logger::Severity::Error);
	}
}

template<typename T>
T
FileStorage::ReadNode(std::string nodeName) {
	try {
		T retVal;
		m_fileStorage[nodeName.c_str()] >> retVal;
		return retVal;
	}
	catch (cv::Exception& e) {
		std::string exceptionMsg = "Reading node failed: " + std::string(e.what());
		ASSERT(true, exceptionMsg.c_str(), base::Logger::Severity::Error);
	}
}

void TempFunc() {
	cv::Mat img;
	cv::cuda::GpuMat gpuImg;
	FileStorage fs_write("", cv::FileStorage::WRITE);
	fs_write.AppendNode<int>("", 1);
	fs_write.AppendNode<double>("", 1.0);
	fs_write.AppendNode<float>("", 1.0f);
	fs_write.AppendNode<std::string>("", "");
	fs_write.AppendNode<cv::Mat>("", img);
	fs_write.AppendNode<cv::cuda::GpuMat>("", gpuImg);
	FileStorage fs_read("", cv::FileStorage::READ);
	fs_read.ReadNode<int>("");
	fs_read.ReadNode<double>("");
	fs_read.ReadNode<float>("");
	fs_read.ReadNode<std::string>("");
	fs_read.ReadNode<cv::Mat>("");
	fs_read.ReadNode<cv::cuda::GpuMat>("");
}

}