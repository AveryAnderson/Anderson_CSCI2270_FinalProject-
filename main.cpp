#include <iostream>
#include "roadTripping.hpp"

using namespace std;

//The file containing all of the graph's initial information including Cities, activities for each city, and connections between cities
#define FILENAME "CitySet.txt"

int main(){
	
	//First we need a graph to work with, declare the object.
	Graph * myMap = new Graph();

	//Attempt to populate that graph using the buildgraph function and the defined filename.  If this construction fails we can't do anything so don't enter the menu.
	if(myMap->buildGraph(FILENAME)){

		//initialize a string runFunction. This will be used to navigate all of the menu options
		string runFunction;

		//While my string does not equal the quit command keep reassigning it.
		while(runFunction != "6"){



			//Every loop we want to reset the string and print the options
			runFunction = "-1";
			cout << "======Main Menu======" << endl;
			cout << "1. Print the map" << endl;
			cout << "2. Modify city connections" << endl;
			cout << "3. Modify a city's activities" << endl;
			cout << "4. Load an Itinerary" << endl;
			cout << "5. See/Run/Modify Itinerary" << endl;
			cout << "6. Quit" << endl;

			//Get new strings until we get one that matches a possible function
			while(runFunction != "1" && runFunction != "2" && runFunction != "3" && runFunction != "4" && runFunction != "5" && runFunction != "6"){
				getline(cin,runFunction);
				if(runFunction != "1" && runFunction != "2" && runFunction != "3" && runFunction != "4" && runFunction != "5" && runFunction != "6"){
				cout<<"That was an invalid input, please try again"<<endl; 
				}
			}

			//FUNCTIONS: Once we have a valid user input we can go into the individual functionalities of the program.

			//Prints the graph and all of the city information
			if(runFunction == "1"){
				myMap->printGraph();	
			}

			//There are two ways to modify a city's connections we can add a connection, or remove a connection. 
			else if(runFunction == "2"){

				//First find out if the user wants to add, remove or go back to the main menu. Keep looping until a valid input is entered
				while(runFunction != "Add" && runFunction != "Remove" && runFunction != "Back"){
					cout << "Would you like to add a connection, remove a connection, of go back?" << endl;
					cout << "Type \"Add\", \"Remove\", or \"Back\""<<endl;

					getline(cin,runFunction);
				}

				//If the user wants to add a connection, explain how, gather the necesary information and call the insert Edge function
				if(runFunction == "Add"){
					cout<<endl<<"In order to add a connection, you need to input the two city names and the number of miles between the cities. They each should go on their own lines."<<endl;
					cout<<"Ex:"<<endl;
					cout<<"Boulder"<<endl;	
					cout<<"Canyon_Village"<<endl;	
					cout<<"584"<<endl;	
					cout<<"Make sure the cities are spelled the same way they are in the graph. Especially in terms of capitalization and underscore use."<<endl<<endl;	
					
					cout<<"City One: ";
					string city1;
					getline(cin,city1);
					
					cout<<"City Two: ";
					string city2;
					getline(cin,city2);	

					cout<<"Distance: ";
					string distance;
					getline(cin,distance);

					//This line is the only seg-faultable line i've found in this code. If the entered distance is not a number the stoi function doesn't know what to do.
					myMap->insertEdge(myMap->search(city1),myMap->search(city2),stoi(distance));			
				}

				//If the user wants to add a connection, explain how, gather the necesary information and call the deleteEdge function
				else if(runFunction == "Remove"){
					cout<<endl<<"In order to remove a connection, you need to input the two city names. They each should go on their own lines."<<endl;
					cout<<"Ex:"<<endl;
					cout<<"Las_Vegas"<<endl;	
					cout<<"Roswell"<<endl;	
					cout<<"Make sure the cities are spelled the same way they are in the graph. Especially in terms of capitalization and underscore use."<<endl<<endl;	
					
					cout<<"City One: ";
					string city1;
					getline(cin,city1);
					
					cout<<"City Two: ";
					string city2;
					getline(cin,city2);	

					myMap->deleteEdge(myMap->search(city1),myMap->search(city2));
				}
			}

			//If we are to modify a city's activities we can again add or remove them.
			else if(runFunction == "3"){

				//Ask the user if the want to add, remove or return to the main menu
				while(runFunction != "Add" && runFunction != "Remove" && runFunction != "Back"){
					cout << "Would you like to add an activity, remove an activity, of go back?" << endl;
					cout << "Type \"Add\", \"Remove\", or \"Back\""<<endl;

					getline(cin,runFunction);
				}

				//If they want to show an example and then prompt them for the necesary information
				if(runFunction == "Add"){
					cout<<endl<<"In order to add an activity, you need to input the city name and the activity you wish to add. They each should go on their own lines."<<endl;
					cout<<"Ex:"<<endl;
					cout<<"Keystone"<<endl;	
					cout<<"hiking"<<endl;	
					cout<<"Make sure the cities and activities are spelled the same way they are in the graph. Especially in terms of capitalization and underscore use."<<endl<<endl;	
					
					cout<<"City: ";
					string city1;
					getline(cin,city1);
					
					cout<<"Activity: ";
					string activity;
					getline(cin,activity);

					//Make sure the passed city name actually corresponds to a city in order to avoid a sec fault. Then call the addToBack function to add the activity to the city
					City* toModify = myMap->search(city1);		
					if(!toModify){
						cout<<"There was an error finding the city"<<endl;
					}	
					else{
						toModify->activities->addToBack(activity);
					}
				}

				//If we are to remove an activity from the city we need the city and the activity. Show examples and prompt the user for the necesary information
				else if(runFunction == "Remove"){
					cout<<endl<<"In order to remove an activity, you need to input the city name and the activity you wish to remove. They each should go on their own lines."<<endl;
					cout<<"Ex:"<<endl;
					cout<<"Boulder"<<endl;	
					cout<<"bowling"<<endl;	
					cout<<"Make sure the cities and activities are spelled the same way they are in the graph. Especially in terms of capitalization and underscore use."<<endl<<endl;	
				
					cout<<"City: ";
					string city1;
					getline(cin,city1);
					
					cout<<"Activity: ";
					string activity;
					getline(cin,activity);

					//Verify that the city name was entered correctly in order to avoid a seg-fault
					City* toModify = myMap->search(city1);		
					if(!toModify){
						cout<<"There was an error finding the city"<<endl;
					}	
					else{
						toModify->activities->deleteItem(activity);
					}
				}
			}

			//Users have an option of loading files to create itineraries.  If they select that option prompt them for a file name and call the buildItinerary function to read the file
			else if(runFunction == "4"){
				cout<<"Your itinerary needs to be saved into a text file. It should be a list of activities and or cities each on their own line. \n"
					   " Note the first entry of any itinerary must be a starting city.                                                             " <<endl;
				cout<<"For an example, see Itinerary1.txt Type the name of the file you want to load below."<<endl;
				cout<<"File name: ";
				
				string itineraryName;
				getline(cin,itineraryName);

				myMap->buildItinerary(itineraryName);
			}

			//If users want to see, modify or run their itinerary they do so in this menu subsection
			else if(runFunction == "5"){

				//The user can See, Run, Modify or go back. Keep them in the loop until they choose an applicable option
				while(runFunction != "See" && runFunction != "Run"  && runFunction != "Modify" && runFunction != "Back"){
					cout << "Would you like to see the itinerary, run the itinerary, modify the itinerary, of go back?" << endl;
					cout << "Type \"See\", \"Run\", \"Modify\", or \"Back\""<<endl;

					getline(cin,runFunction);
				}

				//If they want to see the itinerary call the print itinerary function
				if(runFunction == "See"){
					cout<<"\n \n";
					myMap->printItinerary();
					cout<<"\n \n";
				}

				//If they want to run the itinerary call the pathlinker function which will run the itinerary step by step.
				else if(runFunction == "Run"){
					myMap->pathLinker();
				}

				//Itinerary modifications consist of adding or removing items from the itinerary. Ask the user which they'd like to do, get the proper inputs, and execute the function
				else if(runFunction == "Modify"){
					while(runFunction != "Add" && runFunction != "Remove" && runFunction != "Back"){
						cout << "Would you like to add an activity, remove an activity, of go back?" << endl;
						cout << "Type \"Add\", \"Remove\", or \"Back\""<<endl;

						getline(cin,runFunction);
					}
					if(runFunction == "Add"){
						cout<<endl<<"In order to add to the Itinerary just name what you want to add. Be it a city or an activity."<<endl;
						cout<<"Ex1:"<<endl;
						cout<<"hiking"<<endl<<endl;	
						cout<<"Ex2:"<<endl;
						cout<<"Canyon_Village"<<endl<<endl;
						cout<<"Make sure the cities and activities are spelled the same way they are in the graph. Especially in terms of capitalization and underscore use."<<endl;
						cout<<"Note the activity can't already be in the Itinerary. It will be added to the end of the list"<<endl<<endl;	
					
						cout<<"City or Activity: ";
						string toAdd;
						getline(cin,toAdd);

						myMap->addItinerary(toAdd);
					}
					else if(runFunction == "Remove"){
						cout<<endl<<"In order to remove an activity from the Itinerary, just need to name said item."<<endl;
						cout<<"Ex:"<<endl;
						cout<<"Boulder"<<endl;	
						cout<<"Make sure the cities and activities are spelled the same way they are in the graph. Especially in terms of capitalization and underscore use."<<endl<<endl;	
					
						cout<<"City or Activity: ";
						string toDelete;
						getline(cin,toDelete);

						myMap->removeItinerary(toDelete);
					}	

				}
			
			}
		}
		//If the exit character was entered we'll be dropped here. Print goodbye before ending
		cout << "Goodbye!" << endl<<endl;
	}
	
	//Delete the map and return zero as the function ran successfully	
	delete myMap;
	return 0;
}