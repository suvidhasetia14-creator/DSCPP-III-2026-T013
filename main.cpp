#include "Graph.h"
#include <iostream>
using namespace std;

int main() {

    Graph campus;

    // --- Locations ---
    campus.addLocation(Location("Gate 2", 30.268764, 77.997777, "Main entrance"));
    campus.addLocation(Location("Bike Parking", 30.268444, 77.997124, ""));
    campus.addLocation(Location("Dept. of Mechanical Engg", 30.268780, 77.997150, ""));
    campus.addLocation(Location("Dept. of Management Studies", 30.268450, 77.996850, ""));
    campus.addLocation(Location("Dept. of Hotel Management", 30.268094, 77.996562, ""));
    campus.addLocation(Location("Petroleum Engg Block", 30.267794, 77.996388, ""));
    campus.addLocation(Location("Quick Bite Cafe", 30.268007, 77.996565, ""));
    campus.addLocation(Location("Old MCA Block", 30.267485, 77.996573, ""));
    campus.addLocation(Location("MCA Block", 30.267624, 77.996956, ""));
    campus.addLocation(Location("Param Computer Lab", 30.267550, 77.995983, ""));
    campus.addLocation(Location("Santoshanand Library", 30.267541, 77.995705, ""));
    campus.addLocation(Location("Dept. of Electrical Engg", 30.267551, 77.995454, ""));
    campus.addLocation(Location("GEU Main Block", 30.267547, 77.995093, ""));
    campus.addLocation(Location("Digital Lab", 30.267375, 77.995665, ""));
    campus.addLocation(Location("Basketball Court", 30.267050, 77.996018, ""));
    campus.addLocation(Location("Civil Block", 30.266790, 77.995485, ""));
    campus.addLocation(Location("Badminton Court", 30.266906, 77.995051, ""));
    campus.addLocation(Location("Graphic Era Main Ground", 30.268115, 77.994472, ""));
    campus.addLocation(Location("CS/IT Block", 30.268884, 77.993421, ""));

    // --- Edges (distances in meters) ---
    campus.addEdge("Gate 2", "Bike Parking", 72);
    campus.addEdge("Bike Parking", "Dept. of Mechanical Engg", 37);
    campus.addEdge("Dept. of Mechanical Engg", "Dept. of Management Studies", 47);
    campus.addEdge("Dept. of Management Studies", "Dept. of Hotel Management", 48);
    campus.addEdge("Dept. of Hotel Management", "Petroleum Engg Block", 37);
    campus.addEdge("Petroleum Engg Block", "Quick Bite Cafe", 29);
    campus.addEdge("Quick Bite Cafe", "Old MCA Block", 58);
    campus.addEdge("Old MCA Block", "MCA Block", 40);
    campus.addEdge("MCA Block", "Param Computer Lab", 94);
    campus.addEdge("Param Computer Lab", "Santoshanand Library", 27);
    campus.addEdge("Param Computer Lab", "Dept. of Electrical Engg", 51);
    campus.addEdge("Dept. of Electrical Engg", "GEU Main Block", 35);
    campus.addEdge("Santoshanand Library", "Digital Lab", 19);
    campus.addEdge("Digital Lab", "Basketball Court", 50);
    campus.addEdge("Basketball Court", "Civil Block", 59);
    campus.addEdge("Civil Block", "Badminton Court", 44);
    campus.addEdge("Santoshanand Library", "Dept. of Management Studies", 149);
    campus.addEdge("Digital Lab", "Dept. of Management Studies", 165);
    campus.addEdge("Basketball Court", "Dept. of Management Studies", 175);
    campus.addEdge("Civil Block", "Dept. of Management Studies", 226);
    campus.addEdge("Badminton Court", "Dept. of Management Studies", 244);
    campus.addEdge("Santoshanand Library", "GEU Main Block", 59);
    campus.addEdge("GEU Main Block", "Graphic Era Main Ground", 87);
    campus.addEdge("GEU Main Block", "Badminton Court", 72);
    campus.addEdge("GEU Main Block", "Basketball Court", 105);
    campus.addEdge("Graphic Era Main Ground", "Gate 2", 326);
    campus.addEdge("Graphic Era Main Ground", "CS/IT Block", 132);

    // --- Take user input ---
    string start, destination;

    cout << "Enter your starting location: ";
    getline(cin, start);

    cout << "Enter your destination: ";
    getline(cin, destination);

    // --- Basic validation ---
    if (!campus.hasLocation(start) || !campus.hasLocation(destination)) {
        cout << "One or both locations were not recognized. Please check spelling and try again." << endl;
        return 1;
    }

    // --- Run Dijkstra ---
    vector<string> path = campus.dijkstra(start, destination);

    cout << "\nShortest Route:\n";
    for (string loc : path) {
        cout << loc << " -> ";
    }
    cout << "END" << endl;

    return 0;
}