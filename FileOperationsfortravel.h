#ifndef FileOperationsfortravel_h
#define FileOperationsfortravel_h

#include <cstdlib>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <stack>
#include <sstream>

#include "Location.h"

using namespace std;

/**
 * Parses a file containing location data and returns a vector of Location pointers.
 */
vector<Location*> parseLocations(const string& filename, const vector<Route*>& routeList) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return {};
    }

    vector<Location*> locationList;
    string line, countryName, cityName, latStr, lonStr;

    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, countryName, ',');
        getline(ss, cityName, ',');
        getline(ss, latStr, ',');
        getline(ss, lonStr);

        Location* newLocation = new Location(countryName, cityName, stof(latStr), stof(lonStr));

        for (auto& route : routeList) {
            if (route->originS == newLocation->cityName) {
                route->origin = newLocation;
                newLocation->routes.push_back(route);
            } else if (route->destinationS == newLocation->cityName) {
                route->destination = newLocation;
            }
        }

        locationList.push_back(newLocation);
    }

    cout << "Locations parsed from: " << filename << endl;
    return locationList;
}

/**
 * Parses a file containing route data and returns a vector of Route pointers.
 */
vector<Route*> parseRoutes(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return {};
    }

    vector<Route*> routeList;
    string line, startPoint, endPoint, transportType, travelTime, travelCost, additionalInfo;

    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, startPoint, ',');
        getline(ss, endPoint, ',');
        getline(ss, transportType, ',');
        getline(ss, travelTime, ',');
        getline(ss, travelCost, ',');
        getline(ss, additionalInfo);

        Route* newRoute = new Route(nullptr, nullptr, transportType, stof(travelTime), stof(travelCost), additionalInfo);
        newRoute->destinationS = endPoint;
        newRoute->originS = startPoint;

        routeList.push_back(newRoute);
    }

    cout << "Routes parsed from: " << filename << endl;
    return routeList;
}

/**
 * Generates an HTML file displaying route information using Google Maps API.
 */
void generateMapOutput(const string& filename, stack<Location*> locationStack, stack<Route*> routeStack, bool prioritizeCost) {
    ofstream output(filename);
    if (!output.is_open()) {
        cerr << "Error opening file for writing: " << filename << endl;
        return;
    }

    output << "<HTML><HEAD><TITLE>Route Visualization</TITLE></HEAD>"
           << "<script type='text/javascript' src='http://maps.google.com/maps/api/js?sensor=false'></script>"
           << "<script>function initialize() { "
           << "var mapOptions = { zoom: 3, center: new google.maps.LatLng(0, 0), mapTypeId: google.maps.MapTypeId.ROADMAP };"
           << "var map = new google.maps.Map(document.getElementById('map'), mapOptions);\n";

    int markerIndex = 0;
    int polylineIndex = 0;

    while (!locationStack.empty() && !routeStack.empty()) {
        Location* startLocation = locationStack.top(); locationStack.pop();
        Location* endLocation = locationStack.top(); locationStack.pop();
        Route* currentRoute = routeStack.top(); routeStack.pop();

        output << "var marker" << markerIndex++ << " = new google.maps.Marker({ "
               << "position: new google.maps.LatLng(" << startLocation->latitude << ", " << startLocation->longitude << "), "
               << "map: map, title: \"" << startLocation->cityName << ", " << startLocation->country << "\" });\n";
        
        output << "var marker" << markerIndex++ << " = new google.maps.Marker({ "
               << "position: new google.maps.LatLng(" << endLocation->latitude << ", " << endLocation->longitude << "), "
               << "map: map, title: \"" << endLocation->cityName << ", " << endLocation->country << "\" });\n";

        float adjustedCost = (currentRoute->transportType == "plane") ? currentRoute->cost / MULTI : currentRoute->cost;

        output << "var infoContent" << polylineIndex << " = \"" << startLocation->cityName << ", " << startLocation->country
               << " --> " << endLocation->cityName << ", " << endLocation->country
               << " (" << currentRoute->transportType << " - " << currentRoute->travelTime << " hours - $" << adjustedCost << ")\"; "
               << "var routePath" << polylineIndex << " = new google.maps.Polyline({ "
               << "path: [new google.maps.LatLng(" << startLocation->latitude << ", " << startLocation->longitude << "), "
               << "new google.maps.LatLng(" << endLocation->latitude << ", " << endLocation->longitude << ")], "
               << "strokeColor: '#0000FF', strokeOpacity: 1.0, strokeWeight: 2 }); "
               << "routePath" << polylineIndex << ".setMap(map); "
               << "google.maps.event.addListener(routePath" << polylineIndex << ", 'click', function() { alert(infoContent" << polylineIndex << "); });\n";
        
        ++polylineIndex;
    }

    output << "} google.maps.event.addDomListener(window, 'load', initialize); </script>"
           << "<BODY><div id='map' style='width:100%;height:100%;'></div></BODY></HTML>";

    cout << "Output file generated: " << filename << endl;
}

#endif
