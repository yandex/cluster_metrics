#include <cstring>
#include <iostream>

#include "lib/clusters.h"
#include "lib/metrics.h"

void PrintMetrics(const TClusterMetrics& basicMatrics, const TClusterMetrics& optimisticMetrics) {
    printf("ECC   %.5lf (%.5lf)\n", basicMatrics.ECC.GetMean(), optimisticMetrics.ECC.GetMean());
    printf("BCP   %.5lf (%.5lf)\n", basicMatrics.BCP.GetMean(), optimisticMetrics.BCP.GetMean());
    printf("BCR   %.5lf (%.5lf)\n", basicMatrics.BCR.GetMean(), optimisticMetrics.BCR.GetMean());
    printf("BCF1  %.5lf (%.5lf)\n", basicMatrics.BCF1, optimisticMetrics.BCF1);
}

void PrintThemeMetrics(const TClusterMetrics& basicMatrics, const TClusterMetrics& optimisticMetrics) {
    printf("ECC %.3lf (%.3lf)   ", basicMatrics.ECC.GetMean(), optimisticMetrics.ECC.GetMean());
    printf("BCP %.3lf (%.3lf)   ", basicMatrics.BCP.GetMean(), optimisticMetrics.BCP.GetMean());
    printf("BCR %.3lf (%.3lf)   ", basicMatrics.BCR.GetMean(), optimisticMetrics.BCR.GetMean());
    printf("BCF1 %.3lf (%.3lf)\n", basicMatrics.BCF1, optimisticMetrics.BCF1);
}

void PrintUsage() {
    std::cerr << "usage: cluster_metrics markup_clusters_file clusters_file [-v]" << std::endl;
    std::cerr << "add -v for per-theme statistics" << std::endl;
    std::cerr << "clusters file format:" << std::endl;
    std::cerr << "cluster_id \t element_id" << std::endl;
    std::cerr << "cluster_id \t element_id" << std::endl;
    std::cerr << "..." << std::endl;
}

int main(int argc, const char** argv) {
    if (argc < 3) {
        PrintUsage();
        return 1;
    }

    const TClusters markup = TClusters::FromFile(argv[1]);
    const TClusters sample = TClusters::FromFile(argv[2]);

    const bool verbose = argc == 4 && strcmp(argv[3], "-v") == 0;

    const TMatchings matchings = sample.BuildMatchings(markup);

    const std::vector<TClusterMetrics> basicMetricsVector = TClusterMetrics::ThemeMetrics(matchings, 0.);
    const std::vector<TClusterMetrics> optimisticMetricsVector = TClusterMetrics::ThemeMetrics(matchings, 1.);

    if (verbose) {
        for (size_t i = 0; i < matchings.size(); ++i) {
            std::cout << matchings[i].ThemeId << "\t";
            PrintThemeMetrics(basicMetricsVector[i], optimisticMetricsVector[i]);
        }
    }

    const TClusterMetrics basicMetrics = TClusterMetrics::FromThemeMetrics(basicMetricsVector);
    const TClusterMetrics optimisticMetrics = TClusterMetrics::FromThemeMetrics(optimisticMetricsVector);

    PrintMetrics(basicMetrics, optimisticMetrics);

    return 0;
}
