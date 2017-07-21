# CSCI2270-GraphingFinal
This is the repository for the final project in my class CSCI-2270 Data Structures.

The program, roadTripping.cpp, will read in a text file, and use the information in that file to create a graph of cities we can visit on a road trip.  Each of these cities will contain a linked list of possible activities that one can partake in at said city. For example one can go bowling in Boulder.  After creating the graph, the program will display a menu for the user allowing them to pick one of four options. They can add an activity to a city, remove a city, print the graph, or try to load an itinerary. An itinerary is a text file which contains a list of events that one would like to do in order. The first element must be a starting city. An example itinerary would be 
Boulder
hiking
Denver
minigolf
Colorado Springs
The code reads this file into a queue, and attempts to construct a path through the graph doing all of the activities in order without repeating any nodes. If it finds that it cannot complete the queue with its current structure, it will retrace its steps and attempt to find a solution where the conflict can be avoided.  After completing, the code will either list the path of cities that accomplish the route, or it will tell the user that the requested path is impossible to create.
