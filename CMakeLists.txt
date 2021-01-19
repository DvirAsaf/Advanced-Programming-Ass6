cmake_minimum_required(VERSION 3.16)
project(untitled6)

set(CMAKE_CXX_STANDARD 11)

add_executable(untitled6 main.cpp commands.h AnomalyDetector.h CLI.cpp CLI.h HybridAnomalyDetector.cpp HybridAnomalyDetector.h MainTrain.cpp SimpleAnomalyDetector.cpp SimpleAnomalyDetector.h anomaly_detection_util.cpp anomaly_detection_util.h timeseries.cpp timeseries.h minCircle.cpp minCircle.h)