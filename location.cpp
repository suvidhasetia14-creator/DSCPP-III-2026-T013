#include "Location.h"

Location::Location(string n, double lat, double lon, string desc) {
    name = n;
    latitude = lat;
    longitude = lon;
    description = desc;
}

string Location::getName() {
    return name;
}

double Location::getLatitude() {
    return latitude;
}

double Location::getLongitude() {
    return longitude;
}

string Location::getDescription() {
    return description;
}