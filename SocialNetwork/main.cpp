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
#include <limits>


#include "FriendshipGraph.h"
#include "BTree.h"
#include "Util.h"

#define PROFILE_DATA_PATH "./ProfileData.txt"
#define INPUT_DATA_PATH "./Generated1.txt"

using namespace std;

// char* stringToCharArray(string &string);
// map<string, int> generateProfileDataFromVectors(vector<string>& names, vector<string>& ages, vector<string>& occupations);
// void printInfoFromProfileData(int index, string profileDataPath);
// void printInfoListInNameRange(BTree& bTree, string name1, string name2);


// vector<string> split(string str, char delimiter)
// {
// 	vector<string> internal;
// 	stringstream ss(str);       // turn the string into a stream.
// 	string tok;

// 	while(getline(ss, tok, delimiter))
// 	{
// 		internal.push_back(tok);
// 	}

// 	return internal;
// }

int main()
{
	// vector<string> nameList;
	// vector<string> ageList;
	// vector<string> occupationList;
	// vector< vector<string> > frindsList;

	// ifstream f;
	// f.open("./Generated4.txt", ios::in);
	// if(!f) cerr << "File not found" << endl;
	// else
	// {
	// 	string line;
	// 	while(std::getline(f, line))
	// 	{
	// 		vector<string> words = split(line, ',');
			
	// 		nameList.push_back(words[0]);
	// 		//ageList.push_back(atoi( words[1].c_str()));
	// 		ageList.push_back(words[1]);
	// 		occupationList.push_back(words[2]);
	// 		vector<string> tempFrinds;
	// 		for(int i=3; i<words.size(); i++)
	// 		{
	// 			tempFrinds.push_back(words[i]);
	// 		}
	// 		frindsList.push_back(tempFrinds);
	// 	}
	// }
	// f.close();

	// map<string, int> nameIndex = generateProfileDataFromVectors(nameList, ageList, occupationList);
	// map<string, int>::iterator iter;
	// for(iter=nameIndex.begin(); iter!=nameIndex.end(); iter++)
	// {
	// 	cout << iter->first << " (" << iter->second << ")" << endl;
	// }

	// // Create a B-Tree from the nameIndex
	// BTree theTree = BTree::createTreeFromMap(nameIndex, 5, 3);
	// theTree.traverse(theTree.getRootNode());

	// // Range search query
	// printInfoListInNameRange(theTree, "J", "P");
	// cout << "done" << endl;



	//printInfoFromProfileData(1325, PROFILE_DATA_PATH);



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



	/*
	// Testing for createTreeFromMap() function
	map<string,int> testMap;
	testMap.insert ( pair<string,int>("kelly",100) );
	testMap.insert ( pair<string,int>("kelly1",200) );
	BTree theTree = BTree::createTreeFromMap(testMap, 5, 3);
	theTree.traverse(theTree.getRootNode());
	*/

	int bTree_m = 5;
	int bTree_l = 3;

	string manual = string("Social Media Explorer (Beta 1.0)\n\n")
				+ "load\n"
				+ "\tload data (and initialze the database)\n"
				+ "query -a\n"
				+ "\tlist all users' infomation\n"
				+ "query -b <name1> <name2>\n"
				+ "\tlist users' infomation whose names are between name1 and name2\n"
				+ "query -f <name>\n"
				+ "\tlist friends' infomation of user name\n"
				+ "query -u <name>\n"
				+ "\tlist a user's infomation\n"				
				+ "insert -f <name1> <name2>\n"
				+ "\tadd a friendship relationship between two existing users name1 and name2\n"
				+ "insert -u <name>,<age>,<occupation>\n"
				+ "\tadd a user with name, age, and occupation\n"
				+ "manual\n"
				+ "\tshow the manual\n"
				+ "exit\n"
				+ "\tleave the program"
				+ "\n";

	vector<string> nameList;
	vector<string> ageList;
	vector<string> occupationList;
	vector< vector<string> > friendsList;


	//Command line interface
	try
	{
		cout << manual << endl;
		// Initialize...
		BTree theTree(bTree_m, bTree_l);
		FriendshipGraph g;

		while(true)
		{
			cout << ">>> ";
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
			else if(str.compare("manual") == 0)
			{
				cout << manual << endl;
			}
			else if(str.compare("load") == 0)
			{
				if(theTree.getRootNode() == NULL)
				{
					loadDataFromFile(INPUT_DATA_PATH, nameList, ageList, occupationList, friendsList);
					map<string, int> nameIndex = generateProfileDataFromVectors(nameList, ageList, occupationList, false, 0);
					initializeNetwork(g, theTree, nameList, nameIndex, friendsList);
					cout << "Data in " << INPUT_DATA_PATH << " has been loaded\n" << endl;
				}
				else
				{
					cout << "Data has already been loaded" << endl;
				}
			}
			else if(str.compare("insert") == 0)
			{
				string option;
				if(theTree.getRootNode() == NULL)	// empty database
				{
					cout << "The database is empty. Please load the data first." << endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');	// skip the line
				}
				else
				{
					cin >> option;
					if(option.compare("-u") == 0)	// insert a user
					{
						vector<string> name;
		                vector<string> age;
		                vector<string> occupation;
		                vector<string> friends; // friendList of one user
		                // get the input
		                string input;
		                getline(cin, input);
		                while(input[0] == ' ')	// Get rid of space at the beginning
		                {
		                	input = input.substr(1);
		                }

		                vector<string> words = split(input, ',');
		                if(words[0].length() > 20 
		                	|| words[1].length() > 3
		                	|| words[2].length() > 30)
			            {
			                cout << "Error: The length of the input is more than allowed" << endl;
			            }
			            else
			            {
			            	name.push_back(words[0]);
			            	age.push_back(words[1]);
			            	occupation.push_back(words[2]);
			            	friends.clear();

				            map<string, int> nameIndex 
				            		= generateProfileDataFromVectors(name, age, occupation, true, g.getNodeNum()); //append
				            insertUser(g, theTree, name, nameIndex, friends);
				            //cout << "User " << name[0] << " has been inserted" << endl;		    
			            }
			            cout << endl;
					}
					else if(option.compare("-f") == 0)	// Insert a friendship
					{
						string name1;
						string name2;
						cin >> name1 >> name2;
						g.addFriendship(name1, name2);
						cout << endl;
					}
					else
					{
						cout << "Wrong input" << endl;
					}
				}				
			}
			else if(str.compare("query") == 0)
			{
				string option;
				if(theTree.getRootNode() == NULL)	// empty database
				{
					cout << "The database is empty. Please load the data first." << endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');	// skip the line
				}
				else
				{
					cin >> option;
					if(option.compare("-b") == 0)	// List users btw name1 and name2
					{
						string name1;
						string name2;
						cin >> name1 >> name2;
						printInfoListInNameRange(theTree, name1, name2);
						cout << endl;
					}
					else if(option.compare("-a") == 0)	// List all
					{
						printInfoListInNameRange(theTree, "0", "{");
						cout << endl;	
					}
					else if(option.compare("-u") == 0)	// List a user's info
					{
						string name;
						cin >> name;
						printInfoListInNameRange(theTree, name, name);
						cout << endl;	
					}
					else if(option.compare("-f") == 0)	// List friendship of a user
					{
						string name;
						cin >> name;
						g.listFriendsInfo(name);
						cout << endl;						
					}
				}				
			}
			else if(str.compare("print") == 0)
			{
				
			}
			else
			{
				cin.clear();
				cout << "Inputed string format was incorrect" << endl;
				cin.ignore(numeric_limits<streamsize>::max(), '\n');	// skip the line
			}
		}
	}
	catch(exception& ex)
	{
		cerr << ex.what() << endl;
	}




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
// map<string, int> generateProfileDataFromVectors(vector<string>& names, vector<string>& ages, vector<string>& occupations)
// {
// 	// Double check the size of the 3 input files, making sure they are of same size
// 	if(names.size() != ages.size() || names.size() != occupations.size())
// 	{
// 		cout << "Error: size is different for the three input vectors.." << endl;
// 	}

// 	map<string, int> nameIndex;
	
// 	// FILE* pFile;
// 	// pFile = fopen (PROFILE_DATA_PATH, "w");
// 	ofstream pFile;
// 	pFile.open(PROFILE_DATA_PATH);
// 	long initPos = pFile.tellp();
// 	for(int i=0; i<names.size(); i++)
// 	{
// 		int tempIndexToInsert = 53*i;
// 		pFile.seekp(initPos + tempIndexToInsert);
// 		string tempString = names.at(i);
// 		pFile << tempString;

// 		nameIndex.insert(pair<string, int>(tempString, tempIndexToInsert));
		
		
// 		tempIndexToInsert = tempIndexToInsert+20;
// 		pFile.seekp(initPos + tempIndexToInsert);
// 		tempString = ages.at(i);
// 		pFile << tempString;


// 		tempIndexToInsert = tempIndexToInsert+3;
// 		pFile.seekp(initPos + tempIndexToInsert);
// 		tempString=occupations.at(i);
// 		pFile << tempString;				
// 	}
// 	pFile.close();

// 	return nameIndex;
// }


// Print the name, age, and occupation of a person who is record at the index of
//	 index in the file at profileDataPath
// void printInfoFromProfileData(int index, string profileDataPath)
// {
// 	ifstream f;
// 	f.open(profileDataPath.c_str(), ios::in);
// 	if(!f) cerr << "Profile Data file not found" << endl;
// 	else
// 	{
// 		string line;
// 		while(std::getline(f, line))
// 		{
// 			f.seekg (0, ios_base::end);
//     		int length = f.tellg();
//     		if(index >= length)
//     		{
//     			cout << "Error: The input index is out of the size of the file" << endl;
//     		}
//     		else
//     		{
// 				f.seekg(index, ios_base::beg);
// 				//int startIndex = f.tellg();

// 				char* buffer = new char[20];
// 				f.read(buffer, 20);
// 				string tempName(buffer);
// 				delete[] buffer; 
				
// 				f.seekg(index+20);
// 				buffer = new char[3];
// 				f.read(buffer, 3);
// 				string tempAge(buffer);
// 				delete[] buffer; 

// 				f.seekg(index+23);
// 				buffer = new char[30];
// 				f.read(buffer, 30);
// 				string tempOccupation(buffer);

// 				f.close();
// 				if(tempName.length() <= 0)
// 				{
// 					cout << "Error: No character at the index of the file" << endl;
// 				}
// 				else
// 				{
// 					cout << tempName << "\t" << tempAge << "\t" << tempOccupation << endl;
// 				}
				

// 				delete[] buffer;
//     		}			
// 		}
// 	}
// }


// List all users' information with names between name1 and name2
// void printInfoListInNameRange(BTree& bTree, string name1, string name2)
// {
// 	if(bTree.getRootNode() != NULL)
// 	{
// 		cout << "The info of users with names between \"" << name1 << "\" and \"" << name2 << "\":" << endl;
// 		map<string, int> nameIndexList = bTree.rangeSearchQuery(name1, name2, bTree.getRootNode());
// 		map<string, int>::iterator iterQ = nameIndexList.begin();
// 		for(int i=0; i<nameIndexList.size(); i++, iterQ++)
// 		{
// 			printInfoFromProfileData(iterQ->second, PROFILE_DATA_PATH);
// 			//cout << iterQ->first << " (" << iterQ->second << ")" << endl;
// 		}
// 		cout << endl;
// 	}
// }


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


