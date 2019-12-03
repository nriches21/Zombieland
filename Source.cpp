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
#include <fstream>

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

#pragma region initialization

	Simville simville;

	simville.addDist(new District("Uptown"));
	simville.addDist(new District("Downtown"));
	simville.addDist(new District("The University"));
	simville.addDist(new District("Soho"));	
	simville.addDist(new District("Medical Hill"));
	simville.addDist(new District("The Docks"));

	simville.populateDistrict();

#pragma endregion

	int Days = 0;
	std::cout << "Number of Days to simulate: ";
	std::cin >> Days;
	Days = Days * 4;

	// Incorrect input for days triggers user validation 
	if (std::cin.fail())
	{
		do {
			std::cin.clear();
			std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
			std::cout << "PLEASE ENTER A VALID POSITIVE INTEGER." << std::endl;
			std::cout << "Number of Days: ";
			std::cin >> Days;
		} while (std::cin.fail());		
	}

	bool verbose = false;
	string verboseAsk;
	std::cout << "Verbose output for district populations? [Y] ";
	std::cin >> verboseAsk;

	if (verboseAsk == "Y" || verboseAsk == "y") {
		verbose = true;
	}
 
	bool simOver = false;

	// Main simulation loop
	do { 
		for (int i = 0; i < Days; i++) {
			// Displays day and time 
			SetConsoleTextAttribute(hConsole, 2); //Sets time text to green
			simville.hourTick();
			SetConsoleTextAttribute(hConsole, 7); //Sets text back to white
			
			// If true, displays population details per district
			simville.districtPopulation(verbose);

			// Displays total population in all of Simville
			simville.showTotal();
			std::cout << std::endl;
		}
		simOver = true;
	} while (simOver == false);

	return 0;
}