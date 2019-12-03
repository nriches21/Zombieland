#ifndef DENIZEN_H_
#define DENIZEN_H_

#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;


class Denizen {
<<<<<<< Updated upstream
private:
	string name;
=======
protected:
	string name; 
	string status; //ignorant / alarmed / zombie
	bool turnOver; //Already did an action this turn- if bit this turn, cannot also move and vice versa. Has to be reset after every time tick
	float biteChance; //probability of being bitten

>>>>>>> Stashed changes
public:
	//constructor for denizen that just initializes name
	Denizen(string name) : name(name) { turnOver = false; }

	//getters for the variables
	string getName() { return name; }
<<<<<<< Updated upstream
	void speak() { cout << "Hello, world!! My name is " << name << endl; }
=======
	string getStatus() { return status; }
	float getBiteChance() { return biteChance;  }

	//testing method
	virtual void speak() = 0;
	
>>>>>>> Stashed changes
};

class Zombie : public Denizen {
private:
	string status;
public:
<<<<<<< Updated upstream
	Zombie(string name) : Denizen (name), status("Zombie") {}
	string getStatus() { return status; }
=======
	//Zombie constructor
	Zombie(string name) : Denizen(name) { status = "Zombie"; biteChance = 0; }

	//test method
>>>>>>> Stashed changes
	void speak() { cout << "mngfdfgjfdfghdf brains......" << endl; }
};

class Alarmed : public Denizen {
private:
	string status;
public:
<<<<<<< Updated upstream
	Alarmed(string name) : Denizen(name), status("Alarmed") {}
	string getStatus() { return status; }
	void speak() { cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAA!!!" << endl; }
=======
	Alarmed(string name) : Denizen(name) { status = "Alarmed"; }

	//test method
	void speak() { cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAA!!!" << endl; }
	//convert to ignorant to alarmed
};

class Ignorant : public Denizen {
private:
	string workDistrict;
	string homeDistrict;
public:
	Ignorant(string name, string inputWork, string inputHome) : Denizen(name) { status = "Ignorant"; workDistrict = inputWork; homeDistrict = inputHome; }

	//test method
	void speak() { cout << "My word- what a fine, normal day in Simville!" << endl; }
>>>>>>> Stashed changes
};

#endif