#pragma once

#include <vector>
#include <string>

#include "matching.h"
#include "welford.h"

struct TClusterMetrics {
    std::string Id;

    TMeanCalculator ECC;
    TMeanCalculator BCP;
    TMeanCalculator BCR;

    double BCF1 = 0.;

    TClusterMetrics& operator += (const TClusterMetrics& rhs);

    static TClusterMetrics FromThemeMatchings(const TThemeMatchings& themeMatching, const double unmarkedExpectedRelevance);
    static std::vector<TClusterMetrics> ThemeMetrics(const TMatchings& matchings, const double unmarkedExpectedRelevance);
    static TClusterMetrics FromThemeMetrics(const std::vector<TClusterMetrics>& themeMetrics);
};
