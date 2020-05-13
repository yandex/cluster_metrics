cmake_minimum_required(VERSION 3.10)
project(cluster_metrics_lib)
add_library(cluster_metrics_lib STATIC clusters.cpp metrics.cpp)
