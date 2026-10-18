#pragma once
#include <string>
namespace campus {
struct Edge {
  std::string from, to;
  double distance = 0;
  bool oneWay = true;
  bool unknownDistance = false;
  std::string id;
};
}  // namespace campus
