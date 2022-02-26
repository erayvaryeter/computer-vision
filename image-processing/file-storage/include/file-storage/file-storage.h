#pragma once

#include <memory>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/persistence.hpp>

namespace base {
	class Logger;
}

namespace imgproc {

class FileStorage {
public:
	FileStorage(std::string fileName, cv::FileStorage::Mode storageMode);
	~FileStorage() { m_fileStorage.release(); }

	friend cv::FileStorage& operator<< (cv::FileStorage& fs, cv::cuda::GpuMat& gpuMat);
	friend void operator>> (const cv::FileNode& n, cv::cuda::GpuMat& gpuMat);

	template<typename T>
	void AppendNode(std::string nodeName, T object);

	template<typename T>
	T ReadNode(std::string nodeName);

private:
	std::string m_fileName;
	cv::FileStorage m_fileStorage;
	cv::FileStorage::Mode m_storageMode;
	static std::shared_ptr<base::Logger> m_logger;
};

}