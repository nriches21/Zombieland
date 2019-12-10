#include <iostream>// included the 
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

		//everything has a north south east west, all directions lead to a different district.
		med.addConnection('n', &upt);
		med.addConnection('e', &dow);
		med.addConnection('s', &doc);
		med.addConnection('w', &doc);

		doc.addConnection('n', &med);
		doc.addConnection('e', &dow);
		doc.addConnection('s', &dow);
		doc.addConnection('w', &dow);

		upt.addConnection('n', &uni);
		upt.addConnection('e', &soh);
		upt.addConnection('s', &dow);
		upt.addConnection('w', &med);

		dow.addConnection('n', &upt);
		dow.addConnection('e', &soh);
		dow.addConnection('s', &doc);
		dow.addConnection('w', &med);

		uni.addConnection('n', &upt);
		uni.addConnection('e', &upt);
		uni.addConnection('s', &upt);
		uni.addConnection('w', &upt);

		soh.addConnection('n', &upt);
		soh.addConnection('e', &uni);
		soh.addConnection('s', &uni);
		soh.addConnection('w', &dow);


		simville.populateDistrict();

		simville.createAlarmed (&dow, 1); //Testing
		simville.createZombie(&dow, 1); //Testing

        #pragma endregion


		#pragma region mainLoop

		string contd;
		bool c = true;

		do {
			for (int i = 0; i < Days; i++) {

				// Displays day and time 
				SetConsoleTextAttribute(hConsole, 2); //text green
				simville.hourTick();
				SetConsoleTextAttribute(hConsole, 7); //text back white

				// If true, displays population details per district
				simville.districtPopulation(verbose); 

				// Displays total population in all of Simville
				simville.simvilleSum();
				std::cout << std::endl;
			}
			std::cout << std::endl << "Continue simulation? [Y/N] ";
			std::cin >> contd;
			if (contd != "Y" && contd != "y") {
				c == false;
			}
			std::cout << "Number of Days to simulate: ";
			std::cin >> Days;
			Days = Days * 4;
		} while (c == true);

		std::cout << std::endl << "Play simulation again? [Y/N] ";
		std::cin >> playAsk;
		if (playAsk != "Y" && playAsk != "y") {
			playSim = false;
		}

	} while (playSim == true);
			
	#pragma endregion

	return 0;
}