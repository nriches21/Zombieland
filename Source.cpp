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



	string playAsk;
	bool playSim = true;

	do {
		int Days = 0;
		std::cout << "Number of Days to simulate: ";
		std::cin >> Days;
		Days = Days * 4;

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
		std::cout << "Verbose output for district populations? [Y/N] ";
		std::cin >> verboseAsk;

		if (verboseAsk == "Y" || verboseAsk == "y") {
			verbose = true;
		}

        #pragma region initialization

		Simville simville;

		District upt("Uptown");
		District dow("Downtown");
		District uni("The University");
		District soh("Soho");
		District med("Medical Hill");
		District doc("The Docks");

		simville.addDist(&upt);
		simville.addDist(&dow);
		simville.addDist(&uni);
		simville.addDist(&soh);
		simville.addDist(&med);
		simville.addDist(&doc);

		//Ignorant p1("Person one");//Testing
		//Ignorant p2("Person two");//Testing
		//uni.addDenizen(&p1);//Testing
		//uni.addDenizen(&p2);//Testing

		Ignorant s1("Sort one", "Downtown", "The University");//Testing
		Ignorant s2("Sort two", "Downtown", "The University");//Testing
		s1.setBiteChance(30);
		s2.setBiteChance(80);
		uni.addDenizen(&s1);//Testing
		uni.addDenizen(&s2);//Testing
		s1.setBiteChance(50); //Testing
		s2.setBiteChance(100); //Testing
		simville.createZombie(&uni, 1); //Testing
		//simville.createAlarmed (&uni, 1);
		simville.populateDistrict();

        #pragma endregion

		// Main simulation loop
			for (int i = 0; i < Days; i++) {

				// Displays day and time 
				SetConsoleTextAttribute(hConsole, 2); //text green
				simville.hourTick();
				SetConsoleTextAttribute(hConsole, 7); //text back white

				// If true, displays population details per district
				simville.districtPopulation(verbose);

				// Displays total population in all of Simville
				simville.showTotal();
				std::cout << std::endl;
			}

			std::cout << std::endl << "Play simulation again? [Y/N] ";
			std::cin >> playAsk;
			if (playAsk != "Y" && playAsk != "y") {
			playSim = false;
			} //Need to reset variables from simville and districts, or it'll just continue and the zombies will carry over from last time

	} while (playSim == true);
	

	return 0;
}