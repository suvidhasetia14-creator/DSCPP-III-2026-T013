#pragma once
#include "Graph.h"
namespace campus {
class RouteService {
  const Graph& graph_;

 public:
  explicit RouteService(const Graph& g) : graph_(g) {}
  Route route(const std::string& a, const std::string& b) const {
    return graph_.shortestPath(a, b);
  }
};
}  // namespace campus
