# Cities Travel Planner
## Overview
The Indian Cities Travel Planner is a C++ application designed to compute the shortest travel route between cities in India using Dijkstra's algorithm. 
The application processes city and route data to determine the most efficient path based on user preferences, either by travel cost or time.
It then generates a visual representation of the route in an HTML file using Google Maps.

## Features
Shortest Path Calculation: Utilizes Dijkstra's algorithm to find the optimal travel route between cities.
Cost and Time Preferences: Allows users to choose between finding the cheapest or fastest route.
Map Visualization: Generates an interactive HTML map with routes and city markers.

### Components
#### Source Files

##### Main.cpp

The entry point of the application.
Handles command-line arguments, parses input files, initializes the graph, and performs the shortest path computation.
Generates an HTML file for visualizing the shortest path on Google Maps.

##### Graphfunctiontocalculate.h

Defines the Graph class which manages cities and routes.
Implements Dijkstra's algorithm to calculate the shortest path based on user preferences.
Provides methods to retrieve adjacent locations and routes, and to reconstruct the shortest path.
##### Indianlocation.h

Defines the Location class representing a city.
Includes attributes such as country, capital, latitude, longitude, and associated routes.
Supports comparison operations to facilitate priority queue operations in Dijkstra's algorithm.
##### Indianroute.h

Defines the Route class representing a route between two locations.
Includes attributes such as origin, destination, transport type, time, cost, and additional notes.
Provides a method to check if a route connects two specific locations.
##### FileOperationsfortravel.h

Contains functions for parsing input files.
locationParser Function: Reads city data from a CSV file and creates Location objects.
routeParser Function: Reads route data from a CSV file and creates Route objects.
