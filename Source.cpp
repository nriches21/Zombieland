#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <iomanip>
#include <map>
#include <windows.h>

//#include <ctype.h>
//#include <typeinfo>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>

#include "simville.h"
#include "denizen.h"
#include "district.h"
#undef max

using std::cout;
using std::cin;
using std::endl;

int main (){

	/*****************************
	* Enables colored console text API
	* Turns sim text different colors 
	******************************/
	HANDLE hConsole; 
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	string p;
	bool playSim = true;

	string c;
	bool contd = true;

	do {

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


		/*//everything has a north south east west, if there is nothing in that direction the map holds a pointer to itself
		med.addConnection('n', &upt);
		med.addConnection('e', &dow);
		med.addConnection('s', &doc);
		med.addConnection('w', &med); //there is nothing west, so if they want to move west they just stay in the same place

		doc.addConnection('n', &med);
		doc.addConnection('e', &dow);
		doc.addConnection('s', &doc);
		doc.addConnection('w', &doc);

		upt.addConnection('n', &uni);
		upt.addConnection('e', &soh);
		upt.addConnection('s', &dow);
		upt.addConnection('w', &med);

		dow.addConnection('n', &upt);
		dow.addConnection('e', &soh);
		dow.addConnection('s', &doc);
		dow.addConnection('w', &med);

		uni.addConnection('n', &uni);
		uni.addConnection('e', &upt);
		uni.addConnection('s', &uni);
		uni.addConnection('w', &uni);

		soh.addConnection('n', &upt);
		soh.addConnection('e', &soh);
		soh.addConnection('s', &uni);
		soh.addConnection('w', &dow);
		*/

		simville.populateDistrict();

		simville.createAlarmed (&dow, 1); //Testing
		simville.createZombie(&dow, 1); //Testing

        #pragma endregion


		#pragma region mainLoop

		do {
			int Days = 0;
			cout << " Number of Days to simulate: ";
			cin >> Days;
			Days = Days * 4;

			if (cin.fail())
			{
				do {
					cin.clear();
					cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					cout << " PLEASE ENTER A VALID POSITIVE INTEGER." << endl;
					cout << " Number of Days: ";
					cin >> Days;
				} while (std::cin.fail());
			}

			bool verbose = false;
			string v;
			cout << " Verbose output for district populations? [Y/N] ";
			cin >> v;

			if (v == "Y" || v == "y") {
				verbose = true;
			}

			for (int i = 0; i < Days; i++) {

				// Displays day and time 
				SetConsoleTextAttribute(hConsole, 2); //text green
				simville.hourTick();
				SetConsoleTextAttribute(hConsole, 7); //text back white

				// If verbose, displays denizen names and statuses for each district
				simville.districtPopulation(verbose);

				// Displays total population in Simville
				simville.simvilleSum();
				cout << endl;
			}

			

			cout << endl << " Continue simulation? [Y/N] ";
			cin >> c;
			if (c != "Y" && c != "y") {
				contd = false;
			}
		} while (contd == true);

		cout << endl << " New simulation? [Y/N] ";
		cin >> p;
		if (p != "Y" && p != "y") {
			playSim = false;
		}

	} while (playSim == true);
			
	#pragma endregion

	return 0;
}