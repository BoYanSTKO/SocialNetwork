#ifndef SOCIALNETWORK_FRIENDSHIPGRAPH_H_
#define SOCIALNETWORK_FRIENDSHIPGRAPH_H_

#include <string.h>
#include <iostream>
using namespace std;
#define GRAPH_SIZE 201  // default graph size
//#define PROFILE_DATA_PATH "/Users/boyan/Dropbox/UCSB_Class/CS130A/PJ3/SocialNetwork/SocialNetwork/ProfileData.txt"
//#define REMOVE_INDICATOR "~~"

class GraphNode {
    string name; // name is the key
    string* friendNameList;
    int friendNum;
    int index; // index to the profile data
//    int indicator; // indicate whether it's removed, occupied or empty
    
public:
    GraphNode(string name, string* friends, int friendNum, int index);
    ~GraphNode();
    string getName();
    int getFriendNum();
//    int getIndicator();
    int getIndex();

friend class FriendshipGraph;  // FriendshipGraph has access to private members in GraphNode

};

// Graph
// the size is 201 to store a max of 100 records
class FriendshipGraph {
private:
    int nodeNum;
    GraphNode** nodes;
    int hashFun(string name, int seed = 0); // hash function
    int linearProbing(int hash); // linear probing


public:
    // constructor
    FriendshipGraph();
    // destructor
    ~FriendshipGraph();
    int getNodeNum();
    // insert record
    bool insert(string name, string* friends, int friendNum, int index);
    bool addFriendship(string node1, string node2);
    void printAll();
    bool listFriendsInfo(string name);
};




#endif  // SOCIALNETWORK_FRIENDSHIPGRAPH_H_
