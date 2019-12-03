#ifndef SIMVILLE_H_
#define SIMVILLE_H_

#include <iostream>
#include <map>
#include <iomanip>
#include "denizen.h"
#include "district.h"

using std::cout;
using std::endl;
using std::setw;
using std::multimap;
using std::pair;


class Simville {
private:
	int dayT;
	int hourT;
	//multimap<const Denizen*, District> population;
	multimap<const District*, Denizen> population2;
	multimap<const District*, Denizen*> population3;

public:
	Simville() : dayT(0), hourT(0) {}; //Simville has a default day 0 and hour 0

	void hourTick() {
		if (hourT == 0) {
			hourT = 6;
			++dayT;
			cout << endl << setw(17) << "Day " << dayT << endl;
			hourTick();
		}
		else if (hourT == 24) {
			cout << "0:00" << endl;
			hourT = 0;
		}
		else {
			cout << hourT << ":00" << endl;
			hourT = (hourT + 6);
		}
	}

	/*void addDenizen(const Denizen* de, District di) {
		population.insert(pair<const Denizen*, District>(de, di));
	};

	//Takes denizen, returns a district according to population map
	District retDistrict(const Denizen* de) {
		return population.find(de)->second;
	}*/

	//Need district as key, denizen population as output
	void addDenizen2(const District* di, Denizen de) {
		population2.insert(pair<const District*, Denizen>(di, de));
	};

	int districtPop(const District* di) {
		return population2.count(di);
	}

	void printPop(const District* di) {
		/* //This would print the entire count as many times as there are denizens (which is a heckin' lot)
		for (multimap<const District*, Denizen>::iterator it = population2.begin();
			it != population2.end();) {	
			const District * dist = (*it).first;
			cout << dist->getName() << " population: " << population2.count(dist);
			++it;
		}*/
		cout << di->getName() << " population: " << population2.count(di) << endl;
	}

	//Accounting for derived classes of Denizen, meaning that map has to be of Districts and Denizen pointers
	/*void addDenizen3(const District* di, Denizen* de) {
		population3.insert(pair<const District*, Denizen*>(di, de));
	};*/

	void printPop3(const District* di) {
		int zombies, alarmed = 0;
		for (multimap<const District*, Denizen>::iterator it = population2.begin();
			it != population2.end();) {
			//const District* dist = (*it).first;
			cout << di->getName() << " population: " << endl;
			//  B* b = dynamic_cast<B *>(a[0]);
			/*Denizen* personptr = &(population2.find(di)->second);
			Zombie* person = dynamic_cast<Zombie*>(*personptr);*/
			/*
			if (person)
			{
				// This animal is not a dog.
			}
			else continue;
			++it;*/
		}
		
	}


};

#endif