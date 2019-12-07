#ifndef DENIZEN_H_
#define DENIZEN_H_

#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

class Denizen {
protected:
	string name;
	string status;
	bool turnOver; //Already did an action this turn- if bit this turn, cannot also move and vice versa. Has to be reset after every time tick
	int biteChance;

public:
	Denizen(string name) : name(name) { turnOver = false; } 
	string getName() { return name; }
	string getStatus() { return status; }
	int getBiteChance() { return biteChance; }
	virtual void speak() = 0;

	Denizen* operator < (Denizen* const& i2) {
		if (this->getBiteChance() <= i2->getBiteChance()) {
			return this;
		}
		else {
			return i2;
		}
	}

	//getter for turnOver
	bool getTurnOver() { return turnOver; }

	//setter for turnOver
	void setTurnOver(bool inputBool) {
		turnOver = inputBool;
	}

	void setBiteChance(int i) {
		biteChance = i;
	}
};

class Zombie : public Denizen {
public:
	//Zombie constructor
	Zombie(string name) : Denizen(name) { status = "Zombie"; biteChance = 0; }

	//test method
	void speak() { cout << "mngfdfgjfdfghdf brains......" << endl; }
};

class Alarmed : public Denizen {
public:
	Alarmed(string name) : Denizen(name) { status = "Alarmed"; biteChance = 5;}
	void speak() { cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAA!!!" << endl; }
	//convert to ignorant to alarmed
};

class Ignorant : public Denizen {
private:
	string workDistrict;
	string homeDistrict;
public:
	Ignorant(string name, string inputWork, string inputHome) : Denizen(name) { status = "Ignorant"; workDistrict = inputWork; homeDistrict = inputHome; }
	string getWork() { return workDistrict; }
	string getHome() { return homeDistrict; }
	void speak() { cout << "My word- what a fine, normal day in Simville!" << endl; }
};

#endif