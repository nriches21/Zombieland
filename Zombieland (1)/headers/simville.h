#ifndef SIMVILLE_H_
#define SIMVILLE_H_

#include <iostream>
#include <array>
#include <list>
#include <iomanip>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <windows.h>
#include <map>

#undef max

#include "denizen.h"
#include "district.h"
using namespace std;

class Simville {
private:
	int dayT; //current day
	int hourT; //current hour
	std::list<District*>districts; //list of the districts held within Simville
 
public:
	Simville() : dayT(0), hourT(0) {}; //Simville has a default day 0 and hour 0

	/*
	* Increments time. Each day is made up of four 6-hour periods (6:00, 12:00, 18:00, 0:00).
	* int hourT represents the current hour and increments the day (dayT) after every 24-hour cycle.
	*/
	void hourTick() {
		if (hourT == 0) { //If it was the beginning of the day
			hourT = 6; //change the hour to be 6
			++dayT; //increment day
			std::cout << endl << std::setw(17) << " " << setfill('=') << std::setw(19) << " DAY "; //print dat
			std::cout << dayT << " " << std::setw(15) << " " << setfill(' ') << endl;
			hourTick(); //recursively call the hour tick for 1 day
		}
		else if (hourT == 24) { //if we are at the end of the day
			hourT = 0; //change the hour to be 0
		}
		else { //if we are at 6, 12 or 18 go up to the next time clock
			hourT = (hourT + 6);
		}
	}

	/*
	*Prints the current day and hour
	*/
	string hourShow() {
		string hours;
		if (hourT == 0) {
			hours = "DAY " + std::to_string(dayT) + ", 24:00"; //print Day and hours
			return hours;
		}
		else {
			hours = "DAY " + std::to_string(dayT) + ", " + std::to_string(hourT - 6) + ":00"; //print days and hours
			return hours;
		}
	}

	/*
	*Pushback a district into the list of districts
	*/
	void addDist(District* d) { districts.push_back(d); }

	/*
	* Prints district name and the time, then calls printPop(verbose) to print district-
	* specific population. 
	*/
	void districtPopulation(bool verbose) {
		HANDLE hConsole; //set up the color outputs
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		std::list<District*>::iterator it = districts.begin(); //set an iterator to the beginning of the districts
		while (it != districts.end()) { //iterate through the entire districts list
			District* dist = *it; //set a pointer to the current distric the iterator is pointing to 

			if (verbose == false) { //if not verbose, print out the hour and the district name
				SetConsoleTextAttribute(hConsole, 2); //Sets time text to green
				cout << hourShow();
				SetConsoleTextAttribute(hConsole, 7); //Sets text back to white
				cout << " " << dist->getName();
			}
			else { //if verbose, print the District name and hour in a different way
				cout << endl << setw(35) << dist->getName() << " District" << endl;
				SetConsoleTextAttribute(hConsole, 2); //Sets time text to green
				cout << std::setw(42) << hourShow() << endl;
				SetConsoleTextAttribute(hConsole, 7); //Sets text back to white
				cout << setfill('.') << std::setw(80) << " " << setfill(' ') << endl << endl << endl;
			}
			dist->printPop(verbose); //print the population
			dist->alarm(); //have alarmed people try to convert ignorant to alarm
			dist->bite(); //zombies try to bite and convert denizens into zombies
			move(dist, verbose); //people moving
			resetTurn(dist); //reset the turnOver bool for everyone
			
			it++; //increment the iterator to the new district
		}
	}

	/**
	* Generates a random population size for each district, then creates and adds that amount of denizens.
	* Uses denizen names from the ones provided in Residents.txt file. 
	**/
	void populateDistrict() {
		
		array<int, 6>popDist; //array of itegers with size 6
		
		int randSum; //integer to hold a random value
		double coefficient; //variable to hold values while calculating
		double leftover; //variable to hold the leftover amount of denizens that need to be assigned
		int minimum; //minumum num of denizens in each district 
		int total = 2000; //total number of denizens throughout simville

		random_device ran; //set up random

		do {
			//set everything to be 0 for each district
			randSum = 0;
			coefficient = 0;
			leftover = total; //will be altered throughout, still the number of denizens yet to be assigned a district
			minimum = 0;

			//Assigns random value to all elements of popDist[]
			for (int g = 0; g < popDist.size(); g++) {
				popDist[g] = 1+ran() % 100;
				randSum += popDist[g];
			};

			//Finds coefficient of total and randSum
			coefficient = leftover / randSum;

			//Multiplies random popDist[] values by coefficient, updates leftover
			for (int h = 0; h < popDist.size(); h++) {
				popDist[h] *= coefficient;
				leftover -= popDist[h];

				//Finds smallest value of popDist[]
				if (popDist[h] < popDist[minimum]) {
					minimum = h;
				}
			};

			//If there is any leftover, add it to the minimum value
			popDist[minimum] += leftover;

		} while (popDist[minimum] == 0); //Don't want any district's initial population to be zero.

		std::string name; //current name looking at
		std::vector<std::string> namelist; //vecotr to hold all of the names
		namelist.reserve(2000); //make the vector 2000 size
		//Refers to the location of Residents.txt relative to the SOLUTION, not the header and source files. May be different on VS Code. 
		std::ifstream in("../Zombieland/Residents.txt"); 

		//grab names fro the txt file and push them into the nameList vector
		if (in.is_open()) {
			while (getline(in, name)) {
				if (name.size() > 0)
					namelist.push_back(name);
			}in.close();
		}
		else std::cout << "Unable to open file :(" << std::endl;

		//Generates and adds the number of denizens set by popDist[] for each district
		int popcount = 0; 
		int namecount = 0;
		list<District*>::iterator it = districts.begin(); //iterator to the beginning of the district list
		while (it != districts.end()) { //iterate through the entire district list 
			District* dist = *it; //pointer to the district based off the iterator 

			for (int i = 0; i < popDist[popcount]; i++) { //iterate for the entire population
				string newname = namelist[namecount]; //grab a name from the vector
				int randDist = 1 + ran() % 6; //randomize the district 
				string w; //empty string tha twill hold the work district name
				string h; //empty string that will hold the home district name 
				//switch statement to determine what work and home will be for the new ignorant
				switch (randDist) { 
				case 1: { 
					w = "Uptown";
					h = "Downtown";
					break; }
				case 2: {
					w = "Downtown";
					h = "The University";
					break; }
				case 3: {
					w = "The University";
					h = "Soho";
					break; }
				case 4: {
					w = "Soho";
					h = "Medical Hill";
					break; }
				case 5: {
					w = "Medical Hill";
					h = "The Docks";
					break; }
				case 6: {
					w = "The Docks";
					h = "Uptown";
					break; }
				}
				int biteChance = ran() % 80 + 20; //set a random bite chance 
				Denizen* d = new Ignorant(newname, w, h); //create a new ignorant person 
				d->setBiteChance(biteChance + dist->getDensity()); //set their bite chance to be what was randomly created
				dist->addDenizen(d); //add the new ignorant denizen to the current district
				namecount++; //go to the next name in the vector
			}
			popcount++; //tell the thing that their is one more person in the population
			it++; //go to the next district
		}

	}

	/**
	* Given a district and n amount of zombies to create, takes a random denizen
	* from that district's populace list and checks to see if they are already a
	* zombie. If not, the denizen's name is used to create a new zombie and then deleted.
	**/
	void createZombie(District* dist, int n) {
		list<Denizen*>* newlist = dist->getPopulace(); //pointer to the poulace list in the input district
		random_device rand; //setup rand
		int listSize = newlist->size(); //integer to hold the size of the populace
		int randDenizen; //int to hold the random denizen

		for (int i = 0; i < n; i++) { //create a new zombies however many times the parameter determines
			list<Denizen*>::iterator itr = newlist->begin(); //iterator to the populace list of denizens in the district
			randDenizen = rand() % listSize; //Make sure we don't advance beyond list size
			advance(itr, randDenizen);

			Denizen* D = *itr; //pointer to where the iterator is at
			list<Denizen*>::iterator newit = itr; //new iterator (solution because something was breaking)

			if (D->getStatus() != "Zombie") { //Not already a zombie, make zombie
				string Dname = D->getName();
				dist->addDenizen(new Zombie(Dname)); //add new
				newlist->erase(newit); //erase old
			}
			else{ //Is already a zombie
				cout << std::endl << "---------------------- " << D->getName() << " Is already a Zombie!!! -------------------------- " << endl;
				// If a denizen tries to alarm a zombie, they should get bitten.  
			}
		}
	}

	/**
	* Given a district and n amount of alarmed to create, takes a random denizen
	* from that district's populace list and checks to see if they are already an
	* alarmed. If not, the denizen's name is used to create a new zombie and then deleted.
	**/
	void createAlarmed(District* dist, int n) {
		list<Denizen*>* newlist = dist->getPopulace(); //pointer to the input district populace
		random_device rand; //setup rand
		int listSize = newlist->size(); //int to hold size of populace 
		int randDenizen; //int of position of the random denizen to turn into alarmed

		for (int i = 0; i < n; i++) { //make new alarmed for the amoutn of times the parameter said
			list<Denizen*>::iterator itr = newlist->begin(); //iterator to the beginning of populace
			randDenizen = rand() % listSize; //Basically the same as createZombie
			advance(itr, randDenizen);

			Denizen* D = *itr; //pointer to where the iterator is at
			list<Denizen*>::iterator newit = itr; //new iterator (again something was breaking without this_)

			if (D->getStatus() == "Ignorant") { //ignorant? make alarmed
				string Dname = D->getName();
				int chance = D->getBiteChance() - 20;
				dist->addDenizen(new Alarmed(Dname, chance));
				newlist->erase(newit);
			}
			else { //Is either Alarmed or a Zombie
				cout << std::endl << "---------------------- " << D->getName() << " Can't be alarmed!!! -------------------------- " << std::endl;
				// If a denizen tries to alarm a zombie, they should get bitten.  
			}
		}
	}

	/**
	* Given a district, iterator through the populace and move the denizens around
	* Ignorant move based on time
	* Alarmed and Zombie move based the connections to the other districts
	**/
	void move(District* dist, bool verbose) { 
		list<Denizen*>* pop = dist->getPopulace(); //pointer to the poulace
		std::list<Denizen*>::iterator mov = pop->begin(); //iterator to the beginning of the populace
		
			while (mov != pop->end()){ //go through the entire populace
			Denizen* current = *mov; //pointer to a Denizen where the iterator is current at
			if(current->getTurnOver() == false){ //as long as they haven't already had a turn
				if (current->getStatus() == "Ignorant") { //if they are ignorant
					
					string h = dynamic_cast<Ignorant*>(current)->getHome(); //grab home variable
					string w = dynamic_cast<Ignorant*>(current)->getWork(); //grab work variable
					if (hourT - 6 == 6) { //if it's 6 in the morning, go to work
						
						std::list<District*>::iterator iterd = districts.begin(); //iterator to the beginning of the district 
						while (iterd != districts.end()) { //iterate through the districts until we figure out which district the string w correlates to
							District* d = *iterd; //create a pointer to a district where the iterator is at
							string itname = d->getName(); //grabt he name of the current district 
							if (w == itname) { //if we are at the correct district
								Denizen* newd = new Ignorant(current->getName(), h, w); //create new ignorant (same person)
								newd->setBiteChance(current->getBiteChance() + d->getDensity()); //set bite chance of new person (based on old things)
								// if (verbose == true){
								// 	newd->speak(); //if the user wants the verbose description, have the new denizen speak
								// }
								newd->setTurnOver(true);
								d->addDenizen(newd); //add the denizen to the correct district
								mov = pop->erase(mov); //erase the old person
							}
							iterd++; //iterate through the districts
						}							
					}
					//same thing as above but looking for home at 6 pm (18:00)
					else if (hourT - 6 == 18) {
						std::list<District*>::iterator iterd = districts.begin();
						while (iterd != districts.end()) {
							District* d = *iterd;
							string itname = d->getName();
							if (h == itname) {
								Denizen* newd = new Ignorant(current->getName(), h, w);
								cout << " " << newd->getName() << " is going home." << endl;
								newd->setBiteChance(current->getBiteChance() + d->getDensity());
								// if (verbose == true){
								// 	newd->speak(); //if the user wants the verbose description, have the new denizen speak
								// }
								newd->setTurnOver(true);
								d->addDenizen(newd);
								mov = pop->erase(mov);
							}
							iterd++;
						}
					}
				}
				else { //zombie or alarmed
					map<char, District*>* connect = dist->getConnections(); //see the districts you are connected to
					map<char, District*> connect2 = *dist->getConnections(); //see the districts you are connected to
					District* moveTo = NULL; //pointer that will hold the district the zombie or alarmed will move to 
					random_device rand; //setup rand
					int randDirection = rand() % 4; //rand direction that will correlate to a direction

					//set the moveTo pointer to be based on the random direction 
					switch (randDirection) {
					case 0:
						moveTo = connect2['n']; //move north is random is 0
						break;
					case 1:
						moveTo = connect2['e']; //move east if random is 1
						break;
					case 2:
						moveTo = connect2['s']; //move south if random is 2
						break;
					case 3:
						moveTo = connect2['w']; //move west if random is 3
						break;
					}

					int biteChance = current->getBiteChance() + moveTo->getDensity(); //make a bite chance
					if (current->getStatus() == "Alarmed") { //if they are alarmed, create a new alarmed to push to the moveto district
						Denizen* newd = new Alarmed(current->getName(), biteChance);
						cout << " " << newd->getName() << " is screaming and running to " << moveTo->getName() << endl;
						newd->setTurnOver(true);
						// if (verbose == true){
						// 	newd->speak(); //if the user wants the verbose description, have the new denizen speak
						// }
						moveTo->addDenizen(current);
					}
					else { //if they are a zombie, create a new zombie to push to the moveTo district 
						Denizen* newd = new Zombie(current->getName());
						cout << " " << newd->getName() << " is a zombie on the move to " << moveTo->getName() << endl;
						newd->setTurnOver(true);
						// if (verbose == true){
						// 	newd->speak(); //if the user wants the verbose description, have the new denizen speak
						// }
						moveTo->addDenizen(current);
					}
					mov = pop->erase(mov); //erase them from the current district
				}
			}
			mov++; //increment through the populace
		} 
	} 

	/*
	* resetting the turnOver bool after after time click 
	* everyone has one turn per time click to either bite, move, or alarm
	*/
	void resetTurn(District* dist) {
		list<Denizen*>* pop = dist->getPopulace(); //pointer to the populace
		std::list<Denizen*>::iterator den = pop->begin(); //iterator to the populace
		while (den != pop->end()) { //iterate through teh entire populace
			Denizen* d = *den;
			if (d->getTurnOver() == true) { //reset the turnover bool for everyone
				d->setTurnOver(false);
			}			
			den++;
		}		
		cout << endl;
	}

	/*
	* adding up the total zombies, alarmed and ignorant for each district
	*/
	void simvilleSum() {
		//set numbers to be 0 
		int zombies = 0;
		int alarmed = 0;
		int ignorant = 0;
		std::list<District*>::iterator it = districts.begin(); //iterator to the beginning of the district class
		while (it != districts.end()) { //iterate through the whole district list
			District* dist = *it; //pointer to the district based on the iterator 
			dist->countPop(); //call count pop for the current district 
			// call the count for each type of denizen
			zombies += dist->zombieTotal();
			alarmed += dist->alarmedTotal();
			ignorant += dist->ignorantTotal();
			it++; //increment to the next district
		}
		HANDLE hConsole; //Colored Console Text
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		//print out each total just created
		cout << endl << std::setw(12) << "ALL ZOMBIES: ";
		SetConsoleTextAttribute(hConsole, 4);
		cout << zombies;
		SetConsoleTextAttribute(hConsole, 7);
		cout << std::setw(19) << "ALL ALARMED: ";
		SetConsoleTextAttribute(hConsole, 6);
		cout << alarmed;
		SetConsoleTextAttribute(hConsole, 7);
		cout << std::setw(18) << "ALL IGNORANT: ";
		SetConsoleTextAttribute(hConsole, 15);
		cout << ignorant;
		SetConsoleTextAttribute(hConsole, 7);
		cout << std::setw(16) << "ALL TOTAL: " << zombies + alarmed + ignorant << endl << endl;
	}

};

#endif