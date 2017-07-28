#ifndef ROADTRIPPING_HPP
#define ROADTRIPPING_HPP

#include <vector>

using namespace std;

//**********  LLIST ITEMS   **********
//Definition of nodes in list
struct item{
	string name;
	item * next;

	item(string n){
		name = n;
		next = nullptr;
	}
};

class llist{
private:
	item * head;

public:
	void addToBack(string newItem);     		//When adding to itineraries and adding connections we add them to the back of the list
	string backPop(); 							//Needed to reverse the direction of path that Dijksra's method calculates
	string pop();                               //Takes off the first item from the list and returns the string
	void deleteItem(string toDelete);       	//Deletes a node from the list
	int size();                                 //Returns the count of items in the list
	bool contains(string toFind);				//Bool stating whether or not an item is found within the list
	void printList();							//Prints out all of the items in the list in order

	llist();
	~llist();
};


//**********   NODE DEFINITIONS   **********
//Graph nodes
struct City;
struct Adjacent{
	City * neighbor;
	int weight;

	Adjacent(City * n, int w){
		neighbor = n;
		weight = w;
	}
};

struct City{
	string name;
	vector<Adjacent*> neighbors;

	bool visited;
	bool inPath;
	int distance;

	City * previous;
	llist * activities;

	City(string n){
		name = n;
		visited = false;
		inPath = false;
		previous = nullptr;
		activities = new llist();
	}
};


//**********   GRAPH ITEMS   **********
class Graph{
private:
	vector<City *> myCities;
	llist * myItinerary;
	llist * solvedPath;
	int pathDistance;

	void insertCity(City * newCity);                                //Adds a city of the desired name to the graph
	void populateCityActivities(City * theCity, string list);		//Takes a city and the unparsed list of activies and turns it into a list of activities
	City * Dijkstra(string starting, string destination);        	//Finds the shortest path to destination. Be it an activity or a city

public:
	//Graph based functions
	City* search(string name);										//Returns the city with the given name
	void insertEdge(City* startCity, City* endCity, int weight);    //Adds an edge between the two cities of the desired weight
	void deleteCity(string name);                                   //Deletes the requested city from the graph
	void deleteEdge(City* startCity, City* endCity);                //Delete a connection between two cities
 	bool buildGraph(string fileName);                               //Takes a file name from the user and uses it to build the graph
 	void printNeighbors(City * theCity);							//Goes through each adjacent city and prints it's name and distance
 	void printGraph();                                              //Prints out each city. all of that city's activities, and all of its neighbors


 	//Itinerary based functions
 	bool buildItinerary(string fileName);                           //Takes a file name and uses it to build a linked list of items I want to do
 	void addItinerary(string toAdd);								//Adds an item to the itinerary
 	void removeItinerary(string toAdd);								//Removes an item from the itinerary
 	bool contains(City * theCity, string activity);					//Calls the list contains function with the itinerary
 	void pathLinker();												//Calls the Dijkstra function for each of the items in the itinerary
 	void printItinerary();											//Calls the printlist function on the Itinerary

	Graph();
	~Graph();
};


#endif