#ifndef Graphfunctiontocalculate_H
#define Graphfunctiontocalculate_H

#include <string>
#include <vector>
#include <iostream>
#include <queue>
#include <cstdlib>
#include <stack>
#include <cmath>

#include "Location.h"
#include "Route.h"
#include "FileOperations.h"

using namespace std;

class Graph {
public:
    vector<Location*> locations;
    vector<Route*> routes;

    int numLocations;

    Graph(const string& nodesFile, const string& edgesFile);

    int findLocationIndex(const string& name) const;
    Location* findLocation(const string& name) const;

    float calculateWeight(const string& startName, const string& endName, bool byCost) const;
    float calculateWeight(Location* start, Location* end, bool byCost) const;

    void applyDijkstra(const string& startName, bool byCost);

    vector<Location*>* getAdjacentLocations(Location* location) const;
    Route* findRoute(Location* start, bool byCost, float accumulatedDistance) const;

    vector<Route*>* getAdjacentRoutes(Location* location) const;

    stack<Location*> buildLocationStack(const string& destinationName) const;
    stack<Route*> buildRouteStack(const string& destinationName, bool byCost) const;
};

Graph::Graph(const string& nodesFile, const string& edgesFile) {
    routes = parseRoutes(edgesFile);
    locations = parseLocations(nodesFile, routes);
    numLocations = locations.size();
}

int Graph::findLocationIndex(const string& name) const {
    for (int i = 0; i < locations.size(); ++i) {
        if (locations[i]->cityName == name) {
            return i;
        }
    }
    return -1;
}

Location* Graph::findLocation(const string& name) const {
    int index = findLocationIndex(name);
    return (index != -1) ? locations[index] : nullptr;
}

float Graph::calculateWeight(const string& startName, const string& endName, bool byCost) const {
    Location* start = findLocation(startName);
    Location* end = findLocation(endName);

    for (const auto& route : routes) {
        if (route->connects(start, end)) {
            return byCost ? route->cost : route->time;
        }
    }
    return -1;
}

float Graph::calculateWeight(Location* start, Location* end, bool byCost) const {
    for (const auto& route : routes) {
        if (route->connects(start, end)) {
            return byCost ? route->cost : route->time;
        }
    }
    return -1;
}

void Graph::applyDijkstra(const string& startName, bool byCost) {
    Location* start = findLocation(startName);
    if (!start) return;

    start->distanceFromStart = 0;

    auto compare = [](Location* a, Location* b) { return a->distanceFromStart > b->distanceFromStart; };
    priority_queue<Location*, vector<Location*>, decltype(compare)> minHeap(compare);

    for (const auto& loc : locations) {
        minHeap.push(loc);
    }

    while (!minHeap.empty()) {
        while (!minHeap.empty() && !minHeap.top()->exists) {
            minHeap.pop();
        }

        if (minHeap.empty()) return;

        Location* current = minHeap.top();
        minHeap.pop();
        current->exists = false;

        vector<Location*>* adjLocations = getAdjacentLocations(current);
        for (const auto& neighbor : *adjLocations) {
            float distance = calculateWeight(current, neighbor, byCost) + current->distanceFromStart;

            if (distance < neighbor->distanceFromStart) {
                neighbor->distanceFromStart = distance;
                neighbor->previous = current;
                minHeap.push(neighbor);
            }
        }
        delete adjLocations;
    }
}

vector<Location*>* Graph::getAdjacentLocations(Location* location) const {
    auto* adjLocations = new vector<Location*>();
    for (const auto& route : location->routes) {
        if (route->destination->exists) {
            adjLocations->push_back(route->destination);
        }
    }
    return adjLocations;
}

vector<Route*>* Graph::getAdjacentRoutes(Location* location) const {
    auto* adjRoutes = new vector<Route*>();
    for (const auto& route : routes) {
        if (route->origin->cityName == location->cityName) {
            adjRoutes->push_back(route);
        }
    }
    return adjRoutes;
}

Route* Graph::findRoute(Location* start, bool byCost, float accumulatedDistance) const {
    vector<Route*>* adjRoutes = getAdjacentRoutes(start);

    const float epsilon = 1e-5;
    for (const auto& route : *adjRoutes) {
        float routeDistance = byCost ? route->cost : route->time;
        if (fabs(accumulatedDistance - routeDistance - route->origin->distanceFromStart) < epsilon) {
            delete adjRoutes;
            return route;
        }
    }

    delete adjRoutes;
    return nullptr;
}

stack<Location*> Graph::buildLocationStack(const string& destinationName) const {
    Location* destination = findLocation(destinationName);
    stack<Location*> locationStack;

    while (destination) {
        locationStack.push(destination);
        destination = destination->previous;
    }

    return locationStack;
}

stack<Route*> Graph::buildRouteStack(const string& destinationName, bool byCost) const {
    stack<Route*> routeStack;
    Location* destination = findLocation(destinationName);
    float accumulatedDistance = destination->distanceFromStart;

    while (destination->previous) {
        Route* route = findRoute(destination->previous, byCost, accumulatedDistance);
        if (route) {
            routeStack.push(route);
            destination = destination->previous;
            accumulatedDistance = destination->distanceFromStart;
        } else {
            break;
        }
    }

    return routeStack;
}

#endif
