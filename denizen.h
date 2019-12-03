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

public:
	Denizen(string name) : name(name) {}
	string getName() { return name; }
	string getStatus() { return status; }
	virtual void speak() = 0;
	//Needs virutal void Move()
	//Needs virtual void SetByChance()
};

class Zombie : public Denizen {
public:
	Zombie(string name) : Denizen(name) { status = "Zombie"; }
	void speak() { cout << "mngfdfgjfdfghdf brains......" << endl; }
	//convert to ignorant to zombie
};

class Alarmed : public Denizen {
public:
	Alarmed(string name) : Denizen(name) { status = "Alarmed"; }
	void speak() { cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAA!!!" << endl; }
	//convert to ignorant to alarmed
};

class Ignorant : public Denizen {
//private:
	//Work district name
	//Home district name
public:
	Ignorant(string name) : Denizen(name) { status = "Ignorant"; }
	void speak() { cout << "My word- what a fine, normal day in Simville!" << endl; }
};

#endif