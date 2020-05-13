#pragma once

#include <vector>

struct TClustersMatching {
    size_t ThemeSize = 0;

    size_t ClusterSize = 0;
    size_t ClusteMarkedCount = 0;

    size_t CommonCount = 0;

    double Recall() const {
        return (double) CommonCount / ThemeSize;
    }

    double Precision(const double unmarkedExpectedRelevance = 0.) const {
        const double expectedCommonCount = CommonCount + unmarkedExpectedRelevance * (ClusterSize - ClusteMarkedCount);
        return expectedCommonCount / ClusterSize;
    }

    bool operator > (const TClustersMatching& rhs) const {
        return CommonCount > rhs.CommonCount;
    }
};

struct TThemeMatchings {
    std::vector<TClustersMatching> ClusterMatchings;
    std::string ThemeId;

    bool operator < (const TThemeMatchings& rhs) const {
        return ThemeId < rhs.ThemeId;
    }
};

using TMatchings = std::vector<TThemeMatchings>;
