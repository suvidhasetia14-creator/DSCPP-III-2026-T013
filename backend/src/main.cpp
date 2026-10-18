#include <iostream>

#include "ApiServer.h"
#include "Config.h"
#include "DataLoader.h"
int main(int argc, char** argv) {
  try {
    std::string root = argc > 1 ? argv[1] : ".";
    int port = argc > 2 ? std::stoi(argv[2]) : campus::config::defaultPort;
    auto g = campus::loadData(root + "/" + campus::config::nodesFile,
                              root + "/" + campus::config::edgesFile);
    campus::runApiServer(g, root + "/" + campus::config::webRoot, port);
  } catch (const std::exception& e) {
    std::cerr << "Startup error: " << e.what() << '\n';
    return 1;
  }
  return 0;
}
