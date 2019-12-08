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
	map<char, District*>connections; //north/south/east/west char and connected district (ex., N, Downtown means go north to reach Downtown)
	queue<Denizen*>biteAttempt;
	int zombies;
	int alarmed;
	int ignorant;
	int density; //An int between 0 - 50 to be set when district is initialized

public:
	District(string name) : name(name), zombies(0), alarmed(0), ignorant(0), density(0) {}

	string getName() const{ return name; }
	int zombieTotal() { return zombies; }
	int alarmedTotal() { return alarmed; }
	int ignorantTotal() { return ignorant; }
	list<Denizen*>* getPopulace() { return &populace; }
	void addDenizen(Denizen* de) { populace.push_back(de);	}
	void removeDenizen(Denizen* de) { populace.remove(de); }

	void addConnection(char direction, District* location) { connections.insert({ direction, location }); }

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

	/*bool sortBiteChance() {
		list<Denizen*>::iterator it1 = populace.begin(); //first
		list<Denizen*>::iterator it2 = populace.end();
		int middle = populace.size() / 2;
		list<Denizen*>::iterator it3 = it1; //middle
		advance(it2, middle); //middle
	
		while (it1 != populace.end() && it2 != populace.end()) {
			Denizen* Dit1 = *it1;
			Denizen* Dit2 = *it2;
			Denizen* Dit3 = *it3;
			
			if (Dit1->getBiteChance() < Dit3->getBiteChance()) {
				//cout << "Dit1: " << Dit1->getBiteChance() <<" < Dit2: " << Dit2->getBiteChance() << endl;
				//return true;

				//Call partition
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

	}*/

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

	//Fill biteAttempt cue for bite() to use- find Qsize number of Denizens to push to queue
	void fillQueue(int Qsize) {
		//queue<Denizen*> biteAttempt;
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

	/*void bite() { //Alyssa's version of Nora's bite() function, neither of which currently works 

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

	void bite() { //From Nora's branch

		if (zombies != 0) {

			fillQueue(zombies); //Get as many Denizens to bite as there are zombies
			int i = 0;
			std::list<Denizen*>::iterator zom = populace.begin(); //iterator to the end of the list (should point to the zombies)
			while (zom != populace.end()) {
				Denizen* z = *zom;
				if (z->getStatus() == "Zombie") { //go through the list for the number of zombies
					z->setTurnOver(true); //simulate that a zombie attempted to bite and used their turn this time tick
					i++;
				}
				if (i == zombies) { break; }
				zom++;
			}

			srand(time(0));

			std::list<Denizen*>::iterator del = populace.begin();
			while (del != populace.end() && biteAttempt.size() != 0) {
				int prob = 1 + rand() % 10;
				if (prob <= 7) { //bite successful
					Denizen* d = *del;
					if (d == biteAttempt.front()) {
						populace.push_back(new Zombie(d->getName()));
						del = populace.erase(del); //delete the person at that iterator
						populace.back()->setTurnOver(true); //set the new Zombies turnOver to be true
						cout << d->getName() << " has been bitten." << endl;
						biteAttempt.pop(); //delete the person from the queue
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
							cout << i->getName() << " avoided being bitten and is now alarmed." << endl;
							biteAttempt.pop(); //delete the person from teh queue
						}
						del++;
					}
					
					else if (biteAttempt.front()->getStatus() == "Alarmed") {
						cout << biteAttempt.front()->getName() << " avoided being bitten and is already alarmed." << endl;
						biteAttempt.front()->setTurnOver(true); //set the turn over to be true for the alarmed person
						biteAttempt.pop();
					}
				}
			}
		}
		
	}

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

				cout << ": " << std::left << setw(4) << pointr->getBiteChance() << "  -  ";
				Ignorant* ig = dynamic_cast<Ignorant*>(pointr);
				if (ig) {
					cout << "Home: " << setw(20) <<  ig->getHome() << "Work: " << setw(20) << ig->getWork();
				}
				cout << std::right << endl;
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