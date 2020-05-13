#pragma once

#include <istream>
#include <vector>
#include <string>
#include <unordered_map>

#include "matching.h"

class TClusters {
private:
    std::unordered_map<std::string, size_t> ClusterSizes;
    std::unordered_map<std::string, std::string> ElementToClusterId;
public:
    void Load(std::istream& in);
    static TClusters FromFile(const std::string& path);

    size_t GetClusterSize(const std::string& clusterId) const;
    TMatchings BuildMatchings(const TClusters& targetClusters) const;
};
