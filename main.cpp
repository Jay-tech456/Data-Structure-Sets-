#include "SetT.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;


int main()
{
	string str;
	char set;
	int number;
	ifstream infile;
	SetT<int> a;
	SetT<int> b;

	infile.open("testDriver.txt");
	if (!infile)
	{
		cout << "\nThere is an error of opening up the file " << endl;
		return 0;
	}

	while (str != "QUIT" || !infile.eof())			// A cannot equal quit or not end of file 
	{
		infile >> set;
		infile >> str;

		if (str == "Add")						//else if A is equal to append, add on to the list 
		{
			infile >> number;
			if (set == 'a')
			{
				a + number;
			}
			else
				b + number;
		}
		else if (str == "Remove")				//else if A is equal to GetLength, get the length
		{
			infile >> number;
			if (set == 'a')
			{
				a - number;
			}
			else
			{
				b - number;
			}
		}
		else if (str == "Union")				//else if A == GetIndex, get the index of the file
		{
			a + b;
		}
		else if (str == "Interception")
		{
			a* b;
		}
		else if (str == "Difference")
		{
			if (set == 'a')
			{
				a - b;
			}
			else
				b - a;
		}
		else
			break;
	}
	infile.close();

	return 0;
}
