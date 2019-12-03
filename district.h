#ifndef DISTRICT_H_
#define DISTRICT_H_

class District {
private:
	string name;
public:
	District(string name) : name(name) {}
	string getName() const{ return name; }
<<<<<<< Updated upstream
=======

	int zombieTotal() { return zombies; }
	int alarmedTotal() { return alarmed; }
	int ignorantTotal() { return ignorant; }

	void addDenizen(Denizen* de) { populace.push_back(de);	}

	//iterate through the population
	//setBiteChance in here??
	//	if (alarmed){
	//		biteChance = 25;
	//		biteChance += density of district; //need to make sure that the density of the district is somewhere between 0 and 50
	//	}
	// if (ignorant){
	//		bitechance = 45;
	//		biteChance += density of district; //need to make sure that the density of the district is somewhere between 0 and 50
	//	}

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

	//alarmedTellIgnorant()	
	/**
	* Each district checks to see if there are alarmed, if yes-
	* for loop the size of numalarmed
	* iterate through list of people
	* if ignorant, delete them and create a new alarmed 
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

>>>>>>> Stashed changes
};

#endif