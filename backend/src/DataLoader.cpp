#include "DataLoader.h"

#include <cmath>
#include <stdexcept>

#include "CsvReader.h"
namespace campus {
Graph loadData(const std::string& np, const std::string& ep) {
  Graph g;
  auto n = CsvReader::read(np);
  if (n.empty() || n[0].fields.size() != 6 || n[0].fields[0] != "id" ||
      n[0].fields[2] != "type" || n[0].fields[3] != "x" ||
      n[0].fields[4] != "y")
    throw std::runtime_error(np + ": expected id,name,type,x,y,notes");
  for (size_t i = 1; i < n.size(); ++i) {
    auto& r = n[i];
    if (r.fields.size() != 6)
      throw std::runtime_error(np + ":" + std::to_string(r.line) +
                               ": expected 6 columns");
    try {
      g.addNode({r.fields[0], r.fields[1], r.fields[2], std::stod(r.fields[3]),
                 std::stod(r.fields[4]), r.fields[5]});
    } catch (const std::exception& e) {
      throw std::runtime_error(np + ":" + std::to_string(r.line) + ": " +
                               e.what());
    }
  }
  auto e = CsvReader::read(ep);
  if (e.empty() || e[0].fields.size() != 5 || e[0].fields[0] != "id" ||
      e[0].fields[1] != "from" || e[0].fields[2] != "to" ||
      e[0].fields[3] != "direction" || e[0].fields[4] != "distance_m")
    throw std::runtime_error(ep + ": expected id,from,to,direction,distance_m");
  for (size_t i = 1; i < e.size(); ++i) {
    auto& r = e[i];
    if (r.fields.size() != 5)
      throw std::runtime_error(ep + ":" + std::to_string(r.line) +
                               ": expected 5 columns");
    if (r.fields[3] != "one_way" && r.fields[3] != "two_way")
      throw std::runtime_error(ep + ":" + std::to_string(r.line) +
                               ": direction must be one_way or two_way");
    bool unknown = r.fields[4].empty();
    double d = 0;
    if (!unknown) {
      try {
        d = std::stod(r.fields[4]);
      } catch (...) {
        throw std::runtime_error(ep + ":" + std::to_string(r.line) +
                                 ": distance_m must be numeric or blank");
      }
      if (d < 0 || !std::isfinite(d))
        throw std::runtime_error(ep + ":" + std::to_string(r.line) +
                                 ": distance_m must be nonnegative");
    }
    try {
      g.addEdge({r.fields[1], r.fields[2], d, r.fields[3] == "one_way", unknown,
                 r.fields[0]});
    } catch (const std::exception& x) {
      throw std::runtime_error(ep + ":" + std::to_string(r.line) + ": " +
                               x.what());
    }
  }
  return g;
}
}  // namespace campus
