#ifndef DENIZEN_H_// defined the classes here 
#define DENIZEN_H_

#include <iostream>
#include <string>

using std::cout;// defined namespace for std::cout
using std::endl;// defined namespace for std::endl
using std::string;// defined namespace for std::string

//class Denizen
class Denizen {
protected:
	string name;// stores the name of the denizen
	string status;// stores the status of the Denizens, whether they are zombies, ignorant, or alarmed
	bool turnOver; //Already did an action this turn- if bit this turn, cannot also move and vice versa. Has to be reset after every time tick
	int biteChance;// every ignorant and alarmed denizen has a bite chance, biteChance of alarmed > biteChance of ignorant

public:
	/*
		constructor Denizen
		sets the name and turnOver of the denizens
	*/
	Denizen(string name) : name(name) { turnOver = false; } 

	/*
		getName
		getter method for returning the name of the denizens
	*/
	string getName() { return name; }

	/*
		getStatus
		getter method for returning the status of the denizens
	*/
	string getStatus() { return status; }

	/*
		getBiteChance
		getter method for returning the biteChance of the denizen
	*/
	int getBiteChance() { return biteChance; }

	/*
		Abstract method void speak
		A denizen makes a noise when he/she is converted to a zombie
	*/
	virtual void speak() = 0;


	/*
		getTurnOver
		getter method for returning the turnOver of the zombies
	*/
	bool getTurnOver() { return turnOver; }

	/*
		setTurnOver
		setter method that sets the turnOver od the denizens 
	*/
	void setTurnOver(bool inputBool) {
		turnOver = inputBool;
	}

	/*
		setBiteChance
		setter method that sets the BiteChance of the denizens
	*/

	void setBiteChance(int i) {
		biteChance = i;
	}
};

// class Zombies
// inherted from the Denizen class 
class Zombie : public Denizen {
public:
	/*
		Constructor for the zombie class
		sets the status to Zombie and the bitChance to 0
	*/
	Zombie(string name) : Denizen(name) { status = "Zombie"; biteChance = 0; }

	/*
		abstract method from the Denizen class
	*/
	void speak() { cout << "mngfdfgjfdfghdf brains......" << endl; }
};

class Alarmed : public Denizen {
public:

	/*
		constructor for alarmed overloads the constructor of the base class Denizen
		overloads the constructor of the base class and sets the status = Alarmed and biteChance
	*/
	Alarmed(string name, int chance) : Denizen(name) { status = "Alarmed"; biteChance = chance; }

	/*
		abstract method from the Denizen class that gets overloaded
	*/
	void speak() { cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAA!!!" << endl; }
	//convert to ignorant to alarmed
};

class Ignorant : public Denizen {
private:
	string workDistrict;// stores the name of the district where ignorants work
	string homeDistrict;// stores the name of the district where ignorants live
public:

	/*
		constructor for Ignorant
		overloads the constructor of the base class and sets the status = Ignorant and work & home district
	*/
	Ignorant(string name, string inputWork, string inputHome) : Denizen(name) { status = "Ignorant"; workDistrict = inputWork; homeDistrict = inputHome; }
	
	//getter method returns the name of the work district 
	string getWork() { return workDistrict; }

	// getter method returns the name of the home district
	string getHome() { return homeDistrict; }

	/*
		abstract method from the Denizen class that gets overloaded
	*/
	void speak() { cout << "My word- what a fine, normal day in Simville!" << endl; }
};

#endif