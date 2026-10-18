#include "Graph.h"

#include <algorithm>
#include <cmath>
#include <functional>
#include <limits>
#include <queue>
#include <stdexcept>
namespace campus {
void Graph::addNode(Node n) {
  if (n.id.empty() || nodes_.count(n.id))
    throw std::invalid_argument("duplicate or empty node id");
  nodes_.emplace(n.id, std::move(n));
}
void Graph::addEdge(Edge e) {
  if (e.id.empty()) e.id = "edge-" + std::to_string(edges_.size() + 1);
  if (!nodes_.count(e.from) || !nodes_.count(e.to))
    throw std::invalid_argument("edge references unknown node");
  if (e.distance < 0 || (!e.unknownDistance && !std::isfinite(e.distance)))
    throw std::invalid_argument("invalid edge distance");
  edges_.push_back(e);
  if (!e.unknownDistance) {
    adjacency_[e.from].push_back({e.to, e.distance});
    if (!e.oneWay) adjacency_[e.to].push_back({e.from, e.distance});
  }
}
const std::unordered_map<std::string, Node>& Graph::nodes() const {
  return nodes_;
}
const std::vector<Edge>& Graph::edges() const { return edges_; }
Route Graph::shortestPath(const std::string& s, const std::string& t) const {
  Route r;
  for (const auto& e : edges_)
    if (e.unknownDistance)
      r.warnings.push_back("Skipped unknown distance ID " + e.id);
  if (!nodes_.count(s) || !nodes_.count(t)) return r;
  std::unordered_map<std::string, double> d;
  std::unordered_map<std::string, std::string> p;
  for (const auto& x : nodes_)
    d[x.first] = std::numeric_limits<double>::infinity();
  d[s] = 0;
  using Q = std::pair<double, std::string>;
  std::priority_queue<Q, std::vector<Q>, std::greater<Q>> q;
  q.push({0, s});
  while (!q.empty()) {
    auto [du, u] = q.top();
    q.pop();
    if (du != d[u]) continue;
    auto ait = adjacency_.find(u);
    if (ait == adjacency_.end()) continue;
    for (auto [v, w] : ait->second)
      if (du + w < d[v]) {
        d[v] = du + w;
        p[v] = u;
        q.push({d[v], v});
      }
  }
  if (!std::isfinite(d[t])) return r;
  r.found = true;
  r.distance = d[t];
  for (auto x = t;; x = p[x]) {
    r.path.push_back(x);
    if (x == s) break;
  }
  std::reverse(r.path.begin(), r.path.end());
  for (size_t i = 1; i < r.path.size(); ++i)
    for (const auto& e : edges_)
      if (e.from == r.path[i - 1] && e.to == r.path[i]) {
        r.edgeIds.push_back(e.id);
        if (e.unknownDistance)
          r.warnings.push_back("Unknown distance used for " + e.id);
        break;
      }
  return r;
}
}  // namespace campus
