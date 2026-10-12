//defines a single location (a building or a floor). Just data + getters: name, latitude, longitude, description. 
//It doesn't know about the graph, edges, or Dijkstra at all — it only knows about itself.



#ifndef LOCATION_H
#define LOCATION_H
#include <string>
using namespace std;

//this class represents a location with a name
class Location {
    private:
        string name;//the name of the location
        double latitude;//the latitude of the location
        double longitude;//the longitude of the location
        string description;//the description of the location

    public:
        Location(string n, double lat, double lon, string desc);//constructor: builds a Location from the four values above

        string getName();//returns the location's name
        double getLatitude();//returns the location's latitude
        double getLongitude();//returns the location's longitude
        string getDescription();//returns the location's description
};
#endif