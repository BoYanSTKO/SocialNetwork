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
        pFile.open(PROFILE_DATA_PATH, std::ofstream::out | std::ofstream::in | std::ofstream::binary);  // append to file
        pFile.seekp(53*(nodeNum));  // find the correct position to insert
        //cout << pFile.tellp() << endl;
        //cout << 53*nodeNum << endl;
        long initPos = pFile.tellp();
        //cout << initPos << endl;
        for (int i = 0; i < names.size(); i++) {
            int tempIndexToInsert = 53*i;
            pFile.seekp(initPos + tempIndexToInsert);
            string tempString = names.at(i);
            //cout << tempString<< endl;
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
            
            pFile.seekp(initPos+53*(i+1));
            tempString = "0";
            pFile << tempString;
            //cout << tempString << endl;
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
            //cout << tempString << endl;
            pFile << tempString.c_str();
            
            nameIndex.insert(pair<string, int>(tempString, tempIndexToInsert));
            
            
            tempIndexToInsert = tempIndexToInsert+20;
            pFile.seekp(initPos + tempIndexToInsert);
            tempString = ages.at(i);
            pFile << tempString;
            
            
            tempIndexToInsert = tempIndexToInsert+3;
            pFile.seekp(initPos + tempIndexToInsert);
            tempString=occupations.at(i);
            pFile << tempString;

            pFile.seekp(53*(i+1));
            tempString = "0";
            pFile << tempString;
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
            //cout << length << endl;
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
                buffer = new char[30+1];
                f.read(buffer, 30);
                string tempOccupation(buffer);
                //cout << tempOccupation << tempOccupation.length() << endl;
                if((tempOccupation.at(tempOccupation.length()-1) < 'A' || tempOccupation.at(tempOccupation.length()-1) > 'Z')
                    && (tempOccupation.at(tempOccupation.length()-1) < 'a' || tempOccupation.at(tempOccupation.length()-1) > 'z'))
                {
                    //cout << tempOccupation << tempOccupation.at(tempOccupation.length()-1) << endl;
                    tempOccupation = tempOccupation.substr(0, tempOccupation.length()-1);
                }
                //cout << tempOccupation << tempOccupation.length() << endl;
                
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
        //cout << nameList.at(i) << nameIndex[nameList.at(i)] << endl;
        btree.insert(userRecord);
    }
}


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

// List all users' information with names between name1 and name2
void printInfoListInNameRange(BTree& bTree, string name1, string name2)
{   
    if(bTree.getRootNode() != NULL)
    {
        if(name1 != "0" && name2 != "{")
        {
            cout << "The info of users with names between \"" << name1 << "\" and \"" << name2 << "\":" << endl;
        }
        map<string, int> nameIndexList = bTree.rangeSearchQuery(name1, name2, bTree.getRootNode());
        map<string, int>::iterator iterQ = nameIndexList.begin();
        if(nameIndexList.size() == 0 && name1 == name2)
        {
            cout << name1 << " is not found in the database" << endl;
        }
        else if(nameIndexList.size() == 0 && name1 != name2)
        {
            cout << "No user is found between " << name1 << " and " << name2 << endl;
        }
        for(int i=0; i<nameIndexList.size(); i++, iterQ++)
        {
            printInfoFromProfileData(iterQ->second, PROFILE_DATA_PATH);
            //cout << iterQ->first << " (" << iterQ->second << ")" << endl;
        }
    }
}


// Load the data form file at filePath and put them into the vectors
//   Also check the length of the input
void loadDataFromFile(string filePath, vector<string>& nameList, vector<string>& ageList, vector<string>& occupationList, vector< vector<string> >& friendsList)
{
    ifstream f;
    f.open(filePath.c_str(), ios::in);
    if(!f) cerr << "File not found" << endl;
    else
    {
        string line;
        while(std::getline(f, line))
        {
            vector<string> words = split(line, ',');

            if(words[0].length() > 20 
                || words[1].length() > 3
                || words[2].length() > 30)
            {
                cout << "Error: The length of the input is more than allowed" << endl;
                nameList.clear();
                ageList.clear();
                occupationList.clear();
                friendsList.clear();
                return;
            }
            else
            {
                nameList.push_back(words[0]);
                ageList.push_back(words[1]);
                occupationList.push_back(words[2]);
            }            
            vector<string> tempFridends;
            for(int i=3; i<words.size(); i++)
            {
                tempFridends.push_back(words[i]);
            }
            friendsList.push_back(tempFridends);
        }
    }
    f.close();
}



