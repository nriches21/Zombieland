#ifndef DISTRICT_H_
#define DISTRICT_H_

#include <iostream>
#include <iomanip>
#include <list>
#include "denizen.h"
#include <map>
#include <windows.h>

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

		HANDLE hConsole;
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		list<Denizen*>::iterator it = populace.begin();
		while (it != populace.end()) {

			Denizen* pointr = *it;
			if (verbose != false) {
				if (column == 6) {
					column = 0;
					cout << endl;
				}
				column++;

				if (pointr->getStatus() == "Zombie") {
					SetConsoleTextAttribute(hConsole, 4); //Red Text
					cout << std::setw(13) << pointr->getName();
					SetConsoleTextAttribute(hConsole, 7); //Back to white Text
				}
				else if (pointr->getStatus() == "Alarmed") {
					SetConsoleTextAttribute(hConsole, 6); //Yellow Text
					cout << std::setw(13) << pointr->getName();
					SetConsoleTextAttribute(hConsole, 7); //Back to white Text
				}
				else {
					cout << std::setw(13) << pointr->getName();
				}
			}

			if (pointr->getStatus() == "Zombie") ++zombies;
			else if (pointr->getStatus() == "Alarmed") ++alarmed;
			else ++ignorant;
			++it;
		}

		if (verbose != false) {
			cout << endl << endl << std::setw(12) << "ZOMBIES: ";
			SetConsoleTextAttribute(hConsole, 4);
			cout << zombies;
			SetConsoleTextAttribute(hConsole, 7);
			cout << std::setw(19) << "ALARMED: ";
			SetConsoleTextAttribute(hConsole, 6);
			cout << alarmed;
			SetConsoleTextAttribute(hConsole, 7);
			cout << std::setw(18) << "IGNORANT: ";
			SetConsoleTextAttribute(hConsole, 15);
			cout << ignorant;
			SetConsoleTextAttribute(hConsole, 7); 
			cout << std::setw(16) << "TOTAL: " << zombies + alarmed + ignorant << endl;
			cout << setfill('=') << std::setw(80) << " " << setfill(' ') << endl << endl;
		}
		else {
			cout << std::setw(15) << std::right << "ZOMBIES: ";
			SetConsoleTextAttribute(hConsole, 4);
			cout << zombies;
			SetConsoleTextAttribute(hConsole, 7);
			cout << std::setw(12) << std::right << "ALARMED: ";
			SetConsoleTextAttribute(hConsole, 6);
			cout << alarmed;
			SetConsoleTextAttribute(hConsole, 7);
			cout << std::setw(12) << std::right << "IGNORANT: ";
			SetConsoleTextAttribute(hConsole, 15);
			cout << ignorant;
			SetConsoleTextAttribute(hConsole, 7);
			cout << std::setw(10) << std::right << "TOTAL: " << zombies + alarmed + ignorant << endl;
		}
	}

};

#endif