#include <cassert>

#include "Graph.h"
int main() {
  campus::Graph g;
  g.addNode({"main_ground", "Main", "landmark", 0, 0, ""});
  g.addNode({"junction", "J", "junction", 0, 0, ""});
  g.addNode({"end", "E", "building", 0, 0, ""});
  g.addEdge({"main_ground", "junction", 1, true, false, "e1"});
  g.addEdge({"junction", "end", 2, false, false, "e2"});
  auto r = g.shortestPath("main_ground", "end");
  assert(r.found && r.distance == 3);
  assert(!g.shortestPath("end", "main_ground").found);
  return 0;
}
