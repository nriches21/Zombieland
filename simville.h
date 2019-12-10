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
			hourT = 0;
		}
		else {
			hourT = (hourT + 6);
		}
	}

	string hourShow() {
		string hours;
		if (hourT == 0) {
			hours = "DAY " + std::to_string(dayT) + ", 24:00";
			return hours;
		}
		else {
			hours = "DAY " + std::to_string(dayT) + ", " + std::to_string(hourT - 6) + ":00";
			return hours;
		}
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
			//dist->alarm();
			//dist->bite();
			move(dist);

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
				int biteChance = ran() % 80 + 20;
				Denizen* d = new Ignorant(newname, w, h);
				d->setBiteChance(biteChance + dist->getDensity());
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

	void move(District* dist) {
		list<Denizen*>* pop = dist->getPopulace();
		std::list<Denizen*>::iterator mov = pop->begin();

		while (mov != pop->end()){
			Denizen* current = *mov;
			if(current->getTurnOver() == false){
				if (current->getStatus() == "Ignorant") {
					string h = dynamic_cast<Ignorant*>(current)->getHome();
					string w = dynamic_cast<Ignorant*>(current)->getWork();
					if (hourT - 6 == 6) {						
						std::list<District*>::iterator iterd = districts.begin();
						while (iterd != districts.end()) {
							District* d = *iterd;
							string itname = d->getName();
							if (w == itname) {
								Denizen* newd = new Ignorant(current->getName(), h, w);
								newd->setBiteChance(current->getBiteChance() + d->getDensity());
								newd->setTurnOver(true);
								d->addDenizen(newd);
								mov = pop->erase(mov);
							}
							iterd++;
						}							
					}
					else if (hourT - 6 == 18) {
						std::list<District*>::iterator iterd = districts.begin();
						while (iterd != districts.end()) {
							District* d = *iterd;
							string itname = d->getName();
							if (h == itname) {
								Denizen* newd = new Ignorant(current->getName(), h, w);
								newd->setBiteChance(current->getBiteChance() + d->getDensity());
								newd->setTurnOver(true);
								d->addDenizen(newd);
								mov = pop->erase(mov);
							}
							iterd++;
						}
					}
				}
				else {
					map<char, District*>* connect = dist->getConnections(); //see the districts you are connected to
					map<char, District*> connect2 = *dist->getConnections(); //see the districts you are connected to
					District* moveTo = NULL;
					random_device rand;
					int randDirection = rand() % 4;
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
					int biteChance = current->getBiteChance() + moveTo->getDensity();
					if (current->getStatus() == "Alarmed") {
						Denizen* newd = new Alarmed(current->getName(), biteChance);
						newd->setTurnOver(true);
						moveTo->addDenizen(current);
						mov = pop->erase(mov);
					}
					else {
						Denizen* newd = new Zombie(current->getName());
						newd->setTurnOver(true);
						moveTo->addDenizen(current);
						mov = pop->erase(mov);
					}
					//moveTo->addDenizen(current);
					//mov = pop->erase(mov);
				}
			}
			mov++;
		} 
	} 

	void resetTurn(District* dist) {
		list<Denizen*>* pop = dist->getPopulace();
		std::list<Denizen*>::iterator del = pop->begin();

		Denizen* d;
		d->setTurnOver(false);
	}

	/** //move()
	* Iterate through the entire list of denizens, check if denizen already made an action (turnOver bool)
	* If not, then check if ignorant; if ignorant, check time tick, and if they move this tick, then move to home or work.
	* Simville has a map of denizens and districts. move() takes a pointer to Simville's map and modifies denizen locations.
	* map<denizen*, district name string>some_map
	* After move, delete denizen from this list and push new pair to simville's map.
	* Zombie and Alarmed move- move into any connected district at random. Delete from list and push new pair to map.
	* District has to update list based on Simville's map each time tick.
	**/
	/*void move(District* inputDistrict) {
		list<Denizen*>* pop = inputDistrict->getPopulace();
		std::list<Denizen*>::iterator mov = pop->begin(); //iterator to the beginning of the populace list
		cout << "mov" << endl;
		mov++;
		Denizen* current;

		while (mov != pop->end()) {
			cout << "mov has either just been set or iterated." << endl;
			current = *mov;
			//mov++;

			if (current->getTurnOver() == false) {

				if (current->getStatus() == "Ignorant") {
					string h = dynamic_cast<Ignorant*>(current)->getHome();
					string w = dynamic_cast<Ignorant*>(current)->getWork();

					if (hourT - 6 == 6) { //if it's 6 in the morning, go to work
						cout << hourShow() << endl;
						cout << current->getName() << " is going to work!" << endl;
						std::list<District*>::iterator dist = districts.begin();
						District* d;

						while (dist != districts.end() && current->getTurnOver()!=true) {
							d = *dist;
							if (d->getName() == w) {
								cout << current->getName() << " is about to move." << endl;
								current->setTurnOver(true);
								d->addDenizen(current);//push the same person to the district where they work
								inputDistrict->removeDenizen(current); //delete them from the current list (in the district they are currently in)
							}
							cout << "dist++" << endl;
							dist++;
						}
					}
					else if (hourT - 6 == 18) { //if it's 6 at night, go home
						std::list<District*>::iterator dist = districts.begin();
						District* d = *dist;
						while (dist != districts.end()) {
							if (d->getName() == h) {
								d->addDenizen(current);//push the same person to the district where they live
								inputDistrict->removeDenizen(current); //delete them from the current list (in the district they are currently in)
								cout << current->getName() << " moved." << endl;
							}
							dist++;
						}
					}
				}
				else {
					map<char, District*>* connect = inputDistrict->getConnections(); //see the districts you are connected to 
					map<char, District*> connect2 = *inputDistrict->getConnections(); //see the districts you are connected to 
					District* moveTo = NULL;
					random_device rand;
					int randDirection = rand() % 4;
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
					}
					std::list<District*>::iterator dist = districts.begin();
					District* distr = *dist;

					while (distr->getName() != moveTo->getName()) {
						distr++;
					}
					
					distr->addDenizen(current);
					inputDistrict->removeDenizen(current); //delete them from the current list (the district they are right now)
				}
			}
			cout << "Iterator mov++" << endl;
			mov++;
		}
	}*/

};

#endif