//
//  Util.h
//  SocialNetwork
//
//  Created by Bo Yan on 3/10/16.
//  Copyright © 2016 Bo_Yiting. All rights reserved.
//

#ifndef Util_h
#define Util_h

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


vector<string> split(string str, char delimiter);

map<string, int> generateProfileDataFromVectors(vector<string>& names, vector<string>& ages, vector<string>& occupations, bool append, int nodeNum);

void printInfoFromProfileData(int index, string profileDataPath);

void initializeNetwork(FriendshipGraph &g, BTree &btree, vector<string>& nameList, map<string, int> nameIndex, vector< vector<string> >& friendList);

void insertUser(FriendshipGraph &g, BTree &btree, vector<string>& nameList, map<string, int> nameIndex, vector<string>& friendNameVector);

// List all users' information with names between name1 and name2
void printInfoListInNameRange(BTree& bTree, string name1, string name2);

void loadDataFromFile(string filePath, vector<string>& nameList, vector<string>& ageList, vector<string>& occupationList, vector< vector<string> >& frindsList);


#endif /* Util_h */
