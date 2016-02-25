/*
 * Main.cpp
 *
 *  Created on: Feb 11, 2016
 *      Author: omid
 */

#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

vector<string> split(string str, char delimiter)
{
	vector<string> internal;
	stringstream ss(str);       // turn the string into a stream.
	string tok;

	while(getline(ss, tok, delimiter))
	{
		internal.push_back(tok);
	}

	return internal;
}

int main()
{
	ifstream f;
	f.open("/home/omid55/PythonProjects/RecordGenerator/Generated1.txt", ios::in);
	if(!f) cerr << "File not found" << endl;
	else
	{
		while(!f.eof())
		{
			string line;
			std::getline(f, line);
			vector<string> words = split(line, ',');
			// ... TO DO
		}
	}
	return 0;
}
