//defines the whole campus.
// It holds a collection of Location objects (locations) and the connections between them

#ifndef GRAPH_H
#define GRAPH_H
#include <string>
#include <vector>
#include <map>
#include "Location.h"
using namespace std;

//this class represents the campus graph: all locations and the connections (edges) between them
class Graph {
    private:
        map<string, Location> locations;//stores every Location, keyed by its name
        map<string, vector<pair<string, double>>> adjList;//adjacency list: name -> list of (connected name, distance)

    public:
        void addLocation(Location loc);//adds a new Location to the graph
        bool hasLocation(string name);//checks if a location with the given name exists in the graph
        void addEdge(string from, string to, double distance);//connects two locations with a given distance, both directions
        void printGraph();//prints every location and its connected neighbors, for debugging
        vector<string> dijkstra(string start, string end);//finds the shortest path from start to end, returns it as a list of names in order
};
#endif