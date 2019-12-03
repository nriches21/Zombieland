#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <iomanip>
#include <map>
#include <windows.h>

#include <ctype.h>
#include <typeinfo>
#include <stdlib.h>
#include <stdio.h>

#include "simville.h"
#include "denizen.h"
#include "district.h"
#undef max

using namespace std;

int main (){
	/*****************************
	* Enables colored console text API
	* Turns game text green
	//SetConsoleTextAttribute(hConsole, 7); //White text
	//SetConsoleTextAttribute(hConsole, 10); //Green text
	******************************/
	HANDLE hConsole; 
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	Simville simville;
	District d1("Soho");
	District d2("Uptown");
	District d3("Downtown");

	Denizen de1("Mr. Sir");
	Denizen de2("Ms. Ma'am");
	Denizen de3("Mrs. Missus");
	Denizen de4("Dr. Doc");
	Denizen de5("Professor Prof");
	Zombie z1("Bob");
	Alarmed a1("Dennis");

	simville.addDenizen2(&d1, de1);
	simville.addDenizen2(&d1, de2);
	simville.addDenizen2(&d2, de3);
	simville.addDenizen2(&d3, de4);
	simville.addDenizen2(&d3, de5);
	simville.addDenizen2(&d3, z1);
	simville.addDenizen2(&d3, a1);
	//simville.addDenizen3(&d3, &z1); //Not sure if we need v3 of everything?

	cout << "\nSimville methods output: " << endl;
	simville.printPop(&d1);
	simville.printPop(&d2);
	simville.printPop(&d3);
	int total = simville.districtPop(&d1) + simville.districtPop(&d2) + simville.districtPop(&d3);
	cout << endl << "Total population: " << total << endl;

	bool simOver = false;
	int Days = 0;

	// Prompts user for a number of days to simulate
	cout << "Number of Days to simulate: ";
	cin >> Days;

	// Incorrect input for days triggers user validation 
	if (cin.fail())
	{
		do {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "PLEASE ENTER A VALID POSITIVE INTEGER." << endl;
			cout << "Number of Days: ";
			cin >> Days;
		} while (cin.fail());		
	}
 
	// Main simulation loop
	do {
		for (int i = 0; i < Days * 4; i++) {
			SetConsoleTextAttribute(hConsole, 10); //Sets time text to green
			simville.hourTick();
			SetConsoleTextAttribute(hConsole, 7); //Sets text back to white
			de1.speak();
			z1.speak();
			a1.speak();
		}
		simOver = true;
	} while (simOver == false);

/*
	// Multimap example
	multimap<const char*, int> m;

	cout << endl;
	m.insert(pair<const char* const, int>("a", 1));
	m.insert(pair<const char* const, int>("c", 2));
	m.insert(pair<const char* const, int>("b", 3));
	m.insert(pair<const char* const, int>("b", 4));
	m.insert(pair<const char* const, int>("a", 5));
	m.insert(pair<const char* const, int>("b", 6));

	cout << "Number of elements with key a: " << m.count("a") << endl;
	cout << "Number of elements with key b: " << m.count("b") << endl;
	cout << "Number of elements with key c: " << m.count("c") << endl;

	cout << "Elements in m: " << endl;
	for (multimap<const char*, int>::iterator it = m.begin();
		it != m.end();
		++it)
		cout << "  [" << (*it).first << ", " << (*it).second << "]" << endl;
		*/
	return 0;
}