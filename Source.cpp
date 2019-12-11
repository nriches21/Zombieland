#include <iostream>// included all the required header files
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

using namespace std;// used the namespace std

int main (){

	/*****************************
	* Enables colored console text API
	* Turns game text green
	//SetConsoleTextAttribute(hConsole, 7); //White text
	//SetConsoleTextAttribute(hConsole, 10); //Green text
	******************************/
	HANDLE hConsole; // this from Google's API used to format the terminal
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	string playAsk;// a string that holds the user command for playing the simulation
	bool playSim = true;// a boolean that decides the running of the simulation

	do {// a do while makes sure the simulation is executed until the boolean is true
		int Days = 0;// we start with 0 days
		std::cout << "Number of Days to simulate: ";// we ask the user to enter the number of days.
		std::cin >> Days;// 
		Days = Days * 4;// we multiply the day by 4 because we have 4 time clicks in a day

		if (std::cin.fail())// in case the user enters an invalid input, we ask them to enter the integer again
		{
			do {
				std::cin.clear();
				std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
				std::cout << "PLEASE ENTER A VALID POSITIVE INTEGER." << std::endl;
				std::cout << "Number of Days: ";
				std::cin >> Days;
			} while (std::cin.fail());// the user keeps entering until they enter a valid value
		}

		bool verbose = false;// we initialize a verbose as false
		string verboseAsk;// stores user input
		std::cout << "Verbose output for district populations? [Y/N] ";
		std::cin >> verboseAsk;// we ask the user if they want to execute the verbose or not

		if (verboseAsk == "Y" || verboseAsk == "y") {
			verbose = true;
		}// if verbose is true, we print all the informtion to the terminal

        #pragma region initialization

		Simville simville;// an object to the Simville class

		District upt("Uptown");// we declare the six districts as objects to the district class
		District dow("Downtown");
		District uni("The University");
		District soh("Soho");
		District med("Medical Hill");
		District doc("The Docks");

		simville.addDist(&upt);// and then we add those districts to Simville using the addDist method
		simville.addDist(&dow);
		simville.addDist(&uni);
		simville.addDist(&soh);
		simville.addDist(&med);
		simville.addDist(&doc);

		//everything has a north south east west, all directions lead to a different district.
		med.addConnection('n', &upt);// we assign directions to the districts according to map in the design spec
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
		soh.addConnection('w', &dow);// we do that for all the districts 


		simville.populateDistrict();// we print the popluation info for Simville

		simville.createAlarmed (&dow, 1);// we create the random number of zombies and alarmed denizens
		simville.createZombie(&dow, 1); 

        #pragma endregion


		#pragma region mainLoop

		string contd;// a string variable that stores user's input
		bool c = true;// a boolean that is initialazed to true

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
			std::cin >> contd;// if the user wants to continue simulation in the future. 
			if (contd != "Y" && contd != "y") {
				c == false;
			}
			std::cout << "Number of Days to simulate: ";
			std::cin >> Days;// user enters the number of days agaian 
			Days = Days * 4;// and the four time clicks are taken into condideration once again
		} while (c == true);// end of the loop

		std::cout << std::endl << "Play simulation again? [Y/N] ";
		std::cin >> playAsk;
		if (playAsk != "Y" && playAsk != "y") {
			playSim = false;
		}// and we keep asking until the user says no

	} while (playSim == true);// end of the biggest do-while loop
			
	#pragma endregion// end of the main function

	return 0;
}