#pragma once

#include <feature-detector/feature-detector.h>

namespace base {
	class Logger;
}

namespace features {

class FeatureMatcher {
public:
	/// <summary>
	/// default NORM_L2, SIFT/SURF -> NORM_L1, ORB/BRIEF/BRISK -> HAMMING
	/// </summary>
	/// <param name="mt"></param>
	FeatureMatcher(cv::DescriptorMatcher::MatcherType mt) : m_matcherType(mt) {
		m_matcher = cv::DescriptorMatcher::create(m_matcherType);
	}

	~FeatureMatcher() {}

	void ApplyMatching(cv::Mat queryDescriptors, cv::Mat trainDescriptors);
	/// <summary>
	/// Find best K matches for every descriptor in query descriptors set
	/// </summary>
	/// <param name="queryDescriptors">queried descriptors</param>
	/// <param name="trainDescriptors">trained descriptors</param>
	/// <param name="k">find the best K number of descriptor matches</param>
	/// <param name="ratioThreshold">threshold for filtering out best matches</param>
	void ApplyKnnMatching(cv::Mat queryDescriptors, cv::Mat trainDescriptors, int k = 5, float ratioThreshold = 0.7f);
	/// <summary>
	/// Find best matches in specific radius
	/// </summary>
	/// <param name="queryDescriptors">queried descriptors</param>
	/// <param name="trainDescriptors">trained descriptors</param>
	/// <param name="maxHammingDistance">-1 default means it will be automatically determined</param>
	/// <param name="ratioThreshold">threshold for filtering out best matches</param>
	void ApplyRadiusMatching(cv::Mat queryDescriptors, cv::Mat trainDescriptors, float maxHammingDistance = -1.0f, float ratioThreshold = 0.7f);

	void SortMatches();
	std::vector<cv::DMatch> GetMatches() { return m_matches; }

private:
	cv::Ptr<cv::DescriptorMatcher> m_matcher;
	cv::DescriptorMatcher::MatcherType m_matcherType;
	std::vector<cv::DMatch> m_matches;
	static std::shared_ptr<base::Logger> m_logger;
};

}