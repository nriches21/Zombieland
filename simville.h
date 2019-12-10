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

#include "denizen.h"
#include "district.h"
#undef max

//using namespace std;
using std::cout;
using std::endl;
using std::setw;
using std::setfill;
using std::list;
using std::map;
using std::queue;
using std::random_device;
using std::array;
using std::to_string;

class Simville {
private:
	int dayT;
	int hourT;
	int zSum;
	int aSum;
	int iSum;
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
			cout << endl << setw(17) << " " << setfill('=') << setw(19) << " DAY ";
			cout << dayT << " " << setw(15) << " " << setfill(' ') << endl;
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
			hours = "DAY " + to_string(dayT) + ", 24:00";
			return hours;
		}
		else {
			hours = "DAY " + to_string(dayT) + ", " + to_string(hourT - 6) + ":00";
			return hours;
		}
	}

	void addDist(District* d) { districts.push_back(d); }

	/*
	* Runs and prints simulation for each district. 
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
				SetConsoleTextAttribute(hConsole, 2);	cout << setw(42) << hourShow() << endl;
				SetConsoleTextAttribute(hConsole, 7);	cout << setfill('.') << setw(80) << " " << setfill(' ');
				cout << endl << endl << endl;
			}

			dist->printPop(verbose);
			dist->alarm();
			dist->bite();
			move(dist);
			resetTurn(dist);
			
			it++;
		}
	}

	/*
	* Prints the sum total of all zombies, alarmed and ignorant denizens from all districts.
	*/
	void simvilleSum() {

		HANDLE hConsole; //Colored Console Text
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		cout << " Before and After:" << endl << std::setw(10) << " ALL ZOMBIES: ";
		SetConsoleTextAttribute(hConsole, 4);	cout << zSum;
		SetConsoleTextAttribute(hConsole, 7);	cout << std::setw(19) << "ALL ALARMED: ";
		SetConsoleTextAttribute(hConsole, 6);	cout << aSum;
		SetConsoleTextAttribute(hConsole, 7);	cout << std::setw(18) << "ALL IGNORANT: ";
		SetConsoleTextAttribute(hConsole, 15);	cout << iSum;
		SetConsoleTextAttribute(hConsole, 7);	cout << std::setw(16) << "ALL TOTAL: " << zSum + aSum + iSum << "\n\n";

		zSum = 0;
		aSum = 0;
		iSum = 0;

		std::list<District*>::iterator it = districts.begin();
		while (it != districts.end()) {
			District* dist = *it;
			dist->countPop();
			zSum += dist->zombieTotal();
			aSum += dist->alarmedTotal();
			iSum += dist->ignorantTotal();
			it++;
		}

		cout << std::setw(10) << " ALL ZOMBIES: ";
		SetConsoleTextAttribute(hConsole, 4);	cout << zSum;
		SetConsoleTextAttribute(hConsole, 7);	cout << std::setw(19) << "ALL ALARMED: ";
		SetConsoleTextAttribute(hConsole, 6);	cout << aSum;
		SetConsoleTextAttribute(hConsole, 7);	cout << std::setw(18) << "ALL IGNORANT: ";
		SetConsoleTextAttribute(hConsole, 15);	cout << iSum;
		SetConsoleTextAttribute(hConsole, 7);	cout << std::setw(16) << "ALL TOTAL: " << zSum + aSum + iSum << "\n\n\n";

		cout << setfill('|') << std::setw(80) << " " << setfill(' ') << "\n";
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
		int total = 2000;
		//int total = 150; //For testing, delete later

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
	* Used to create zombies at the start of the simulation.
	* Given a district pointer and n amount of zombies to create, takes a random denizen
	* from that district's populace list and checks to see if they are already a zombie.
	* If not, the denizen's name is used to create a new zombie and the denizen is deleted.
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

	/**
	* Used to create any alarmed at the start of the simulation.
	* Given a district pointer and n amount of alarmed to create, takes a random denizen
	* from that district's populace list and checks to see if they can be alarmed.
	* If not, the denizen's name is used to create a new alarmed and the denizen is deleted.
	**/
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
			}
		}
	}

	void move(District* dist) { 
		list<Denizen*>* pop = dist->getPopulace();
		std::list<Denizen*>::iterator mov = pop->begin();
		int toWork = 0;
		int toHome = 0;
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
								//cout << " " << newd->getName() << " is going to work." << endl;
								newd->setBiteChance(current->getBiteChance() + d->getDensity());
								newd->setTurnOver(true);
								d->addDenizen(newd);
								mov = pop->erase(mov);
								toWork++;
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
								//cout << " " << newd->getName() << " is going home." << endl;
								newd->setBiteChance(current->getBiteChance() + d->getDensity());
								newd->setTurnOver(true);
								d->addDenizen(newd);
								mov = pop->erase(mov);
								toHome++;
							}
							iterd++;
						}
					}
				}
				else { //Buggy
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
						cout << " " << newd->getName() << " is screaming and running to " << moveTo->getName() << endl;
						newd->setTurnOver(true);
						moveTo->addDenizen(current);
					}
					else {
						Denizen* newd = new Zombie(current->getName());
						cout << " " << newd->getName() << " is a zombie on the move to " << moveTo->getName() << endl;
						newd->setTurnOver(true);
						moveTo->addDenizen(current);
					}
					mov = pop->erase(mov);
				}
			}
			mov++;
		} 
			cout << " Denizens commuting to work: " << toWork++ << endl;
			cout << " Denizens commuting home: " << toHome++ << endl;
	} 

	void resetTurn(District* dist) {
		list<Denizen*>* pop = dist->getPopulace();
		std::list<Denizen*>::iterator den = pop->begin();
		while (den != pop->end()) {
			Denizen* d = *den;
			if (d->getTurnOver() == true) {
				d->setTurnOver(false);
			}			
			den++;
		}		
		cout << endl;
	}

};

#endif