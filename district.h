#ifndef DISTRICT_H_
#define DISTRICT_H_

#include <iostream>
#include <iomanip>
#include <list>
#include "denizen.h"
#include <map>

using std::cout;
using std::endl;
using std::setw;
using std::setfill;
using std::list;
using std::map;


class District {
private:
	string name;
	list<Denizen*>populace;
	map<char, District*>connections; //north/south/east/west char and connected district (ex., N, Downtown means go north to reach Downtown)
	int zombies;
	int alarmed;
	int ignorant;

public:
	District(string name) : name(name), zombies(0), alarmed(0), ignorant(0) {}

	string getName() const{ return name; }

	int zombieTotal() { return zombies; }
	int alarmedTotal() { return alarmed; }
	int ignorantTotal() { return ignorant; }

	list<Denizen*>* getPopulace() { return &populace; }

	void addDenizen(Denizen* de) { populace.push_back(de);	}

	//bite()	
	/**
	* Each district checks to see if there are zombies, if yes- 
	* then creates a queue of citazens based on bite chance that is the 
	* size of the # of zombies which pushes denizens in based on bite chance, 
	* and a for loop for # of zombies which determines whether bite was successful- 
	*	If success, 
	* Another person is pushed into the queue if that person in the queue moves to another district
	* Map or list of denizens sorted by bite chance 
	**/

	//move()	
	/**
	* Iterate through the entire list of denizens, check if denizen already made an action (turnOver bool)
	* If not, then check if ignorant; if ignorant, check time tick, and if they move this tick, then move to home or work.
	* Simville has a map of denizens and districts. move() takes a pointer to Simville's map and modifies denizen locations.
	* map<denizen*, district name string>some_map 
	* After move, delete denizen from this list and push new pair to simville's map.
	* Zombie and Alarmed move- move into any connected district at random. Delete from list and push new pair to map. 
	* District has to update list based on Simville's map each time tick.	
	**/

	/*
	* Prints population of zombies, alarmed and ignorant denizens in this district.
	* Also prints the name of each denizen.
	*/
	void printPop(bool verbose) {
		zombies = 0;
		alarmed = 0;
		ignorant = 0;
		int column = 0;

		if (verbose != false) {
			cout << this->getName() << " District" << endl;
		}
		list<Denizen*>::iterator it = populace.begin();
		while (it != populace.end()) {

			Denizen* pointr = *it;/*
			if (verbose != false) {
				if (column == 4) {
					column = 0;
					cout << endl;
				}
				column++;

				//cout << std::setw(20) << pointr->getName();
			}*/

			if (pointr->getStatus() == "Zombie") ++zombies;
			else if (pointr->getStatus() == "Alarmed") ++alarmed;
			else ++ignorant;
			++it;
		}

		if (verbose != false) {
			cout << endl << std::setw(18) << "ZOMBIES: " << zombies << std::setw(19) << "ALARMED: " << alarmed << std::setw(18);
			cout << "IGNORANT: " << ignorant << std::setw(18) << "TOTAL: " << zombies + alarmed + ignorant << endl;
			cout << setfill('-') << std::setw(80) << " " << setfill(' ') << endl << endl;
		}
	}

};

#endif