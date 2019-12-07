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

#undef max

#include "denizen.h"
#include "district.h"

using namespace std;

class Simville {
private:
	int dayT;
	int hourT;
	std::list<District*>districts;

public:
	Simville() : dayT(0), hourT(0) {}; //Simville has a default day 0 and hour 0

	/*
	* Increments time. Each day is made up of four 6-hour periods (6:00, 12:00, 18:00, 0:00).
	* int hourT represents the current hour and increments the day (dayT) after every 24-hour cycle.
	*/
	void hourTick() {
		if (hourT == 0) {
			hourT = 6;
			++dayT;
			std::cout << endl << std::setw(17) << " " << setfill('=') << std::setw(19) << " DAY ";
			std::cout << dayT << " " << std::setw(15) << " " << setfill(' ') << endl;
			hourTick();
		}
		else if (hourT == 24) {
			//std::cout << std::setw(34) << "DAY " << dayT << ", " << hourT << ":00" << endl;
			hourT = 0;
		}
		else {
			//std::cout << std::setw(34) << "DAY " << dayT << ", " << hourT << ":00" << endl;
			hourT = (hourT + 6);
		}
	}

	string hourShow() {
		string hours;
		if (hourT == 0) {
			//std::cout << std::setw(34) << "DAY " << dayT << ", 24:00" << endl;
			hours = "DAY " + std::to_string(dayT) + ", 24:00";
			return hours;
		}
		else {
			hours = "DAY " + std::to_string(dayT) + ", " + std::to_string(hourT - 6) + ":00";
			return hours;
		}//std::cout << std::setw(34) << "DAY " << dayT << ", " << hourT - 6 << ":00" << endl;
	}

	void addDist(District* d) { districts.push_back(d); }

	/*
	* Prints district name and the time, then calls printPop(verbose) to print district-
	* specific population. 
	*/
	void districtPopulation(bool verbose) {
		HANDLE hConsole;
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		std::list<District*>::iterator it = districts.begin();
		while (it != districts.end()) {
			District* dist = *it;

			if (verbose == false) {
				SetConsoleTextAttribute(hConsole, 2); //Sets time text to green
				cout << hourShow();
				SetConsoleTextAttribute(hConsole, 7); //Sets text back to white
				cout << " " << dist->getName();
			}
			else { 			
				cout << endl << setw(35) << dist->getName() << " District" << endl;
			
				SetConsoleTextAttribute(hConsole, 2); //Sets time text to green
				cout << std::setw(42) << hourShow() << endl;
				SetConsoleTextAttribute(hConsole, 7); //Sets text back to white
				cout << setfill('.') << std::setw(80) << " " << setfill(' ') << endl << endl << endl;
			}
			dist->printPop(verbose);
			//dist->createQueue(5);
			it++;
					
		}
	}

	/*
	* Prints the sum total of all zombies, alarmed and ignorant denizens from all districts.
	*/
	void showTotal() {
		int zombies = 0;
		int alarmed = 0;
		int ignorant = 0;

		std::list<District*>::iterator it = districts.begin();
		while (it != districts.end()) {
			District* dist = *it;
			zombies += dist->zombieTotal();
			alarmed += dist->alarmedTotal();
			ignorant += dist->ignorantTotal();
			it++;
		}

		HANDLE hConsole; //Colored Console Text
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

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

	/**
	* Generates a random population size for each district, then creates and adds that amount of denizens.
	* Uses denizen names from the ones provided in Residents.txt file. 
	**/
	void populateDistrict() {
		
		array<int, 6>popDist;
		
		int randSum;
		double coefficient;
		double leftover;
		int minimum;
		//int total = 2000;
		int total = 50; //For testing, delete later

		random_device ran;

		do {
			randSum = 0;
			coefficient = 0;
			leftover = total;
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

		std::string name;
		std::vector<std::string> namelist;
		namelist.reserve(2000);
		//Refers to the location of Residents.txt relative to the SOLUTION, not the header and source files. May be different on VS Code. 
		std::ifstream in("../Zombieland/Residents.txt"); 

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
		list<District*>::iterator it = districts.begin();
		while (it != districts.end()) {
			District* dist = *it;

			for (int i = 0; i < popDist[popcount]; i++) {
				string newname = namelist[namecount];
				int randDist = 1 + ran() % 6;
				string w;
				string h;
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
				int biteChance = ran() % 100 + 20;
				Denizen* d = new Ignorant(newname, w, h);
				d->setBiteChance(biteChance);
				dist->addDenizen(d);
				namecount++;
			}
			popcount++;
			it++;
		}

	}

	/**
	* Given a district and n amount of zombies to create, takes a random denizen
	* from that district's populace list and checks to see if they are already a
	* zombie. If not, the denizen's name is used to create a new zombie and then deleted.
	**/
	void createZombie(District* dist, int n) {
		list<Denizen*>* newlist = dist->getPopulace();
		random_device rand;
		int listSize = newlist->size(); 
		int randDenizen;

		for (int i = 0; i < n; i++) {
			list<Denizen*>::iterator itr = newlist->begin();
			randDenizen = rand() % listSize; //Make sure we don't advance beyond list size
			advance(itr, randDenizen);

			Denizen* D = *itr;
			list<Denizen*>::iterator newit = itr;

			if (D->getStatus() != "Zombie") { //Not already a zombie, make zombie
				string Dname = D->getName();
				dist->addDenizen(new Zombie(Dname));
				newlist->erase(newit);
			}
			else{ //Is already a zombie
				cout << std::endl << "---------------------- " << D->getName() << " Is already a Zombie!!! -------------------------- " << endl;
				// If a denizen tries to alarm a zombie, they should get bitten.  
			}
		}
	}

	void createAlarmed(District* dist, int n) {
		list<Denizen*>* newlist = dist->getPopulace();
		random_device rand;
		int listSize = newlist->size();
		int randDenizen;

		for (int i = 0; i < n; i++) {
			list<Denizen*>::iterator itr = newlist->begin();
			randDenizen = rand() % listSize; //Basically the same as createZombie
			advance(itr, randDenizen);

			Denizen* D = *itr;
			list<Denizen*>::iterator newit = itr;

			if (D->getStatus() == "Ignorant") {
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

};

#endif