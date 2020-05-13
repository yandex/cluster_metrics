#include <algorithm>
#include <fstream>

#include "clusters.h"

void TClusters::Load(std::istream& in) {
    std::string clusterId, elementId;
    while (in >> clusterId >> elementId) {
        ElementToClusterId[elementId] = clusterId;
        ++ClusterSizes[clusterId];
    }
}

TClusters TClusters::FromFile(const std::string& path) {
    TClusters result;
    std::ifstream in(path);
    result.Load(in);
    return result;
}

size_t TClusters::GetClusterSize(const std::string& clusterId) const {
    std::unordered_map<std::string, size_t>::const_iterator clIt = ClusterSizes.find(clusterId);
    return clIt == ClusterSizes.end() ? 0 : clIt->second;
}

TMatchings TClusters::BuildMatchings(const TClusters& targetClusters) const {
    std::unordered_map<std::string, size_t> clusterMarkedCount;
    std::unordered_map<std::string, std::unordered_map<std::string, TClustersMatching>> matchingsMap;

    for (auto&& elementWithTargetClusterId : targetClusters.ClusterSizes) {
        matchingsMap[elementWithTargetClusterId.first];
    }

    for (auto&& elementWithTargetClusterId : targetClusters.ElementToClusterId) {
        const std::string& elementId = elementWithTargetClusterId.first;
        std::unordered_map<std::string, std::string>::const_iterator myIt = ElementToClusterId.find(elementId);
        if (myIt == ElementToClusterId.end()) {
            continue;
        }

        const std::string& targetClusterId = elementWithTargetClusterId.second;
        const std::string& myClusterId = myIt->second;

        ++clusterMarkedCount[myClusterId];

        TClustersMatching& matching = matchingsMap[targetClusterId][myClusterId];

        matching.ThemeSize = targetClusters.GetClusterSize(targetClusterId);
        matching.ClusterSize = this->GetClusterSize(myClusterId);
        ++matching.CommonCount;
    }

    TMatchings matchings;
    for (auto&& themeIdWithData : matchingsMap) {
        TThemeMatchings themeMatchings;
        themeMatchings.ThemeId = themeIdWithData.first;
        for (auto&& clusterIdWithMatching : themeIdWithData.second) {
            const std::string& clusterId = clusterIdWithMatching.first;
            TClustersMatching clusterMatching = clusterIdWithMatching.second;
            clusterMatching.ClusteMarkedCount = clusterMarkedCount[clusterId];
            themeMatchings.ClusterMatchings.emplace_back(clusterMatching);
        }
        std::sort(themeMatchings.ClusterMatchings.begin(), themeMatchings.ClusterMatchings.end(), std::greater<>());
        matchings.push_back(themeMatchings);
    }
    std::sort(matchings.begin(), matchings.end());

    return matchings;
};
