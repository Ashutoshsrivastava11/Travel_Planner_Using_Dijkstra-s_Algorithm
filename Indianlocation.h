#ifndef Indianlocation_h
#define Indianlocation_h

#include <string>
#include <vector>

#include "Route.h"

using namespace std;

class Location {
public:
    string country;
    string capital;
    float latitude;
    float longitude;

    vector<Route*> routes;

    bool isActive;
    Location* previousLocation;
    float distanceFromStart;

    Location();
    ~Location();
    Location(const string& country, const string& capital);
    Location(const string& country, const string& capital, float latitude, float longitude);

    float calculateDistanceTo(const Location* destination) const;

    bool operator==(const Location& other) const;
    bool operator<(const Location& other) const;
    bool operator>(const Location& other) const;
};

Location::Location()
    : country(""), capital(""), latitude(0), longitude(0),
      distanceFromStart(numeric_limits<float>::max()), isActive(true), previousLocation(nullptr) {}

Location::~Location() {}

Location::Location(const string& country, const string& capital)
    : country(country), capital(capital), latitude(0), longitude(0),
      distanceFromStart(numeric_limits<float>::max()), isActive(true), previousLocation(nullptr) {}

Location::Location(const string& country, const string& capital, float latitude, float longitude)
    : country(country), capital(capital), latitude(latitude), longitude(longitude),
      distanceFromStart(numeric_limits<float>::max()), isActive(true), previousLocation(nullptr) {}

float Location::calculateDistanceTo(const Location* destination) const {
    // Calculate the distance to another location (example using Euclidean distance for simplicity)
    float latDiff = latitude - destination->latitude;
    float lonDiff = longitude - destination->longitude;
    return sqrt(latDiff * latDiff + lonDiff * lonDiff);
}

bool Location::operator==(const Location& other) const {
    return capital == other.capital;
}

bool Location::operator<(const Location& other) const {
    return distanceFromStart < other.distanceFromStart;
}

bool Location::operator>(const Location& other) const {
    return distanceFromStart > other.distanceFromStart;
}

class CompareLocation {
public:
    bool operator()(const Location* l1, const Location* l2) const {
        return l1->distanceFromStart > l2->distanceFromStart;
    }
};

#endif
