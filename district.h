#ifndef DISTRICT_H_
#define DISTRICT_H_

#include <queue>
#include <iostream>
#include <random>

class District {
private:
	string name;
	list<Denizen*> populace;
	map<char, District*> connections;
	int zombie;
	int alarmed;
	int ignorant;

public:
	District(string name) : name(name) {}

	string getName() const { return name; }

	int zombieTotal() { return zombies; }
	int alarmedTotal() { return alarmed; }
	int ignorantTotal() { return ignorant; }

	void addDenizen(Denizen* de) { populace.push_back(de); }
	///////////////////////////
	////////////////////////
	//////////////////////
	//////////////////START UNIT TESTING HERE!!!!!

	//iterate through the population in the main
	void setBiteChance(Denizen inputDenizen) {
		if (inputDenizen.getStatus() == "alarmed") {
			inputDenizen.biteChance = 25 + density; //set the bite chance to be 25 (base chance for alarmed) and add the density of the district (some number between 0 and 50)
		}
		if (inputDenizen.getStatus() == "ignorant") {
			inputDenizen.biteChance = 45 + density; //set the bite chance to be 45 (base chance for ignorant) and add the density of the district (some number between 0 and 50)
		}
	}

	bool compare(const Denizen& first, const Denizen& second) {
		return (first > second); //return true if first is greater than the last
		//http://www.cplusplus.com/forum/beginner/91610/ help from this -- use it because idk if this is right
	}

	void sortPopulace() {
		populace.sort(compare); //automatically sorts in descending order(using the operator < overload int eh denizen class)
	}

	void bite() {
		if (zombie != 0) {
			queue<Denizen*> biteAttempt; //queue of denizens that are going to be attempted to be bitten by zombies based on bite chance
		}
		else {
			return;
		}
		sortPopulace(); //sort populace in descending order by bite chance
		std::list<Denizen*>::<iterator> it = populace.begin(); //iterator pointing to the beginning of the list
		for (int i = zombie; i != 0; i--) { //for loop that runs for the number of zombiest there are
			while (it != populace.end()) { //as long as we are not at the end of the populace, keep iterating
				if (*it->getStatus != "zombie") { //check if we are at zombies
					biteAttempt.push(it*); //if it is not a zombie, push the denizen into the queue to be attempted to be bitten
				}
				it++; //increment the iterater
			}
		}

		for (int i = zombie; (i != 0) && (!biteAttempt.empty()); i--) { //iterate through for the number of zombies as long as there are people in the queue
			std::list<Denizen*>::<iterator> zom = populace.end(); //iterator to the end of the list (should point to the zombies)
			while (zom == (zom->getStatus() == "zombie")) { //go through the list for the number of zombies
				zom--; //decrement the iterator
				zom->setTurnOver(true); //simulate that a zombie attempted to bite and used their turn this time tick
			}
			random_device ran;
			int prob = 1 + ran() % 10;
			if (prob <= 8) { //bite successful
				std::list<Denizen*>::<iterator> del = populace.begin();
				while (del != biteAttempt.begin()) { //until we have found the correct person in the list, continue incrementing the iterator
					del++; 
				}
				populace.end() = new Zombie(del->getName()); //create a new zombie and push it to the end of the list
				populace.erase(del); //delete the person at that iterator
				populace.end()->setTurnOver(true); //set the new zombies turnOver to be true
				biteAttempt.pop(); //delete the person from the queue
			}
			else { //bite unsuccessful
				if (biteAttempt.begin()->getStatus() == "ignorant";) {
					std::list<Denizen*>::<iterator> ig = populace.begin();
					while (ig != biteAttempt.begin()) {
						ig++;
					}
					populace.end() = new Alarmed(ig->getName()); //create a new alarmed and push it to the list
					populace.erase(ig); //delete ignorant
					populace.end()->setTurnOver(true); //set turnover
					biteAttempt.pop(); //delete the person from teh queue
				}
				if (biteAttempt.begin()->getStatus() == "alarmed") {
					biteAttempt.begin()->setTurnOver(true); //set the turn over to be true for the alarmed person 
				}
		}
	}

	void alarmedTellIgnorant() {
		if (alarmed != 0) { //check to make sure there are actually alarmed people in the district
			if (alarmed >= ignorant){ //if there are more alarmed denizens, only iterate through telling people for the number of ignorant
				for (int i = ignorant; i >= 0; i--) { //set the turn over for the alarmed people
					std::list<Denizen*>::<iterator> alarm = populace.begin(); //set an iterator to the beginning of the list
					while (alarm->getStatus != "alarmed") { //increment the iterator until we have found an alarmed person
						alarm++;
					}
					alarm->setTurnOver(true); //set turn over status
				}
				std::list<Denizen*>::<iterator> tell = populace.begin(); //another iterator pointing to the beginnning of the list
				for (int i = ignorant; i >= 0; i--){
					while (tell.getStatus != "ignorant") { //find an ignorant person
						tell++;
					}
					populace.end() = new Alarmed(tell->getName()); //make a new alarmed person
					populace.erase(tell); //erase the ignorant person
					populace.end()->setTurnOver(true); //set the turn over variable
				}
			}
			else { //if there are more ignorant, only iterate through telling people for the number of alarmed
				for (int i = alarmed; i >= 0; i--) { //set the turn over for the alarmed people
					std::list<Denizen*>::<iterator> alarm = populace.begin(); //set an iterator to the beginning of the list
					while (alarm->getStatus != "alarmed") { //increment the iterator until we have found an alarmed person
						alarm++;
					}
					alarm->setTurnOver(true); //set turn over status
				}
				std::list<Denizen*>::<iterator> tell = populace.begin(); //another iterator pointing to the beginnning of the list
				for (int i = alarmed; i >= 0; i--) {
					while (tell.getStatus != "ignorant") { //find an ignorant person
						tell++;
					}
					populace.end() = new Alarmed(tell->getName()); //make a new alarmed person
					populace.erase(tell); //erase the ignorant person
					populace.end()->setTurnOver(true); //set the turn over variable
				}
			}
		}
	}

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

	//needs to be called every time click for every district
	void resetTurnOver() {
		std::list<Denizen*>::<iterator> it = populace.begin(); //iterator to the beginning of the populace
		while (it != populace.end()) { //loop until we have reached the end
			it->setTurnOver(false); //set the turn over status to be false
		}
	}
	////////////////////////////
	///////////////////////
	///////////////////////
	///////////////////
	/////////////END OF UNTESTED MATERIAL

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

			Denizen* pointr = *it;
			if (verbose != false) {
				if (column == 4) {
					column = 0;
					cout << endl;
				}
				column++;

				cout << setw(20) << pointr->getName();
			}

			if (pointr->getStatus() == "Zombie") ++zombies;
			else if (pointr->getStatus() == "Alarmed") ++alarmed;
			else ++ignorant;
			++it;
		}

		if (verbose != false) {
			cout << endl << setw(18) << "ZOMBIES: " << zombies << setw(19) << "ALARMED: " << alarmed << setw(18);
			cout << "IGNORANT: " << ignorant << setw(18) << "TOTAL: " << zombies + alarmed + ignorant << endl;
			cout << setfill('-') << setw(80) << " " << setfill(' ') << endl << endl;
		}
	}

};

#endif