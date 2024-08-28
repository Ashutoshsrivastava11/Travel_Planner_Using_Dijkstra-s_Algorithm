#ifndef Parsercode_H
#define Parsercode_H

#include <cstdlib>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include "Location.h"

using namespace std;

vector<Location*> locationParser(const string& filename, vector<Route*>& routes) {
    fstream locations(filename.c_str());

    if (!locations.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return {};
    }

    string country;
    string city;
    string latitude;
    string longitude;

    vector<Location*> cities;
    Location* node;

    while (getline(locations, country, ',') &&
           getline(locations, city, ',') &&
           getline(locations, latitude, ',') &&
           getline(locations, longitude)) {
        // Create a new Location object
        node = new Location(country, city, atof(latitude.c_str()), atof(longitude.c_str()));

        // Update routes with the new location
        for (Route* route : routes) {
            if (route->originS == node->capital) {
                route->origin = node;
                node->routes.push_back(route);
            }
            else if (route->destinationS == node->capital) {
                route->destination = node;
            }
        }

        // Add the location to the list
        cities.push_back(node);
    }

    cout << "Cities Parsed from: " << filename << endl;
    return cities;
}

vector<Route*> routeParser(const string& filename) {
    fstream routes(filename.c_str());

    if (!routes.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return {};
    }

    string originS;
    string destinationS;
    string type;
    string time;
    string cost;
    string note;

    vector<Route*> allRoutes;
    Route* edge;

    while (getline(routes, originS, ',') &&
           getline(routes, destinationS, ',') &&
           getline(routes, type, ',') &&
           getline(routes, time, ',') &&
           getline(routes, cost, ',') &&
           getline(routes, note)) {
        // Create a new Route object
        edge = new Route(nullptr, nullptr, type, atof(time.c_str()), atof(cost.c_str()), note);
        edge->destinationS = destinationS;
        edge->originS = originS;

        // Add the route to the list
        allRoutes.push_back(edge);
    }

    cout << "Routes Parsed from: " << filename << endl;
    return allRoutes;
}

#endif
