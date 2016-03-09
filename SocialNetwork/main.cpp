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
#include <string>
#include <map>
#include <stdio.h>
#include <stdlib.h> 
#include <cstring>

#include "FriendshipGraph.h"
#include "BTree.h"

#define PROFILE_DATA_PATH "./ProfileData.txt"

using namespace std;

char* stringToCharArray(string &string);
map<string, int> generateProfileDataFromVectors(vector<string>& names, vector<string>& ages, vector<string>& occupations);


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
    try
    {
        FriendshipGraph g;
        while(true)
        {
            string str;
            cin >> str;
            if(cin.eof())
            {
                break;
            }
            if(str.compare("exit") == 0)
            {
                break;
            }
            else if(str.compare("addFriendship") == 0)
            {
                string name1;
                string name2;
                cin >> name1 >> name2;
                g.addFriendship(name1, name2);
            }
            else if(str.compare("doublehashing") == 0)
            {
                
            }
            else if(str.compare("insert") == 0)
            {
                string name;
                string friendString;
                string delimiter = ",";
                string* friendList;
                int pos = 0;
                int cnt = 0;
                
                cin >> name >> friendString;
                // check friends num
                int friendNum = (int)count(friendString.begin(), friendString.end(), ',') + 1;
                
                friendList = new string[friendNum]();
                //remove quotes from string
                friendString.erase(remove(friendString.begin(), friendString.end(), '"'), friendString.end());
                while ((pos = (int)friendString.find(delimiter)) != string::npos) {
                    friendList[cnt] = friendString.substr(0, pos);
                    friendString.erase(0, pos + delimiter.length());
                    cnt++;
                }
                friendList[cnt] = friendString;
//                cout << friendNum << endl;
                g.insert(name, friendList, friendNum, 1);
                cout << name << "," << friendString << endl;
//                cout << "here" << endl;
            }
            else if(str.compare("lookup") == 0)
            {
            }
            else if(str.compare("delete") == 0)
            {
            }
            else if(str.compare("print") == 0)
            {
                g.printAll();
            }
            else
            {
                cin.clear();
                cout << "Inputed string format was incorrect" << endl;
            }
        }
    }
    catch(exception& ex)
    {
        cerr << ex.what() << endl;
    }
    
	/*
	ifstream f;
	f.open("./Generated1.txt", ios::in);
	if(!f) cerr << "File not found" << endl;
	else
	{
		
		// while(!f.eof())
		// {
		// 	string line;
		// 	std::getline(f, line);
		// 	if(line.length() != 0){		// Ignore the last line...
		// 		vector<string> words = split(line, ',');				
		// 		//cout << words[0] << endl;

		// 	}	
			
		// }
		

		string line;
		while(std::getline(f, line))
		{
			vector<string> words = split(line, ',');
			// ... TO DO ...
			cout << words[0] << endl;
		}
	}
	*/


	// Test generateProfileDataFromVectors
	vector<string> nameList;
	vector<string> ageList;
	vector<string> occupationList;
	vector< vector<string> > frindsList;

	ifstream f;
	f.open("./Generated1.txt", ios::in);
	if(!f) cerr << "File not found" << endl;
	else
	{
		string line;
		while(std::getline(f, line))
		{
			vector<string> words = split(line, ',');
			
			nameList.push_back(words[0]);
			//ageList.push_back(atoi( words[1].c_str()));
			ageList.push_back(words[1]);
			occupationList.push_back(words[2]);
			vector<string> tempFrinds;
			for(int i=3; i<words.size(); i++)
			{
				tempFrinds.push_back(words[i]);
			}
			frindsList.push_back(tempFrinds);
		}
	}

	map<string, int> nameIndex = generateProfileDataFromVectors(nameList, ageList, occupationList);
	map<string, int>::iterator iter;
	for(iter=nameIndex.begin(); iter!=nameIndex.end(); iter++)
	{
		cout << iter->first << " (" << iter->second << ")" << endl;
	}


	/*
	int m = 5;
	int l = 3;

	// Testing on BTree class
	item item1 = {"Jerry1", 2};
	item item2 = {"Jerry2", 4};
	item item3 = {"Jerry3", 6};
	item item4 = {"Derry4", 8};
	item item5 = {"Jerry0", 10};
	BTree bTree(5, 3);
	bTree.insert(item1);	
	bTree.insert(item3);
	bTree.insert(item4);
	bTree.insert(item2);
	bTree.insert(item5);
	bTree.traverse(bTree.getRootNode());
	
	item item6 = {"Jerry00", 12};
	bTree.insert(item6);
	bTree.traverse(bTree.getRootNode());

	item item7 = {"Jerry23", 22};
	item item8 = {"Jerry34", 24};
	item item9 = {"Jerry25", 26};
	item item10 = {"Jerry27", 27};
	item item11 = {"Jerry01", 32};
	item item12 = {"Jerry", 35};
	bTree.insert(item7);
	bTree.insert(item8);
	bTree.insert(item9);
	bTree.insert(item10);
	bTree.insert(item11);
	bTree.insert(item12);
	bTree.traverse(bTree.getRootNode());
	// Testing for case 3b-b
	item item13 = {"Jerry02", 37};
	bTree.insert(item13);
	bTree.traverse(bTree.getRootNode());
	*/




	// Testing for createTreeFromMap() function
//	map<string,int> testMap;
//	testMap.insert ( pair<string,int>("kelly",100) );
//	testMap.insert ( pair<string,int>("kelly1",200) );
//	BTree theTree = BTree::createTreeFromMap(testMap, 5, 3);
//	theTree.traverse(theTree.getRootNode());
	
	return 0;
}



// Generate the profile Data file on the disk from three vectors
//	 return with a map of names along with corresponding entry(index) in the 
//   Profile Data file
// map<string, int> generateProfileDataFromVectors(vector<string>& names, vector<string>& ages, vector<string>& occupations)
// {
// 	// Double check the size of the 3 input files, making sure they are of same size
// 	if(names.size() != ages.size() || names.size() != occupations.size())
// 	{
// 		cout << "Error: size is different for the three input vectors.." << endl;
// 	}

// 	map<string, int> nameIndex;
	
// 	FILE* pFile;
// 	pFile = fopen (PROFILE_DATA_PATH, "w");
// 	for(int i=0; i<names.size(); i++)
// 	{
// 		cout << "2222" << endl;
// 		int tempIndexToInsert = 53*i;
// 		fseek(pFile, tempIndexToInsert, SEEK_SET);

// 		string tempString = names.at(i);
// 		cout << "22223333" << endl;
// 		char* tempName = stringToCharArray(tempString);
// 		cout << "3333" << endl;
// 		fputs(tempName, pFile);
// 		cout << tempName << endl;
// 		delete[] tempName;
// 		cout << "4444" << endl;
// 		nameIndex.insert(pair<string, int>(tempString, tempIndexToInsert));
		

// 		tempIndexToInsert = tempIndexToInsert+20;
// 		fseek(pFile, tempIndexToInsert, SEEK_SET);
// 		tempString = ages.at(i);
// 		//cout << tempString << endl;
// 		char* tempAge = stringToCharArray(tempString);
		
// 		fputs(tempAge, pFile);
// 		cout << tempAge << endl;
// 		delete[] tempAge;
			

// 		tempIndexToInsert = tempIndexToInsert+3;
// 		fseek(pFile, tempIndexToInsert, SEEK_SET);
// 		tempString=occupations[i];
// 		//cout << tempString << endl;
// 		char* tempOccupation = stringToCharArray(tempString);
// 		fputs(tempOccupation, pFile);
// 		cout << tempOccupation << endl;
// 		delete[] tempOccupation;		
// 	}
// 	fclose ( pFile );
// 	//cout << "3333" << endl;
// 	return nameIndex;
// }


// Generate the profile Data file on the disk from three vectors
//	 return with a map of names along with corresponding entry(index) in the 
//   Profile Data file
map<string, int> generateProfileDataFromVectors(vector<string>& names, vector<string>& ages, vector<string>& occupations)
{
	// Double check the size of the 3 input files, making sure they are of same size
	if(names.size() != ages.size() || names.size() != occupations.size())
	{
		cout << "Error: size is different for the three input vectors.." << endl;
	}

	map<string, int> nameIndex;
	
	// FILE* pFile;
	// pFile = fopen (PROFILE_DATA_PATH, "w");
	ofstream pFile;
	pFile.open(PROFILE_DATA_PATH);
	long initPos = pFile.tellp();
	for(int i=0; i<names.size(); i++)
	{
		int tempIndexToInsert = 53*i;
		pFile.seekp(initPos + tempIndexToInsert);
		string tempString = names.at(i);
		pFile << tempString;

		nameIndex.insert(pair<string, int>(tempString, tempIndexToInsert));
		
		
		tempIndexToInsert = tempIndexToInsert+20;
		pFile.seekp(initPos + tempIndexToInsert);
		tempString = ages.at(i);
		pFile << tempString;


		tempIndexToInsert = tempIndexToInsert+3;
		pFile.seekp(initPos + tempIndexToInsert);
		tempString=occupations.at(i);
		pFile << tempString;				
	}
	pFile.close();

	return nameIndex;
}



// Convert string to char array
// char* stringToCharArray(string& string)
// {
// 	//cout << "~~~~" << endl;
// 	//cout << string.c_str() << ": " << string.length()	 << endl;
// 	char* charArray = new char[string.length()];
// 	strncpy(charArray, string.c_str(), string.length());
// 	cout << string.length() << endl;
// 	charArray[string.length()] = '\0';
// 	cout << charArray << endl;
// 	return charArray;
// }
