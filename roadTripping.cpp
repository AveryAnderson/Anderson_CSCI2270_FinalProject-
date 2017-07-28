#include <iostream>
#include <fstream>
#include <sstream>


#include "roadTripping.hpp"

/************************         LINKED LIST IMPLEMENTATION FUNCTIONS          ************************/

//If I want to add items to the end of my linked list, like when I read them from my itinerary file, I call this function
void llist::addToBack(string newItem){
	//First make sure that the list actually has a head pointer.
	if(head){

		//Make sure the head isn't the value we want to add to the list
		if(head->name == newItem){
			cout<<"That item is already in the list!"<<endl;
			return;
		}

		//Loop through the rest of the list to make sure we aren't adding a repeat
		item * runner = head;
		while(runner->next){
			if(runner->next->name == newItem){
				cout<<"That item is already in the list!"<<endl;
				return;
			}
			runner = runner->next;
		}

		//Leaving the loop means that the item we seek to add is not already in the list. Also runner now acts as a tail pointer as it is immediately before the null pointer.
		//For that reason we can simply make a new item and add it after the runner.

		runner->next = new item(newItem);
	}
	//If the list is not already initialized, this must be the first item in thel list.
	else{
		head = new item(newItem);
	}	
}

//When getting the shortest path, I need to reverse a list. This process is much easier if I can pull from the back of a list as well as the front. This function
//returns the string from the item at the tail of a list.
string llist::backPop(){
	string temp;
	item * runner = head;

	//If there is no list to pull from say so
	if(!runner) return "NULLPTR";

	//If I only have one node, get it's string, set the head to null and delete the item
	if(!runner->next){
		temp = runner->name;
		deleteItem(temp);
		head = nullptr;
	}

	//Otherwise find the tail, then get its name. Delete it, and remove the pointer to it.
	else{

		while(runner->next->next ){
			runner = runner->next;
		}

		temp = runner->next->name;
		deleteItem(temp);
		runner->next = nullptr;
	}

	//Return the string formed by the above functions
	return temp;
}

//This function returns the top node of the llist, so I can treat the list as if it were a queue.
string llist::pop(){
	//If there is no list to pop from, do nothing
	if(!head){
		cout<<"The list is empty, there is nothing to pop"<<endl;
		return "";
	}
	else{
		//Otherwise store the name and the node in temporary variables
		string returnString = head->name;

		//Delete the old head. The deleteItem function takes care of moving the head pointer for me.
		deleteItem(head->name);

		return returnString;
	}
}

//This function handles the process of deleting a node from the llist.
void llist::deleteItem(string toDelete){
	//If there is no list to delete from do nothing
	if(!head){
		cout<<"The list is empty, there is nothing to delete"<<endl;
		return;
	}

	//If I am deleting the head, be sure to move the head
	if(head->name == toDelete){
		item * temp = head;
		head = head->next;
		delete temp;
		return;
	}

	//If it wasn't the head, loop through the list looking for the node we want to delete
	item * runner = head;
	while(runner->next){
		if(runner->next->name == toDelete){
			item * temp = runner->next;
			runner->next = runner->next->next;
			delete temp;
			return;
		}
		runner = runner->next;
	}

	//If I didn't find the name in the list, say as much and end the function
	cout<<"The item \""<<toDelete<<"\" was not in the list. Are you sure you spelled it correctly?"<<endl;
}

//This function returns the ammount of items in the llist
int llist::size(){
	//Make a counter and and increase it for each value in the list and return
	int count = 0;
	item * runner = head;
	while(runner){
		count++;
		runner = runner->next;
	}
	return count;
}

//Search run through the list, if toFind is in the list return true. If I make it to the end return false
bool llist::contains(string toFind){
	item * runner = head;
	while(runner){
		if(runner->name == toFind) return true;
		runner = runner->next;
	}

	return false;
}

//Go to each item in the list and print it's name
void llist::printList(){
	item * runner = head;
	while(runner){
		cout<<runner->name<<endl;
		runner = runner->next;
	}
}

//Itinitialize the head to be a nullptr
llist::llist(){
	head = nullptr;
}

//While head is not a nullptr, continue to delete the head.
llist::~llist(){
	while(head){
		deleteItem(head->name);
	}
}

/************************         GRAPH  IMPLEMENTATION FUNCTIONS          ************************/
//Private Functions:

//After a city pointer has been created by another function, add it by first making sure its not already in my vector, and then pusing it into the vector
void Graph::insertCity(City * newCity){
	//First make sure the city is not in the graph
	if(search(newCity->name) != nullptr) {
		cout<<"The city of "<<newCity->name<<" is already in the map. The duplicate was not added."<<endl;
		return;
	}

	myCities.push_back(newCity);
}

//This function parses a string of activities separated by commas and stores them inan llist for the city.
void Graph::populateCityActivities(City * theCity, string list){
	list = list+ '\0';
	//The list of activities is parsed separated by commas, so if I hunt for commas I can separate out the list.
	char findComma = list[0]; 

	//While I haven't found the end of the string keep running.
	while(findComma != '\0'){

		int i = 0;
		findComma = list[0];

		//Iterate i until I'm either at the end of the string or am at another comma
		while(findComma!=',' && findComma != '\0'){
			findComma = list[i];
			i++;	
		}

		//Once I know the comma's location I store the name in my item, and can remove name information from
		//The string line
		string newItem = list.substr(0,i-1);
		list = list.substr(i);

		//Add the newly discovered item to the city's llist
		theCity->activities->addToBack(newItem);
	}
}

//My modified version of Dijkstra's algorithm. It will find the shortest path between a starting city and an ending location. The end can be either
//a city or an actiity.
City * Graph::Dijkstra(string starting, string endFlag){
    
    //First search through the list of cities to find the start and ending cities. Also initialize a boolean to tell me what sort of search I am operating 
    // and initialize a vector to hold the cities that Dijkstra's will "solve"
    City * start = search(starting);
    City * end =search(endFlag);
    bool activitySearch = false;
    vector<City*> solved;

    //The process needs to begin at a city, therefore if I failed to find a start city the input must be invalid and we should terminate the function
    if(!start){
    	cout<<"Error finding starting city";
    	return nullptr;
    }

    //If I failed to find an ending city, I must be searching for an activity. Make a holder city to hold in the end pointer and change the bool.
   	if(!end){    
    	end = new City(endFlag);
    	activitySearch = true;
    }

    //Every time we run this function we want to reset all of the cities, make sure their distances are arbitrarily large, visited is false, and previous is nullptr.
 	for(int i =0; i<myCities.size(); i++){
    	myCities[i]->distance = 999999;
    	myCities[i]->visited = false;
     	myCities[i]->previous = nullptr;
    }

    //Only the start should be visited at the start. Set its distance to be zero and visited to be true. Then add it to the solved vector
    start->distance = 0;
    start->visited = true;
    start->previous = nullptr;
    solved.push_back(start);

    //Initialize the temp variables for Dijkstra's algorithm
    City * closest;
    int min;
    City * parent;
    
    //Run Dijkstra's algortithm until we either find the city or the activity.
    //If we are doing a city run we terminate when end->visited == true
    //If we are doing an activity run we terminate as soon as we have added a city which contains the desired activity
    //However we also have to make sure the node containing the activity is not the start node as we don't want to double use that city.
   	while(end->visited == false && (!contains(solved.back(),end->name) || start == solved.back())){
   		//Initialize the temp variables to useless information so we don't reuse information/don't acidentaly segfault if we get trapped in a corner
       	min = 9999999;
       	closest = start;
       	parent = nullptr;
        
        //Loop through the solved nodes, and find the next closest node that is neither in my solved vertex nor i the larger solvedPath. Store said
        // closest node's information in the temp variables. This operates just as the typical Dijkstra's algorithm does, with the addition of a check
        // to make sure we don't use a city that was used in a previous leg of the overarching roadtrip.
       	for(int i = 0; i<solved.size(); i++){
   	        int distance;
            
       	    for(int j = 0; j< solved[i]->neighbors.size(); j++){
           		distance = solved[i]->distance;
               	if(!(solved[i]->neighbors[j]->neighbor->visited) && !(solved[i]->neighbors[j]->neighbor->inPath)){
                   	distance += solved[i]->neighbors[j]->weight;
               	 	if(distance < min){
               	        min = distance;
           	            closest = solved[i]->neighbors[j]->neighbor;
       	                parent = ((solved[i]));                        
                   	}
               	}
           	}
       	}

       	//Put all of the temp variable information into the next closest city
       	closest->distance = min;
   	    closest->previous = parent;
        closest->visited = true;
        
        //Optional cout statement used for debugging purposes
   		//cout<<"ADDING "<<closest->name<<endl<<endl;
   	 
        //Add this next closest city to the solved vector
       	solved.push_back((closest));
        
        //If for whatever reason a path cannot be completed, this function will continue adding cities to the vector forever. As we know the amount of
        //cities we can possibly visit, if we find that we have added more solved cities than there are cities, there must be no solution. return nullptr.
   	   	if(solved.size() == myCities.size()+1){
   			cout<<"No such path could be made. The itinerary failed to find a way to get to "<<endFlag<<endl;
   			return nullptr;
   		}
   	}
	
   	//If we were doing an activity search, we can now get rid of the holder in end, and set end to be the actual end of the journey whatever it may be.
    if(activitySearch){
    	delete end;
    	end = solved.back();
    }
    
    //Now begin to transfer the new leg of the journey into the larger roadtrip path. This leg will be added using the new subSolved path vector

    //Get the end of the leg, set its value to true, add it to the subsolved path, and increase the overall distance variable by the subdistance
    City * runner = solved[solved.size()-1];
    runner->inPath = true;
    llist * subSolvedPath = new llist();
    subSolvedPath->addToBack(runner->name);
    pathDistance += runner->distance;

    //Loop through the leg's solution. At each step flag the node as now being in the larger toadtrip path, add the node to the subsSolvedPath
    // create a string of the subpath and then move to the next node.
    string ret= runner->name;
    while(runner->previous){
    	subSolvedPath->addToBack(runner->previous->name);
    	runner->previous->inPath = true;

        ret = runner->previous->name + " - " + ret;
        runner = runner->previous;
    }

    //Using the subSolvedPath list, add this new leg of the journey to the overarching solvedPath. Use the list functions backPop and addToBack to 
    //Ensure the correct ordering of the items in the list and use the contains function to void adding dubplicates into the list
    while( 0<subSolvedPath->size()){
    	string temp = subSolvedPath->backPop();
    	if(!solvedPath->contains(temp))
    		solvedPath->addToBack(temp);
    }

	//Print out the leg's path as well as the leg's distance      
	cout<<endl<<"Shortest Path"<<endl;
    cout<<ret<<endl;
    
    cout<<"Minimum Distance: "<<end->distance<<endl<<endl;

    //Return the city we ended at. This allows us to start the next leg of the journey regardless of whether or not the previous leg was an activity.
    return solved[solved.size()-1];  
}

//Public Functions:

//Loop through the cities, if a city's name matches the passed string return it's pointer. Otherwise return a nullptr
City* Graph::search(string name){
	for(int i = 0; i< myCities.size(); i++){
		if (myCities[i]->name == name)
			return myCities[i];
	}

	return nullptr;
}

//Make sure we have both cities, make sure there isn't already a path between the two cities, and then add a path between the two cities
//Note this is an undirected insert function. To make it directed we'd only need to remove the last two lines
void Graph::insertEdge(City* startCity, City* endCity, int weight){
	if(!startCity || !endCity){
		cout<<"The cities couldn't be found, no edge was inserted"<<endl;
		return;
	}


	for(int i = 0; i<startCity->neighbors.size(); i++){
		if(startCity->neighbors[i]->neighbor->name == endCity->name){
			cout<<"There's already a path between these cities"<<endl;
			return;
		}
	}

	Adjacent * sToEnd = new Adjacent(endCity,weight);
	startCity->neighbors.push_back(sToEnd);

	Adjacent * eToStart = new Adjacent(startCity,weight);
	endCity->neighbors.push_back(eToStart);
}

//This function searches for  a city, makes sure it exists, makes sure its edges have all been removed, deletes its empty edge list deletes the city and then 
//removes the city from the list of cities. Note the usual search function cannot work here as the index of the city within the cities list must be known
void Graph::deleteCity(string name){
	
	//Declare an index variable, and toDelete pointer
	int i;
	City* toDelete = nullptr;

	//Loop through the list of cities. If i find the city, store it and break the loop so i remains the correct index
	for(i = 0; i< myCities.size(); i++){
		if (myCities[i]->name == name){
			toDelete = myCities[i];
			break;
		}
	} 

	//If I didn't find the city I can't delete it. Say as much
	if(!toDelete){
		cout<<"The city \""<<name<<"\" could not be found"<<endl;
		return;
	}

	//Due to the way the functions are called, this loop never be entered, but I included it just in case.
	while(!(toDelete->neighbors.empty())){
		deleteEdge(toDelete,toDelete->neighbors[0]->neighbor);
	}

	//Delete the memory allocated for the activies list and for the city itself
	delete toDelete->activities;
	delete toDelete;

	//There is now a dead pointer within the cities list. Use the previously calculated index to erase the item from the vector
	myCities.erase(myCities.begin() + i) ;

}

//This function deletes an edge between two cities. It will flag errors if the cities weren't found or if the paths didn't exist.
//If all of the edges connecting a city are removed it will remove said city from the cities list
void Graph::deleteEdge(City* startCity, City* endCity){
	//If either pointer is null we can't work so exit the functio
	if(!startCity || !endCity){
		cout<<"There was an error finding the cities."<<endl;
		return;
	}

	//Create a flag boolean.  Loop through the startCitie's connections looking for a connection to the end.
	//If we find such a connection, delete it and change the flag to true
	bool removed = false;
	for(int i = 0; i< startCity->neighbors.size() && removed == false; i++){
		if(startCity->neighbors[i]->neighbor == endCity){
			Adjacent * temp = startCity->neighbors[i];
			startCity->neighbors.erase(startCity->neighbors.begin()+i);
			delete temp;
			removed = true;
		}
	}

	//If the flag variable was not changed, a connection between the two cities didn't exist, say as much and exit the function
	if(!removed){
		cout<<"Error, no such path existed"<<endl;
		return;
	}

	//Reset the flag variable and repeat the last two steps looking at the end city
	removed = false;
	for(int i = 0; i< endCity->neighbors.size() && removed == false; i++){
		if(endCity->neighbors[i]->neighbor == startCity){
			Adjacent * temp = endCity->neighbors[i];
			endCity->neighbors.erase(endCity->neighbors.begin()+i);
			delete temp;
			removed = true;
		}
	}
	if(!removed){
		cout<<"Error, no such path existed"<<endl;
		return;
	}

	//If either city now has no neighbors, it has no use being in the list, so delete the city.
	if(startCity->neighbors.empty()){
		cout<<startCity->name<<" is no longer connected to the map, so it is being deleted."<<endl;
		deleteCity(startCity->name);
	}
	if(endCity->neighbors.empty()){
		cout<<endCity->name<<" is no longer connected to the map, so it is being deleted."<<endl;
		deleteCity(endCity->name);
	}
}

//This is the graph initializing function. If passed the fileName  it opens the file, and uses it to create a graph of cities
//and for each city calls functions to populate the city's activities and connections.
bool Graph::buildGraph(string fileName){
	//Check if the file is openable. If not say as much and exit
	ifstream file(fileName);
	if(!file.is_open()){
		cout<<"The graph making file didn't open properly."<<endl;
		cout<<"Are you sure \""<<fileName<<"\" is in the working directory?"<<endl;
		return false;
	}
	
	//Make sure the file is formatted correctly. The first line should be "Cities:"
	//If we don't have the right file format, say as much and exit
	string tempWord = "";
	getline(file,tempWord);
	if(tempWord != "Cities:"){
		cout<<"The file is improperly formatted. The graph could not be made."<<endl;
		return false;
	}

	//If the file is formatted correctly, line until the word "Connections:" should be a city and it's activities
	//While we can get these city lines, do so
	while(getline(file,tempWord) && tempWord != "Connections:"){

		//turn the line into a stringstream to increase ease of parsing.
		stringstream line;
		line.str(tempWord);

		//The first word in the line should be the city name. Create a city object using the name
		line>>tempWord;
		City* newCity = new City(tempWord);

		//The next word should be list of activities. Take the list and use it to populate the cities
		//However, if the city has no activities tempWord will still be the city name. So we must check
		//That we actually have activities before we attempt to populate
		line>>tempWord;
		if(tempWord != newCity->name)
			populateCityActivities(newCity,tempWord);

		//Call the insertCity function to add the newly made city to the myCities vector
		insertCity(newCity);
    }

    //If the file ended before I made any connections, the file wasn't formatted correctly, say as much and exit the funtion
    if(tempWord != "Connections:"){
		cout<<"The file is improperly formatted. The graph could not be made."<<endl;
		return false;
	}

	//Every remaining line ought to contain contain between two cities. Make those connections
	while(getline(file,tempWord)){
		//Make a stream again
		stringstream line;
		line.str(tempWord);

		//Initialize the holder variables
		string city1;
		string city2;
		int weight;

		//Populate the variables
		line>>city1;
		line>>city2;
		line>>weight;

		//Search for the two cities, and immediately call the insertEdge function on the results and the passed weight.
		insertEdge(search(city1),search(city2),weight);
	}
    //Close the file
    file.close();
}

//This function goes through a city's neighbors and prints each of the city's neighbors and the distance to the neighbor.
void Graph::printNeighbors(City * theCity){
	for(int i = 0; i < theCity->neighbors.size(); i++){
		cout<<theCity->neighbors[i]->neighbor->name<<" is "<< theCity->neighbors[i]->weight<<" miles away"<<endl;
	}
}

//Printing the graph involves printing each city, each of the cities activities and each of the cities neighbors
void Graph::printGraph(){
	for(int i =0; i< myCities.size(); i++){
		cout<<"NAME: "<<endl<<myCities[i]->name<<endl;

		cout<<endl<<"ACTIVITIES: "<<endl;
		myCities[i]->activities->printList();

		cout<<endl<<"CONNECTIONS"<<endl;
		printNeighbors(myCities[i]);

		cout<<endl<<endl;
	}
}

//***ALL OF THE ITINERARY FUNCTIONS***

//When we try to populate the itinerary from a file, this function will add all of the items in the file to the itinerary
bool Graph::buildItinerary(string fileName){
	//Try to open the file, if it doesn't open say as much
	ifstream file(fileName);
	if(!file.is_open()){
		cout<<"The itinerary making file didn't open properly."<<endl;
		cout<<"Are you sure \""<<fileName<<"\" is in the working directory?"<<endl;
		return false;
	}

	//The first item in an itinerary must be a city, otherwise paths will not know where to begin.
	//For that reason we will immediately run a search for the first item and terminate the process
	//if the first item is not a city.
	string tempWord;
	getline(file,tempWord);
	if(!search(tempWord)){
		cout<<"The file is improperly formatted. Itineraries must start with a city."<<endl;
		return false;
	}

	//If the first item is a city, we are then free to add it and the remaining items to the itinerary list
	addItinerary(tempWord);	
	while(getline(file,tempWord)){
		//cout<<tempWord<<endl;
		addItinerary(tempWord);
	}

	//Once we have made our itinerary, print it out so the user can have confirmation of the itinerary they just loaded.
	printItinerary();

}

//This function calls the addToBack function for llists so we can add a string to the itinerary
void Graph::addItinerary(string toAdd){
	myItinerary->addToBack(toAdd);
}

//This function uses the llist deleteItem function to remove an item from the itinerary
 void Graph::removeItinerary(string toDelete){
 	myItinerary->deleteItem(toDelete);
 }

//This function uses the llist boolean function to return whether or not the itinerary contains the desired activity
 bool Graph::contains(City* theCity, string activity){
 	return theCity->activities->contains(activity);
 }

//This function is called to run an itinerary. It goes step by step through the itinerary using Dijkstra's method to find the shortest path between each consective item in the itinerary
void Graph::pathLinker(){

	//If there are not two items in the itinerary, we cannot create a path. Say as much and exit the function
	if(myItinerary->size()<2){
		cout<<"The Itinerary is too short to make a path"<<endl;
		return;
	}

	//This function might be called multiple times on the same graph, to resolve issues between runs we must reset all of the nodes in the graph, delete the previously solved path, and 
	//delete the distance of the previously solved path
	for(int i = 0; i<myCities.size(); i++){
		myCities[i]->inPath = false;

		delete solvedPath;
		solvedPath = new llist();
		pathDistance = 0;
	}

	//Pull the first item from the itinerary.
	string start = myItinerary->pop();

	//The first item must be a city, if it is not a city delete the itinerary and exit the function
	if(!search(start)){
		cout<<"Itineraries need to begin with a city, this is invalid"<<endl;
		delete myItinerary;
		myItinerary = new llist();
		return;
	}

	//If the first item is a city and there is at least one other item, the itinerary is valid regardless of what follows This is true as even if a following item isn't formatted properly, 
	//my Dijkstra's algorithm will simply say that no such path exists and terminates.  Now we simply pull items from the itinerary in order, and call the Dijkstra's function on the pair
	// of starting city, and ending string. Dijkstra's function will always return the city* it ended on. If it ended on null there was an issue and we exit the function. If it landed on a 
	//city, pull the city's name and use it to begin the next leg of the journey.  
	while(myItinerary->size()>0){
		string end = myItinerary->pop();
		City* tempCity = Dijkstra(start,end);
		if(!tempCity) return;
		start = tempCity->name;
	}

	//After going through the itinerary, Dijkstra's function ought to have added the solved path in the correct order to the class variable solvedPath. Print that path and it's distance
	cout<<"The total path needed to accomplish the itinerary is: "<<endl;
	solvedPath->printList();
	cout<<"The total distance traveled is: "<<pathDistance<<endl<<endl;
	
}

//Function that links the itinerary to the llist print function
void Graph::printItinerary(){
	if(myItinerary->size() == 0){
		cout<<"The Itinerary is empty"<<endl;
		return;
	}
	cout<<"\nThe Itinerary: "<<endl;
	myItinerary->printList();
	cout<<endl;
}

//Graph constructor needs to create lists for the itinerary and the path, also set the pathdistance to zero
Graph::Graph(){
	myItinerary = new llist();
	solvedPath = new llist();
	pathDistance = 0;
}

//Deleting the graph requires every connection in the graph and every city to be deleted. As deleting one city can lead to the deletion of a second city since every city is deleted if it has
//No more edges, I chose to delete the graph by deleting all of the edges rather than calling deleteCity() on each city in the list. This creates a more uniform deletion statement at the end
//of the program. After deleting the graph itself I also need to delete the itinerary and solved path.
Graph::~Graph(){
	
	while(!myCities.empty()){
		deleteEdge(myCities[0],myCities[0]->neighbors[0]->neighbor);
		
	}
	delete myItinerary;
	delete solvedPath;
	myItinerary = nullptr;
	solvedPath = nullptr;
}