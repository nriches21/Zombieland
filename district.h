#ifndef DISTRICT_H_
#define DISTRICT_H_

#include <queue>
#include <iostream>

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
		std::list<iterator> it = populace.begin(); //iterator pointing to the beginning of the list
		for (int i = zombie; i != 0; i--) { //for loop that runs for the number of zombiest there are
			while (it != populace.end()) { //as long as we are not at the end of the populace, keep iterating
				if (*it->getStatus != "zombie") { //check if we are at zombies
					biteAttempt.push(it*); //if it is not a zombie, push the denizen into the queue to be attempted to be bitten
				}
				it++; //increment the iterater
			}
		}

		for (int i = zombie; (i != 0) && (!biteAttempt.empty()); i--) { //iterate through all the zombies as long as there are people in the queue

		}
	}

	//bite()	
	/**
	* Each district checks to see if there are zombies, if yes- done
	* then creates a queue of denizens based on bite chance that is the done
	* size of the # of zombies which pushes denizens in based on bite chance, done
	* and a for loop for # of zombies which determines whether bite was successful-
	*	If success,
	* Another person is pushed into the queue if that person in the queue moves to another district
	* Map or list of denizens sorted by bite chance
	* change turnOver status
	**/

	//alarmedTellIgnorant()	
	/**
	* Each district checks to see if there are alarmed, if yes-
	* for loop the size of numalarmed
	* iterate through list of people
	* if ignorant, delete them and create a new alarmed
	* change their turnOver status once they have been created
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

	//iterate through entire list of denizens and reset turnOver to bool 

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