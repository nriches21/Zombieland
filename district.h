#ifndef DISTRICT_H_
#define DISTRICT_H_

#include <iostream>
#include <iomanip>
#include <list>
#include <random>
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
	map<char, District*>connections; //north/south/east/west char and connected district (ex., N, Downtown means go north to reach Downtown)
	int zombies;
	int alarmed;
	int ignorant;
	int density; //An int between 0 - 50 to be set when district is initialized

public:
	District(string name) : name(name), zombies(0), alarmed(0), ignorant(0) {}

	string getName() const{ return name; }
	int zombieTotal() { return zombies; }
	int alarmedTotal() { return alarmed; }
	int ignorantTotal() { return ignorant; }
	list<Denizen*>* getPopulace() { return &populace; }
	void addDenizen(Denizen* de) { populace.push_back(de);	}

	//From Nora branch
	void setBiteChance(Denizen* inputDenizen) {
		if (inputDenizen->getStatus() == "Alarmed") {
			inputDenizen->setBiteChance(25 + density);
			//inputDenizen->biteChance = 25 + density; //set the bite chance to be 25 (base chance for alarmed) and add the density of the district (some number between 0 and 50)
		}
		if (inputDenizen->getStatus() == "Ignorant") {
			inputDenizen->setBiteChance(45 + density);
			//inputDenizen->biteChance = 45 + density; //set the bite chance to be 45 (base chance for ignorant) and add the density of the district (some number between 0 and 50)
		}
	}

	//deleted compare() from Nora branch
	//bool sortBiteChance(Denizen& A, Denizen& B) {
	//	return (A.getBiteChance() < B.getBiteChance());
	//}

	bool sortBiteChance() {
		list<Denizen*>::iterator it1 = populace.begin();
		list<Denizen*>::iterator it2 = populace.begin();
		it2++;
	
		while (it1 != populace.end() && it2 != populace.end()) {
			Denizen* Dit1 = *it1;
			Denizen* Dit2 = *it2;
			bool sortything;

			
			if (Dit1->getBiteChance() < Dit2->getBiteChance()) {
				cout << "Dit1: " << Dit1->getBiteChance() <<" < Dit2: " << Dit2->getBiteChance() << endl;
				return true;
			}
			//else if (Dit1->getBiteChance() == Dit2->getBiteChance()){
			
			//}
			else {
				cout << "Dit1: " << Dit1->getBiteChance() << " > Dit2: " << Dit2->getBiteChance() << endl;
				return false;
			}
			it1++;
			it2++;
		}

	}

	//bool biteChanceFirst(Denizen* a, Denizen* b) { return a->getBiteChance() < b->getBiteChance(); }

	/*void sortPopulace() { //From Nora branch
		list<Denizen*>::iterator it1 = populace.begin();
		list<Denizen*>::iterator it2 = populace.begin();
		it2++;
		while (it1 != populace.end() && it2 != populace.end()) {
			Denizen* Dit1 = *it1;
			Denizen* Dit2 = *it2;
			populace.sort(sortBiteChance); //automatically sorts in descending order(using the operator < overload int eh denizen class)
			it1++;
			it2++;
		}
	}*/
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

	/*void bite() {

		queue<Denizen*> biteAttempt;
		
		if (zombies > 0) {
			sortPopulace();
			int zNumber = 0;
			//Add to the queue as many denizens as there are zombies
			while (zNumber <= zombies) { 
				std::list<Denizen*>::iterator it = populace.begin();
				while (it != populace.end()) {
					Denizen* Dpointer = *it;
					if (Dpointer->getStatus() == "Zombie") {
						//it++;
						break;	// First element of sorted populace is zombie, nothing to bite- 
								// Don't need to iterate through list, since zombies have the lowest biteChance
					}
					else {
						biteAttempt.push(Dpointer);
						it++;
						zNumber++; //One more zombie has been added to the queue
					}
				}
			} 
			/*for (int i = 0; i < zombies; i++) {

			}*/
		/*}
		else { return; }
	}*/

/*	void bite() {
		if (zombies != 0) {
			queue<Denizen*> biteAttempt; //queue of denizens that are going to be attempted to be bitten by zombies based on bite chance
			//sortPopulace(); //sort populace in descending order by bite chance
			std::list<Denizen*>::iterator it = populace.begin(); //iterator pointing to the beginning of the list
			for (int i = zombies; i != 0; i--) { //for loop that runs for the number of zombiest there are
				while (it != populace.end()) { //as long as we are not at the end of the populace, keep iterating
					Denizen* Dpointer = *it;
					if (Dpointer->getStatus() != "Zombie") { //check if we are at zombies

						biteAttempt.push(*it); //if it is not a zombie, push the denizen into the queue to be attempted to be bitten
					}
					it++; //increment the iterater
				}
			}

			for (int i = zombies; (i != 0) && (!biteAttempt.empty()); i--) { //iterate through for the number of zombies as long as there are people in the queue
				std::list<Denizen*>::iterator zom = populace.end(); //iterator to the end of the list (should point to the zombies)
				while (zom == (zom->getStatus() == "Zombie")) { //go through the list for the number of zombies
					zom--; //decrement the iterator
					zom->setTurnOver(true); //simulate that a zombie attempted to bite and used their turn this time tick
				}
				random_device ran;
				int prob = 1 + ran() % 10;
				if (prob <= 8) { //bite successful
					std::list<Denizen*>::iterator del = populace.begin();
					while (del != biteAttempt.begin()) { //until we have found the correct person in the list, continue incrementing the iterator
						del++;
					}
					populace.end() = new Zombie(del->getName()); //create a new zombie and push it to the end of the list
					populace.erase(del); //delete the person at that iterator
					populace.end()->setTurnOver(true); //set the new Zombies turnOver to be true
					biteAttempt.pop(); //delete the person from the queue
				}
				else { //bite unsuccessful
					if (biteAttempt.begin()->getStatus() == "Ignorant";) {
						std::list<Denizen*>::iterator ig = populace.begin();
						while (ig != biteAttempt.begin()) {
							ig++;
						}
						populace.end() = new Alarmed(ig->getName()); //create a new alarmed and push it to the list
						populace.erase(ig); //delete ignorant
						populace.end()->setTurnOver(true); //set turnover
						biteAttempt.pop(); //delete the person from teh queue
					}
					if (biteAttempt.begin()->getStatus() == "Alarmed") {
						biteAttempt.begin()->setTurnOver(true); //set the turn over to be true for the alarmed person 
					}
				}
			}
		}
		else {
			return;
		}
		

	}*/


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

				cout << ":  " << pointr->getBiteChance() << "  +++++++++++++++++++++++++++++++++" << endl;
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