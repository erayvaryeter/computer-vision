#include <catch2/catch.hpp>
#include <logger/logger.h>
#include "Fixture.h"

auto filestorageLogger = std::make_shared<base::Logger>();

TEST_CASE_METHOD(Fixture, "Write Storage XML Files") {
	filestorageLogger << MESSAGE("Write Storage XML Files", base::Logger::Severity::Info);
	auto fileStorage = std::make_shared<imgproc::FileStorage>(m_resourceDirectory / "Test.xml", cv::FileStorage::WRITE);
	fileStorage->AppendNode("INTEGER", m_integerValue);
	fileStorage->AppendNode("DOUBLE", m_doubleValue);
	fileStorage->AppendNode("FLOAT", m_floatValue);
	fileStorage->AppendNode("STRING", m_stringValue);
	fileStorage->AppendNode("IMAGE", m_image);
}

TEST_CASE_METHOD(Fixture, "Write Storage YML Files") {
	filestorageLogger << MESSAGE("Write Storage YML Files", base::Logger::Severity::Info);
	auto fileStorage = std::make_shared<imgproc::FileStorage>(m_resourceDirectory / "Test.yml", cv::FileStorage::WRITE);
	fileStorage->AppendNode("INTEGER", m_integerValue);
	fileStorage->AppendNode("DOUBLE", m_doubleValue);
	fileStorage->AppendNode("FLOAT", m_floatValue);
	fileStorage->AppendNode("STRING", m_stringValue);
	fileStorage->AppendNode("IMAGE", m_image);
}

TEST_CASE_METHOD(Fixture, "Read Storage XML Files") {
	filestorageLogger << MESSAGE("Read Storage XML Files", base::Logger::Severity::Info);
	auto fileStorage = std::make_shared<imgproc::FileStorage>(m_resourceDirectory / "Test.xml", cv::FileStorage::READ);
	CHECK(fileStorage->ReadNode<int>("INTEGER") == m_integerValue);
	CHECK(fileStorage->ReadNode<double>("DOUBLE") == m_doubleValue);
	CHECK(fileStorage->ReadNode<float>("FLOAT") == m_floatValue);
	CHECK(fileStorage->ReadNode<std::string>("STRING") == m_stringValue);
	CHECK(CheckImagesEqual(fileStorage->ReadNode<cv::Mat>("IMAGE"), m_image));
}

TEST_CASE_METHOD(Fixture, "Read Storage YML Files") {
	filestorageLogger << MESSAGE("Read Storage YML Files", base::Logger::Severity::Info);
	auto fileStorage = std::make_shared<imgproc::FileStorage>(m_resourceDirectory / "Test.yml", cv::FileStorage::READ);
	CHECK(fileStorage->ReadNode<int>("INTEGER") == m_integerValue);
	CHECK(fileStorage->ReadNode<double>("DOUBLE") == m_doubleValue);
	CHECK(fileStorage->ReadNode<float>("FLOAT") == m_floatValue);
	CHECK(fileStorage->ReadNode<std::string>("STRING") == m_stringValue);
	CHECK(CheckImagesEqual(fileStorage->ReadNode<cv::Mat>("IMAGE"), m_image));
}