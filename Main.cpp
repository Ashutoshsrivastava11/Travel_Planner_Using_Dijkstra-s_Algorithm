#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stack>

#include "FileOperations.h"
#include "Route.h"
#include "GraphFunctions.h"

using namespace std;

int main(int argc, char* argv[]) {
    string citiesFilename;
    string routesFilename;
    string outputFilename;
    string originCity;
    string destinationCity;
    string preference;
    bool useCost; // true for cheapest, false for fastest

    // Parse command-line arguments or prompt for input
    if (argc > 1) {
        citiesFilename = argv[1];
    } else {
        cout << "Enter filename containing cities: ";
        cin >> citiesFilename;
    }

    if (argc > 2) {
        routesFilename = argv[2];
    } else {
        cout << "Enter filename containing routes: ";
        cin >> routesFilename;
    }

    if (argc > 3) {
        outputFilename = argv[3];
    } else {
        cout << "Enter filename for output (.html): ";
        cin >> outputFilename;
    }

    if (argc > 4) {
        originCity = argv[4];
    } else {
        cout << "Origin city: ";
        cin >> originCity;
    }

    if (argc > 5) {
        destinationCity = argv[5];
    } else {
        cout << "Destination city: ";
        cin >> destinationCity;
    }

    if (argc > 6) {
        preference = argv[6];
    } else {
        cout << "Enter preference (fastest/cheapest): ";
        cin >> preference;
    }

    if (preference == "cheapest") {
        useCost = true;
    } else if (preference == "fastest") {
        useCost = false;
    } else {
        cout << "Invalid preference entered" << endl;
        return 1;
    }

    // Create the graph
    Graph graph(citiesFilename, routesFilename);

    // Check if origin and destination are valid
    if (graph.getCity(originCity) == nullptr || graph.getCity(destinationCity) == nullptr) {
        cout << "Invalid city entry" << endl;
        return 1;
    }

    // Run Dijkstra's algorithm
    graph.Dijkstras(originCity, useCost);

    // Generate the path and route stacks
    stack<Location*> pathStack = graph.cityStacker(destinationCity);
    stack<Route*> routeStack = graph.routeStacker(destinationCity, useCost);

    // Generate the output file
    outputGenerator(outputFilename, pathStack, routeStack, useCost);

    return 0;
}
