#pragma once
#include <string>
namespace campus {
struct Node {
  std::string id, name, type;
  double x = 0, y = 0;
  std::string notes;
  bool isJunction() const { return type == "junction"; }
};
}  // namespace campus
