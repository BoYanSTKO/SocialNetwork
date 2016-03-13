//
//  Util.cpp
//  SocialNetwork
//
//  Created by Bo Yan on 3/10/16.
//  Copyright © 2016 Bo_Yiting. All rights reserved.
//

#include "Util.h"

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


// Generate the profile Data file on the disk from three vectors
//	 return with a map of names along with corresponding entry(index) in the
//   Profile Data file
map<string, int> generateProfileDataFromVectors(vector<string>& names, vector<string>& ages, vector<string>& occupations, bool append, int nodeNum)
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
    if (append) {
        pFile.open(PROFILE_DATA_PATH, ios_base::app);  // append to file
        pFile.seekp(53*nodeNum);  // find the correct position to insert
        cout << pFile.tellp() << endl;
        long initPos = pFile.tellp();
        for (int i = 0; i < names.size(); i++) {
            int tempIndexToInsert = 53*i;
            pFile.seekp(initPos + tempIndexToInsert);
            string tempString = names.at(i);
            pFile << tempString;
            
            nameIndex.insert(pair<string, int>(tempString, initPos));
            
            tempIndexToInsert = tempIndexToInsert + 20;
            pFile.seekp(initPos + tempIndexToInsert);
            tempString = ages.at(i);
            pFile << tempString;
            
            
            tempIndexToInsert = tempIndexToInsert+3;
            pFile.seekp(initPos + tempIndexToInsert);
            tempString=occupations.at(i);
            pFile << tempString;
            
        }
    }
    else {
        pFile.open(PROFILE_DATA_PATH);  // overwrite
        long initPos = pFile.tellp();
        for(int i=0; i<names.size(); i++)
        {
            //        cout << "here" << endl;
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
//            cout << initPos << endl;
        }
    }
    
    pFile.close();
    
    return nameIndex;
}


// Print the name, age, and occupation of a person who is record at the index of
//	 index in the file at profileDataPath
void printInfoFromProfileData(int index, string profileDataPath)
{
    ifstream f;
    f.open(profileDataPath.c_str(), ios::in);
    if(!f) cerr << "Profile Data file not found" << endl;
    else
    {
        string line;
        while(std::getline(f, line))
        {
            f.seekg (0, ios_base::end);
            int length = f.tellg();
            if(index >= length)
            {
                cout << "Error: The input index is out of the size of the file" << endl;
            }
            else
            {
                f.seekg(index, ios_base::beg);
                //int startIndex = f.tellg();
                
                char* buffer = new char[20];
                f.read(buffer, 20);
                string tempName(buffer);
                delete [] buffer;
                
                
                f.seekg(index+20);
                buffer = new char[3];
                f.read(buffer, 3);
                string tempAge(buffer);
                delete [] buffer;
                f.seekg(index+23);
                buffer = new char[30];
                f.read(buffer, 30);
                string tempOccupation(buffer);
                
                f.close();
                if(tempName.length() <= 0)
                {
                    cout << "Error: No character at the index of the file" << endl;
                }
                else
                {
                    cout << tempName << "\t" << tempAge << "\t" << tempOccupation << endl;
                }
                
                
                delete[] buffer;
                
            }			
        }
    }
}

// insert users from input
void insertUser(FriendshipGraph &g, BTree &btree, vector<string>& nameList, map<string, int> nameIndex, vector<string>& friendNameVector) {
    // insert into graph
    string* friendNameList = new string[friendNameVector.size()];
    for (int i = 0; i < friendNameVector.size(); i++) {
        friendNameList[i] = friendNameVector.at(i);
    }
    g.insert(nameList.at(0), friendNameList, (int) friendNameVector.size(), nameIndex[nameList.at(0)]);
    delete [] friendNameList;
    friendNameList = NULL;
    
    // insert into btree
    for (int i = 0; i < nameIndex.size(); i++) {
        Item userRecord = {nameList.at(i), nameIndex[nameList.at(i)]};
        btree.insert(userRecord);
    }
}

// initialize the social network with data from files
void initializeNetwork(FriendshipGraph &g, BTree &btree, vector<string>& nameList, map<string, int> nameIndex, vector< vector<string> >& friendList) {
    // insert into graph
    for (int i=0; i < nameIndex.size(); i++) {
        string* friendNameList = new string[friendList.at(i).size()];
        for (int j = 0; j < friendList.at(i).size(); j++) {
            friendNameList[j] = friendList.at(i).at(j);
        }
        g.insert(nameList.at(i), friendNameList, (int) friendList.at(i).size(), nameIndex[nameList.at(i)]);
        
        delete [] friendNameList;
        friendNameList = NULL;
    }
    
    // insert into btree
    for (int i = 0; i < nameIndex.size(); i++) {
        Item userRecord = {nameList.at(i), nameIndex[nameList.at(i)]};
        btree.insert(userRecord);
    }

    
}
