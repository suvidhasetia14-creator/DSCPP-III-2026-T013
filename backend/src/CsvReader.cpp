#include "CsvReader.h"

#include <fstream>
#include <stdexcept>
namespace campus {
std::vector<CsvRow> CsvReader::read(const std::string& path) {
  std::ifstream f(path);
  if (!f) throw std::runtime_error("cannot open " + path);
  std::vector<CsvRow> rows;
  std::string s;
  int n = 0;
  while (std::getline(f, s)) {
    ++n;
    if (s.empty()) continue;
    std::vector<std::string> c;
    std::string x;
    bool quote = false;
    for (char ch : s) {
      if (ch == '"')
        quote = !quote;
      else if (ch == ',' && !quote) {
        c.push_back(x);
        x.clear();
      } else
        x += ch;
    }
    if (quote)
      throw std::runtime_error(path + ":" + std::to_string(n) +
                               ": unterminated quote");
    c.push_back(x);
    for (auto& v : c) {
      while (!v.empty() && (v.front() == ' ' || v.front() == '\t'))
        v.erase(v.begin());
      while (!v.empty() && (v.back() == ' ' || v.back() == '\t')) v.pop_back();
    }
    rows.push_back({n, c});
  }
  return rows;
}
}  // namespace campus
