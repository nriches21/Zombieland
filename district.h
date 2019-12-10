#ifndef DISTRICT_H_
#define DISTRICT_H_

#include <iostream>
#include <iomanip>
#include <list>
#include <random>
#include <algorithm>
#include <stdio.h> 
#include <stdlib.h>
#include <queue>
#include "denizen.h"
#include <map>
#include <windows.h>

using std::cout;
using std::endl;
using std::setw;
using std::setfill;
using std::list;
using std::map;
using std::queue;



class District {
private:
	string name;
	list<Denizen*>populace;
	map<char, District*>connections; // n/s/e/w char and connected district pointer(ex: N, Downtown means go north to reach Downtown)
	queue<Denizen*>biteAttempt;
	int zombies;
	int alarmed;
	int ignorant;
	int density; //An int between 0 - 50 to be set when district is initialized

public:
	District(string name) : name(name), zombies(0), alarmed(0), ignorant(0), density(0) {}

	string getName() { return name; }

	int zombieTotal() { return zombies; }
	int alarmedTotal() { return alarmed; }
	int ignorantTotal() { return ignorant; }

	list<Denizen*>* getPopulace() { return &populace; }
	
	map<char, District*>* getConnections() { return &connections; }
	
	void addDenizen(Denizen* de) { populace.push_back(de);	}
	
	int getDensity() { return density; }
	
	void addConnection(char direction, District* location) { connections.insert({ direction, location }); }

	//Fill biteAttempt cue for bite() to use. Finds <Qsize> number of Denizens to push to queue
	void fillQueue(int Qsize) {
		while (!biteAttempt.empty()) {
			biteAttempt.pop();
		}
		list<Denizen*> Cpopulace;
		Cpopulace = populace;
		int j = 0;
		while (j < Qsize) {
			list<Denizen*>::iterator iter = Cpopulace.begin();
				
				Denizen* max = Cpopulace.front();
				Denizen* D;
				for (int i = 0; i < Cpopulace.size(); i++) {
					while (iter != Cpopulace.end()) {
						D = *iter;
						if (D->getBiteChance() > max->getBiteChance()) {
							max = D;
							Cpopulace.splice(Cpopulace.begin(), Cpopulace, iter);
						}
						else {
							++iter;
						}
					}
				}
				biteAttempt.push(max);
				Cpopulace.pop_front();
				j++;
		}
	}

	/**
	* Each district checks to see if there are zombies, if yes- 
	* then creates a queue of citazens based on bite chance that is the 
	* size of the # of zombies which pushes denizens in based on bite chance, 
	* and a for loop for # of zombies which determines whether bite was successful- 
	*	If success, 
	* Another person is pushed into the queue if that person in the queue moves to another district
	* Map or list of denizens sorted by bite chance 
	**/

	void bite() { //From Nora's branch

		if (zombies != 0) {
			srand(time(0));
			int j = rand() % zombies;
			int i = 0;

			if (zombies < 5) {
				j = zombies;
			}
			

			fillQueue(j); //Get as many Denizens to bite as there are zombies
			
			std::list<Denizen*>::iterator zom = populace.begin(); //iterator to the end of the list (should point to the zombies)
			while (zom != populace.end()) {
				Denizen* z = *zom;
				if (z->getStatus() == "Zombie") { //go through the list for the number of zombies
					z->setTurnOver(true); //simulate that a zombie attempted to bite and used their turn this time tick
					i++;
				}
				if (i == j) { break; }
				zom++;
			}

			srand(time(0));

			std::list<Denizen*>::iterator del = populace.begin();
			while (del != populace.end() && biteAttempt.size() != 0) {
				int prob = 1 + rand() % 10;
				if (prob <= 8) { //bite successful
					Denizen* d = *del;
					if (d == biteAttempt.front()) {
						populace.push_back(new Zombie(d->getName()));
						del = populace.erase(del); //delete the person at that iterator
						populace.back()->setTurnOver(true); //set the new Zombies turnOver to be true
						cout << " " << d->getName() << " has been bitten" << endl;
						biteAttempt.pop(); //delete the person from the queue
						del = populace.begin();
					}
					++del;
				}
				
				else { //bite unsuccessful
					
					if (biteAttempt.front()->getStatus() == "Ignorant") {
						Denizen* i = *del;
						if (i == biteAttempt.front()) {
							int chance = i->getBiteChance() - 20;
							populace.push_back(new Alarmed(i->getName(), chance)); //create a new alarmed and push it to the list
							del = populace.erase(del); //delete ignorant
							populace.back()->setTurnOver(true); //set turnover
							cout << " " << i->getName() << " avoided being bitten and is now alarmed" << endl;
							biteAttempt.pop(); //delete the person from teh queue
							del = populace.begin();
						}
						del++;
					}
					
					else if (biteAttempt.front()->getStatus() == "Alarmed") {
						cout << " " << biteAttempt.front()->getName() << " avoided being bitten and is already alarmed" << endl;
						biteAttempt.front()->setTurnOver(true); //set the turn over to be true for the alarmed person
						biteAttempt.pop();
						del = populace.begin();
					}
				}
			}
		}
		
	}

	void alarm() { //From Nora's branch
		if (alarmed != 0) {
			srand(time(0));

			int numA = rand() % alarmed;
			int numB = alarmed - numA;
			int i = 0;
			std::list<Denizen*>::iterator alrm = populace.begin(); //iterator to the end of the list (should point to the zombies)
			while (alrm != populace.end()) {
				Denizen* a = *alrm;
				if (a->getStatus() == "Alarmed") { //go through the list for the number of zombies
					a->setTurnOver(true); //simulate that a zombie attempted to bite and used their turn this time tick
					i++;
				}
				if (i == numA) { break; }
				alrm++;
			}

			
			std::list<Denizen*>::iterator del = populace.begin();
			int randDenizen = rand() % populace.size();
			advance(del, randDenizen);
			while (del != populace.end() && numB != 0) {
				Denizen* d = *del;
					if (d->getStatus() == "Ignorant") {
						int prob = 1 + rand() % 10;
						if (prob <= 7) { //alarm successful
							
								int chance = d->getBiteChance() - 20;
								populace.push_back(new Alarmed(d->getName(), chance));
								del = populace.erase(del); //delete the person at that iterator
								populace.back()->setTurnOver(true); //set the new Alarmed turnOver to be true
								cout << " " << d->getName() << " has been alarmed" << endl;
								//biteAttempt.pop(); //delete the person from the queue
								numB--;
							
						}
					}
					++del;
				}
		}
	}

	void countPop() {
		zombies = 0;
		alarmed = 0;
		ignorant = 0;
		list<Denizen*>::iterator it = populace.begin();
		while (it != populace.end()) {
			Denizen* pointr = *it;
			if (pointr->getStatus() == "Zombie") ++zombies;
			else if (pointr->getStatus() == "Alarmed") ++alarmed;
			else ++ignorant;
			++it;
		}
	}

	/*
	* Prints population of zombies, alarmed and ignorant denizens in this district.
	* Also prints the name of each denizen.
	*/

	void printPop(bool verbose) {
		HANDLE hConsole;
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		
		zombies = 0;
		alarmed = 0;
		ignorant = 0;
		int column = 0;

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
					SetConsoleTextAttribute(hConsole, 4);	cout << std::setw(13) << pointr->getName();
					SetConsoleTextAttribute(hConsole, 7);
				}
				else if (pointr->getStatus() == "Alarmed") {
					SetConsoleTextAttribute(hConsole, 6);	cout << std::setw(13) << pointr->getName();
					SetConsoleTextAttribute(hConsole, 7); 
				}
				else {
					cout << std::setw(13) << pointr->getName();
				}

				//Uncomment to see bite chance, work and home districts per ignorant denizen
				/*
				cout << ": " << std::left << setw(4) << pointr->getBiteChance() << "  -  "; 
				Ignorant* ig = dynamic_cast<Ignorant*>(pointr);
				if (ig) { 
					cout << "Home: " << setw(20) <<  ig->getHome();
					cout << "Work: " << setw(20) << ig->getWork(); 
					cout << std::right << endl;
				}
				*/
			}

			if (pointr->getStatus() == "Zombie") ++zombies;
			else if (pointr->getStatus() == "Alarmed") ++alarmed;
			else ++ignorant;
			++it;
		}

		if (verbose != false) {
			cout << endl << endl << std::setw(12) << "ZOMBIES: ";
			SetConsoleTextAttribute(hConsole, 4);	cout << zombies;
			SetConsoleTextAttribute(hConsole, 7);	cout << std::setw(19) << "ALARMED: ";
			SetConsoleTextAttribute(hConsole, 6);	cout << alarmed;
			SetConsoleTextAttribute(hConsole, 7);	cout << std::setw(18) << "IGNORANT: ";
			SetConsoleTextAttribute(hConsole, 15);	cout << ignorant;
			SetConsoleTextAttribute(hConsole, 7);	cout << std::setw(16) << "TOTAL: " << zombies + alarmed + ignorant << endl;
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