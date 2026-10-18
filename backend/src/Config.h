#pragma once
#include <string>
namespace campus::config {
inline constexpr int defaultPort = 8080;
inline constexpr const char* nodesFile = "data/nodes.csv";
inline constexpr const char* edgesFile = "data/edges.csv";
inline constexpr const char* webRoot = "frontend";
inline constexpr const char* apiPrefix = "/api";
}  // namespace campus::config
