#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#include "Edge.h"
#include "Node.h"
namespace campus {
struct Route {
  bool found = false;
  double distance = 0;
  std::vector<std::string> path;
  std::vector<std::string> edgeIds;
  std::vector<std::string> warnings;
};
class Graph {
 public:
  void addNode(Node);
  void addEdge(Edge);
  const std::unordered_map<std::string, Node>& nodes() const;
  const std::vector<Edge>& edges() const;
  Route shortestPath(const std::string&, const std::string&) const;

 private:
  std::unordered_map<std::string, Node> nodes_;
  std::vector<Edge> edges_;
  std::unordered_map<std::string, std::vector<std::pair<std::string, double>>>
      adjacency_;
};
}  // namespace campus
