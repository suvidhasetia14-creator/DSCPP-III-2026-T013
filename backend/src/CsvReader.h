#pragma once
#include <string>
#include <vector>
namespace campus {
struct CsvRow {
  int line;
  std::vector<std::string> fields;
};
class CsvReader {
 public:
  static std::vector<CsvRow> read(const std::string&);
};
}  // namespace campus
