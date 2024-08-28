#ifndef Indianroute_H
#define Indianroute_H

#include <string>
#include "Location.h"

using namespace std;

class Location;

const float PLANE_COST_MULTIPLIER = 3.0f;

class Route {
public:
    Location* origin;
    Location* destination;

    string originName;
    string destinationName;

    string transportMode;
    float duration;
    float cost;
    string additionalInfo;

    Route();
    ~Route();
    Route(Location* origin, Location* destination);
    Route(Location* origin, Location* destination, const string& transportMode, float duration, float cost, const string& additionalInfo);

    bool connects(Location* start, Location* end) const;
};

Route::Route()
    : origin(nullptr), destination(nullptr), transportMode(""), duration(0), cost(0), additionalInfo("") {}

Route::~Route() {}

Route::Route(Location* origin, Location* destination)
    : origin(origin), destination(destination), transportMode(""), duration(0), cost(0), additionalInfo("") {}

Route::Route(Location* origin, Location* destination, const string& transportMode, float duration, float cost, const string& additionalInfo)
    : origin(origin), destination(destination), transportMode(transportMode), duration(duration), cost(cost), additionalInfo(additionalInfo) {
    if (transportMode == "plane") {
        this->cost *= PLANE_COST_MULTIPLIER;
    }
}

bool Route::connects(Location* start, Location* end) const {
    return start == origin && end == destination;
}

#endif
