#include "Graph.h"
#include <iostream>
#include <queue>
#include <limits>
#include <algorithm>

//this class represents the campus graph: all locations and the connections (edges) between them
void Graph::addLocation(Location loc) {
    locations.insert({loc.getName(), loc});
}

bool Graph::hasLocation(string name) {
    return locations.find(name) != locations.end();
}

//connects two locations with a given distance, both directions
void Graph::addEdge(string from, string to, double distance) {
    adjList[from].push_back({to, distance});
    adjList[to].push_back({from, distance});
}

//prints every location and its connected neighbors, for debugging
void Graph::printGraph() {
    for (auto const& pair : adjList) {
        cout << pair.first << " connects to:" << endl;
        for (auto const& neighbor : pair.second) {
            cout << "  -> " << neighbor.first << " (" << neighbor.second << "m)" << endl;
        }
    }
}

//tihs function implements Dijkstra's algorithm to find the shortest path from start to end. It returns the path as a vector of location names in order.
vector<string> Graph::dijkstra(string start, string end) {//this accepts the names of the start and end locations, and returns a vector of names representing the shortest path from start to end
    map<string, double> dist;//stores the shortest known distance from start to each location
    map<string, string> prev;//stores the previous location in the shortest path for each location

    for (auto const& pair : locations) {//initialize distances to infinity
        dist[pair.first] = numeric_limits<double>::infinity();
    }
    dist[start] = 0;//distance to start is 0

    priority_queue<pair<double, string>, vector<pair<double, string>>, greater<pair<double, string>>> pq;//min-heap priority queue to explore the closest location next
    pq.push({0, start});
     
    //main loop of Dijkstra's algorithm
    while (!pq.empty()) {//while there are still locations to explore
        double currentDist = pq.top().first;//get the closest location
        string current = pq.top().second;//get the name of the closest location
        pq.pop();//remove it from the queue

        if (current == end) break;//if we reached the destination, we can stop
        if (currentDist > dist[current]) continue;//if we found a better path to current, skip it

        for (auto const& neighbor : adjList[current]) {//for each neighbor of the current location
            string neighborName = neighbor.first;//get the name of the neighbor
            double edgeWeight = neighbor.second;//get the distance to the neighbor
            double newDist = currentDist + edgeWeight;//calculate the new distance to the neighbor

            if (newDist < dist[neighborName]) {//if the new distance is shorter, update it
                dist[neighborName] = newDist;//update the shortest distance to the neighbor
                prev[neighborName] = current;//update the previous location for the neighbor
                pq.push({newDist, neighborName});
            }
        }
    }
     
    //reconstruct the shortest path from start to end using the prev map
    vector<string> path;
    string at = end;
    while (prev.find(at) != prev.end()) {
        path.push_back(at);
        at = prev[at];
    }
    path.push_back(start);
    reverse(path.begin(), path.end());

    return path;
}