#include <iostream>
#include <cstdlib>
#include <time.h>
#include <fstream>
#include <string>
using namespace std;

// Jonny Olswang
// CECS 325-01
// Prog 5 - Sorting Contest (threading with thread)
// 4/05/23
//
// I certify that this program is my own original work. I did not copy any part of this program from
// any other source. I further certify that I typed each and every line of code in this program.

/*********************************************
 * generate(count, min, max)
 *
 * This program runs from the command line and
 * takes in 3 additional arguments: count, min
 * and max. It then generates a count amount of 
 * numbers between the range of min and max and
 * outputs them line by line to file numbers.dat. 
 *********************************************/
int main(int argc, char *argv[]) 
{
	if(argc == 4 && stoi(argv[2]) <= stoi(argv[3])) //check if proper amount of inputs are taken in and min <= max
	{
		srand(time(0));				//set seed
		ofstream fileOut;			//open file output stream
		int count = stoi(argv[1]);	//convert count argument to an int with stoi
		int min = stoi(argv[2]);	//convert minimum argument to an int with stoi
		int max = stoi(argv[3]);	//convert maximum argument to an int with stoi

		fileOut.open("numbers.dat");	//set output file name

		for(int i = 0; i < argc; i++) //output args to terminal
		{
			cout << "arg[" << i << "]: " << argv[i] << endl;
		}

		for(int i = 0; i < count; i++)	
		{
			fileOut << (rand() % (max - min)) + min; //generate a random number between min and max and output to file
			if(i + 1 < count)
			{
				fileOut << endl;
			}
		}

		fileOut.close(); //close file stream
	}
	else //error message
	{
		cout << "generate:";
		for(int i = 1; i < argc; i++)
		{
			cout << " " << argv[i];
		}
		cout  << ": Invalid input" << endl
			  << "Example: generate 1000000 100000 999999" << endl;	
	}

	return 0;	
}
