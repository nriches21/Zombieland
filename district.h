#ifndef DISTRICT_H_// defined the class district
#define DISTRICT_H_

#include <iostream>// included all the required headers
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

using std::cout;//defined the namespace for using cout
using std::endl;//defined the namespace for using endl
using std::setw;//defined the namespace for using setw
using std::setfill;//defined the namespace for using setfill
using std::list;//defined the namespace for using list
using std::map;//defined the namespace for using map
using std::queue;//defined the namespace for using queue


// class District
// contains the map of distrcits, the list of Denizens, and the queue of Denzens
class District {
private:
	string name;// stores the name of the Districts
	list<Denizen*>populace;// a list that stores all the denizens
	map<char, District*>connections; //north/south/east/west char and connected district (ex., N, Downtown means go north to reach Downtown)
	queue<Denizen*>biteAttempt;// a queue in which bitten denizens are pushed
	int zombies;// the number of zombies
	int alarmed;// the number of alarmed
	int ignorant;// the number of ignoorants
	int density; //An int between 0 - 50 to be set when district is initialized

public:

	/*
		constructor for the District class that sets the number of zombies, ignorants, alarmed, and sets the denisity
	*/
	District(string name) : name(name), zombies(0), alarmed(0), ignorant(0), density(0) {}

	/*
		getter method that returns the name of district
	*/
	string getName() { return name; }

	/*
		getter method that returns the total number of zombies
	*/
	int zombieTotal() { return zombies; }

	/*
		getter method that returns the total number of alarmed
	*/
	int alarmedTotal() { return alarmed; }

	/*
		getter method that returns the total number of ignorant
	*/
	int ignorantTotal() { return ignorant; }

	/*
		getter method that returns the pointer to the Denizen from the list
	*/
	list<Denizen*>* getPopulace() { return &populace; }

	/*
		getter method that returns the pointer to Districr from the map
	*/

	/*
		getter method that returns the pointer to the Denizen from the list
	*/
	map<char, District*>* getConnections() { return &connections; }

	/*
		add Denizen
		pushes back the pointer of Denizen to the list of Denizens
	*/
	void addDenizen(Denizen* de) { populace.push_back(de);	}

	/*
		remove Denizen
		removes the denizen from the list of denizens
	*/
	void removeDenizen(Denizen* de) {
		cout << " Removing Denizen " << de->getName() << endl;
		populace.remove(de); }

	/*
		getter method
		returns the density of a district
	*/
	int getDensity() { return density; }

	/*
		add Connections
		adds the direction and location of districts to the map of the districts
	*/
	void addConnection(char direction, District* location) { connections.insert({ direction, location }); }

	/*
		void fillQueue
		takes in the size of the queue and adds the denizens to the queue that are attempted to be bitteh
	*/
	void fillQueue(int Qsize) {
		while (!biteAttempt.empty()) {// while we are not at the end of the queue
			biteAttempt.pop();// we pop the element
		}
		list<Denizen*> Cpopulace;// we create another list in order to preserve the original list
		Cpopulace = populace;
		int j = 0;// an integer that keeps track of the size so we don' exceed the size of the queue
		while (j < Qsize) {// while we don't exceed the size of the queue
			list<Denizen*>::iterator iter = Cpopulace.begin();// an iterator pointing to the beginning of the list
				
				Denizen* max = Cpopulace.front();// a pointer that points to the front element of the list
				Denizen* D;// another pointer to denizen
				for (int i = 0; i < Cpopulace.size(); i++) {// a for loop iterates through the entire list
					while (iter != Cpopulace.end()) {// and while we are not at the end of the list
						D = *iter;// we derefrence the iteratot to the pointer
						if (D->getBiteChance() > max->getBiteChance()) {// we make sure that the biteChance of Denizen is less  
							max = D;//they are in the front of the list now
							Cpopulace.splice(Cpopulace.begin(), Cpopulace, iter);// the splice method splite up the list at the postion where the iterator is pointing to
						}
						else {
							++iter;// we just increment the operator otherwise and keep checking
						}
					}
				}
				biteAttempt.push(max);// we must the denizen into the queue
				Cpopulace.pop_front();// and we that denizen from the list
				j++;// increment the int to change the size of the queue
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

/*
	bite method
	zombies bite the ignorants and they turn into alarmed

*/
	void bite() {

		if (zombies != 0) {
			srand(time(0));
			int j = rand() % zombies;// we produce a random number of zombies
			int i = 0;

			if (zombies < 5) {// if we have less than 5 zombies
				j = zombies;// then the rand is equated to the number of zombies
			}
			

			fillQueue(j); //Get as many Denizens to bite as there are zombies
			
			std::list<Denizen*>::iterator zom = populace.begin(); //iterator to the end of the list (should point to the zombies)
			while (zom != populace.end()) {
				Denizen* z = *zom;
				if (z->getStatus() == "Zombie") { //go through the list for the number of zombies
					z->setTurnOver(true); //simulate that a zombie attempted to bite and used their turn this time tick
					i++;
				}
				if (i == j) { break; }// in case they become equal, we break
				zom++;// and we increment the iterator
			}

			srand(time(0));

			std::list<Denizen*>::iterator del = populace.begin();
			while (del != populace.end() && biteAttempt.size() != 0) {
				int prob = 1 + rand() % 10;// we decide a probabilty of being bitte on a sclae of 1-10
				if (prob <= 8) { //bite successful 
					Denizen* d = *del;
					if (d == biteAttempt.front()) {// if we are at the front of the queue
						populace.push_back(new Zombie(d->getName()));// we create a new zombie and push it back into the list
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
							del = populace.begin();// and point the iterator back to the beginning
						}
						del++;// and then increment the iterator
					}
					
					else if (biteAttempt.front()->getStatus() == "Alarmed") {// if the person in the queue is alarmed
						cout << " " << biteAttempt.front()->getName() << " avoided being bitten and is already alarmed" << endl;
						biteAttempt.front()->setTurnOver(true); //set the turn over to be true for the alarmed person
						biteAttempt.pop();// we pop him from the queue
						del = populace.begin();// and we go back again to the beginning of the list
					}
				}
			}
		}
		
	}

	//void alarmed
	// makes a pdenizen alarmed if they are bitten 
	void alarm() { //From Nora's branch
		if (alarmed != 0) {
			srand(time(0));

			int numA = rand() % alarmed;// we generate a random number of alarmed people
			int numB = alarmed - numA;// this is the difference of the range
			int i = 0;
			std::list<Denizen*>::iterator alrm = populace.begin(); //iterator to the end of the list (should point to the zombies)
			while (alrm != populace.end()) {
				Denizen* a = *alrm;// a pointer to the Denizen that derefrences the iterator
				if (a->getStatus() == "Alarmed") { //go through the list for the number of zombies
					a->setTurnOver(true); //simulate that a zombie attempted to bite and used their turn this time tick
					i++;
				}
				if (i == numA) { break; }// if we exceed the range
				alrm++;// we increment the iterator
			}

			
			std::list<Denizen*>::iterator del = populace.begin();// another iterator
			int randDenizen = rand() % populace.size();// rand number between 0 and the size of the list
			advance(del, randDenizen);// advances the iterator by n element positions
			while (del != populace.end() && numB != 0) {// while we are not at the end of the list & numB is not zero
				Denizen* d = *del;// a pointer to denizen
					if (d->getStatus() == "Ignorant") {// if a denizen is ignorant
						int prob = 1 + rand() % 10;// we decide the probabilty again
						if (prob <= 7) { //alarm successful
							
								int chance = d->getBiteChance() - 20;// we change the biteChance
								populace.push_back(new Alarmed(d->getName(), chance));// and we create a new alarmed since the person is now alarmed
								del = populace.erase(del); //delete the person at that iterator
								populace.back()->setTurnOver(true); //set the new Alarmed turnOver to be true
								cout << " " << d->getName() << " has been alarmed" << endl;
								numB--;// we decrement the integer
							
						}
					}
					++del;// and we increment the ierator
				}
		}
	}

	/*
	* Prints population of zombies, alarmed and ignorant denizens in this district.
	* Also prints the name of each denizen.
	*/

	void countPop() {
		zombies = 0;// we initialize them to 0 again
		alarmed = 0;
		ignorant = 0;
		list<Denizen*>::iterator it = populace.begin();// another iterator to the beginnig of the list
		while (it != populace.end()) {// while we are not at the end of the list
			Denizen* pointr = *it;// another pointer to Denizen
			if (pointr->getStatus() == "Zombie") ++zombies;// increment zombies of the person is a zombie
			else if (pointr->getStatus() == "Alarmed") ++alarmed;// increment alarmed if the person is an alarmed
			else ++ignorant;// increment the ignorant if the person is now ignorant
			++it;// and we keep incrementing the iterator to go through the entire list
		}
	}

	// method that prints the information about the denizens
	// is executed based on user input
	void printPop(bool verbose) {
		zombies = 0;// initialzie all to zero again
		alarmed = 0;
		ignorant = 0;
		int column = 0;

		HANDLE hConsole;// setup for color output
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		list<Denizen*>::iterator it = populace.begin();// an iterator the beginning of the list
		while (it != populace.end()) {// while we are not at the end of the list

			Denizen* pointr = *it;// another pointer to the Denizen class
			if (verbose != false){// if the user doesn't want the extra info
				if (column == 6) {// we set other column options
					column = 0;
					cout << endl;
				}
				column++;// we increment columns

				if (pointr->getStatus() == "Zombie") {// if the denizen is a zombie
					SetConsoleTextAttribute(hConsole, 4); //Red Text
					cout << std::setw(13) << pointr->getName();
					SetConsoleTextAttribute(hConsole, 7); //Back to white Text
				}
				else if (pointr->getStatus() == "Alarmed") {// if the denizen is alarmed
					SetConsoleTextAttribute(hConsole, 6); //Yellow Text
					cout << std::setw(13) << pointr->getName();
					SetConsoleTextAttribute(hConsole, 7); //Back to white Text
				}
				else {
					cout << std::setw(13) << pointr->getName();// sets the width of the columns 
				}

				//cout << ": " << std::left << setw(4) << pointr->getBiteChance() << "  -  "; //Uncomment to see bite chance
				Ignorant* ig = dynamic_cast<Ignorant*>(pointr);// we convent the iterator to the pointer
				//if (ig) {
				//	cout << "Home: " << setw(20) <<  ig->getHome() << "Work: " << setw(20) << ig->getWork(); //Uncomment to see work and home districts
				//}
				//cout << std::right << endl;
			}

			if (pointr->getStatus() == "Zombie") ++zombies;// we increment zombies accordingly
			else if (pointr->getStatus() == "Alarmed") ++alarmed;
			else ++ignorant;
			++it;
		}

		if (verbose != false) {// if the vebose is true, we just print this info
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
		else {// we print this info if the verbose if false
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