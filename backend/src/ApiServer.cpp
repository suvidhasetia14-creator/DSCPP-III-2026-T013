#include "ApiServer.h"

#include <algorithm>
#include <cmath>
#include <iostream>

#include "../vendor/httplib.h"
#include "../vendor/nlohmann/json.hpp"
#include "RouteService.h"
namespace campus {
using json = nlohmann::json;
static void sendJson(httplib::Response& r, const json& body, int status = 200) {
  r.status = status;
  r.set_content(body.dump(), "application/json");
}
void runApiServer(const Graph& g, const std::string& root, int port) {
  httplib::Server s;
  s.set_mount_point("/", root.c_str());
  s.set_logger([](const httplib::Request& q, const httplib::Response& r) {
    std::cout << q.method << " " << q.path << " -> " << r.status << '\n';
  });
  s.Get("/api/nodes", [&](const httplib::Request&, httplib::Response& r) {
    std::vector<const Node*> v;
    for (const auto& [_, n] : g.nodes())
      if (!n.isJunction()) v.push_back(&n);
    std::sort(v.begin(), v.end(),
              [](auto a, auto b) { return a->name < b->name; });
    json a = json::array();
    for (auto n : v)
      a.push_back({{"id", n->id}, {"name", n->name}, {"type", n->type}});
    sendJson(r, json{{"nodes", a}});
  });
  s.Get("/api/graph", [&](const httplib::Request&, httplib::Response& r) {
    json ns = json::array(), es = json::array();
    for (const auto& [_, n] : g.nodes())
      ns.push_back({{"id", n.id},
                    {"name", n.name},
                    {"type", n.type},
                    {"x", n.x},
                    {"y", n.y}});
    for (const auto& e : g.edges())
      es.push_back({{"id", e.id},
                    {"from", e.from},
                    {"to", e.to},
                    {"direction", e.oneWay ? "one_way" : "two_way"},
                    {"distanceM",
                     e.unknownDistance ? json(nullptr) : json(e.distance)}});
    sendJson(r, json{{"nodes", ns}, {"edges", es}});
  });
  s.Get("/api/route", [&](const httplib::Request& q, httplib::Response& r) {
    if (!q.has_param("from") || !q.has_param("to") ||
        q.get_param_value("from").empty() || q.get_param_value("to").empty()) {
      sendJson(r, json{{"error", "from and to are required"}}, 400);
      return;
    }
    const auto from = q.get_param_value("from"), to = q.get_param_value("to");
    if (!g.nodes().count(from) || !g.nodes().count(to)) {
      sendJson(r, json{{"error", "Unknown node id"}}, 400);
      return;
    }
    auto out = RouteService(g).route(from, to);
    if (!out.found) {
      sendJson(r, json{{"found", false},
                       {"reason", "No route found"},
                       {"warnings", out.warnings}});
      return;
    }
    json stops = json::array();
    for (const auto& id : out.path) {
      const auto& n = g.nodes().at(id);
      if (!n.isJunction()) stops.push_back({{"id", n.id}, {"name", n.name}});
    }
    sendJson(r, json{{"found", true},
                     {"from", from},
                     {"to", to},
                     {"path", out.path},
                     {"stops", stops},
                     {"totalDistanceM", out.distance},
                     {"estimatedWalkMinutes",
                      std::round(out.distance / 1.4 / 60.0 * 10.0) / 10.0},
                     {"warnings", out.warnings}});
  });
  s.listen("0.0.0.0", port);
}
}  // namespace campus
