#include "metrics.h"

namespace {
    static inline double ExpectedClusterCompleteness(const TThemeMatchings& sortedClusterMatchings, const double unmarkedExpectedRelevance) {
        double expectedClusterCompleteness = 0.;

        double probability = 1.;
        for (const TClustersMatching& matching : sortedClusterMatchings.ClusterMatchings) {
            const double matchProbability = matching.Precision(unmarkedExpectedRelevance);
            expectedClusterCompleteness += matching.Recall() * matchProbability * probability;
            probability *= 1. - matchProbability;
        }

        return expectedClusterCompleteness;
    }

    static inline double BCubedPrecision(const TThemeMatchings& sortedClusterMatchings, const double unmarkedExpectedRelevance) {
        TMeanCalculator meanPrecisionCalculator;
        for (const TClustersMatching& matching : sortedClusterMatchings.ClusterMatchings) {
            meanPrecisionCalculator.Add(matching.Precision(unmarkedExpectedRelevance), matching.CommonCount);
        }
        return meanPrecisionCalculator.GetMean();
    }

    static inline double BCubedRecall(const TThemeMatchings& sortedClusterMatchings) {
        TMeanCalculator meanRecallCalculator;

        size_t themeSize = 0;
        size_t clustered = 0;

        for (const TClustersMatching& matching : sortedClusterMatchings.ClusterMatchings) {
            themeSize = matching.ThemeSize;
            clustered += matching.CommonCount;

            meanRecallCalculator.Add(matching.Recall(), matching.CommonCount);
        }
        if (themeSize > clustered) {
            meanRecallCalculator.Add(0., themeSize - clustered);
        }

        return meanRecallCalculator.GetMean();
    }

    static inline double F1(const double precision, const double recall) {
        return 2. * precision * recall / (precision + recall + 1e-15);
    }

    static inline double F1(const TClusterMetrics& metrics) {
        return F1(metrics.BCP.GetMean(), metrics.BCR.GetMean());
    }
}

TClusterMetrics& TClusterMetrics::operator += (const TClusterMetrics& rhs) {
    ECC += rhs.ECC;

    if (rhs.BCR.GetMean()) {
        BCP += rhs.BCP;
    }

    BCR += rhs.BCR;
    BCF1 = F1(*this);
    return *this;
}

TClusterMetrics TClusterMetrics::FromThemeMatchings(const TThemeMatchings& themeMatching, const double unmarkedExpectedRelevance) {
    TClusterMetrics result;
    result.ECC.Add(ExpectedClusterCompleteness(themeMatching, unmarkedExpectedRelevance));
    result.BCP.Add(BCubedPrecision(themeMatching, unmarkedExpectedRelevance));
    result.BCR.Add(BCubedRecall(themeMatching));
    result.BCF1 = F1(result);
    return result;
}

std::vector<TClusterMetrics> TClusterMetrics::ThemeMetrics(const TMatchings& matchings, const double unmarkedExpectedRelevance) {
    std::vector<TClusterMetrics> result;
    for (const TThemeMatchings& themeMatching : matchings) {
        result.push_back(TClusterMetrics::FromThemeMatchings(themeMatching, unmarkedExpectedRelevance));
        result.back().Id = themeMatching.ThemeId;
    }
    return result;
}

TClusterMetrics TClusterMetrics::FromThemeMetrics(const std::vector<TClusterMetrics>& themeMetrics) {
    TClusterMetrics result;
    for (const TClusterMetrics metrics : themeMetrics) {
        result += metrics;
    }
    return result;
}
