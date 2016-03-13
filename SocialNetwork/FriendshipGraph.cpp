#include "FriendshipGraph.h"
#include "BTree.h"
#include "Util.h"

GraphNode::GraphNode(string name, string* friends, int friendNum, int index) {
    this->name = name;
    this->friendNum = friendNum;
    this->index = index;
//    this->indicator = EMPTY_INDICATOR;
    this->friendNameList = new string[this->friendNum]();
    // initialize friend list
    for (int i = 0; i < friendNum; i++) {
        (this->friendNameList)[i] = friends[i];
    }
    
}

//string GraphNode::getName() {
//    return this->name;
//}

//int GraphNode::getIndicator() {
//    return this->indicator;
//}

int GraphNode::getFriendNum() {
    return this->friendNum;
}

int GraphNode::getIndex() {
    return this->index;
}

GraphNode::~GraphNode() {
    delete [] friendNameList;
}


FriendshipGraph::FriendshipGraph(){
    //keep track of node num
    this->nodeNum = 0;
    nodes = new GraphNode*[GRAPH_SIZE];
    for (int i = 0; i < GRAPH_SIZE; i++) {
        nodes[i] = NULL;
    }
    
}

FriendshipGraph::~FriendshipGraph() {
    for (int i = 0; i < GRAPH_SIZE; i++) {
        if (nodes[i] != NULL) {
            delete nodes[i];
            nodes[i] = NULL;
        }
        
    }
    delete [] nodes;
    nodes = NULL;
}

int FriendshipGraph::getNodeNum() {
    return this->nodeNum;
}

int FriendshipGraph::hashFun(string name, int seed) {
    int hash = seed;
    for (int i=0; i < name.length(); i++) {
        hash = hash * 101 + name[i];
    }
    return hash % GRAPH_SIZE;
}

int FriendshipGraph::linearProbing(int hash) {
    return (hash + 1) % GRAPH_SIZE;
}

bool FriendshipGraph::insert(string name, string* friends, int friendNum, int index) {
    // check if the user already present
    for (int i = 0; i < GRAPH_SIZE; i++) {
        if (nodes[i] != NULL && name == nodes[i]->name) {
            cout << "User already present!" << endl;
            return false;
        }
    }
    
    int hash = hashFun(name);
    // linear probing
    while (nodes[hash] != NULL) {
//        if (nodes[hash]->getName() == REMOVE_INDICATOR) {
//            // to do
//            // do something here to remove user
//            // then add new user
//            cout << "User successfully added!" << endl;
//            return true;
//        }
        hash = linearProbing(hash);
    }
    
    // insert user
    GraphNode* insertNode = new GraphNode(name, friends, friendNum, index);
    nodes[hash] = insertNode;
    // increase user count
    nodeNum++;
    // change indicator
//    nodes[hash]->indicator = OCCUPY_INDICATOR;
    
    cout << "User " << nodes[hash]->name<< " successfully added!" << endl;
//    cout << nodes[hash]->getName() << endl;

    
    return true;
    
}

bool FriendshipGraph::addFriendship(string node1, string node2) {
    int hash1 = hashFun(node1);
    int hash2 = hashFun(node2);
    
    // find out the users
    while (nodes[hash1] != NULL && nodes[hash1]->name!= node1) {
        hash1 = linearProbing(hash1);
    }
    while (nodes[hash2] != NULL && nodes[hash2]->name != node2) {
        hash2 = linearProbing(hash2);
    }
    // if the user does not exist
    if (nodes[hash1] == NULL) {
        cout << node1 << " not found!" << endl;
        return false;
    }
    if (nodes[hash2] == NULL) {
        cout << node2 << " not found!" << endl;
        return false;
    }
    // add friendship relationships to users
    // check if relationship already present
    for (int i = 0; i < nodes[hash1]->getFriendNum(); i++) {
        if (nodes[hash1]->friendNameList[i] == node2) {
            cout << "Friendship relationship already present!" << endl;
            return false;
        }
    }
    // create a new ptr that point to the old friend list
    string* oldFriendNameList1 = nodes[hash1]->friendNameList;
    // increase friend num by 1
    nodes[hash1]->friendNum++;
    // assign new memory for the friendlist
    nodes[hash1]->friendNameList = new string[nodes[hash1]->friendNum]();
    // copy the old friend list to the new assigned one
    for (int i = 0; i < nodes[hash1]->friendNum - 1; i++) {
        nodes[hash1]->friendNameList[i] = oldFriendNameList1[i];
    }
    // add the new friend
    nodes[hash1]->friendNameList[nodes[hash1]->friendNum - 1] = node2;
    // free memory for the old friend list
    delete [] oldFriendNameList1;
    
    // do the same for the 2nd user
    // create a new ptr that point to the old friend list
    string* oldFriendNameList2 = nodes[hash2]->friendNameList;
    // increase friend num by 1
    nodes[hash2]->friendNum++;
    // assign new memory for the friendlist
    nodes[hash2]->friendNameList = new string[nodes[hash2]->friendNum]();
    // copy the old friend list to the new assigned one
    for (int i = 0; i < nodes[hash2]->friendNum - 1; i++) {
        nodes[hash2]->friendNameList[i] = oldFriendNameList2[i];
    }
    // add the new friend
    nodes[hash2]->friendNameList[nodes[hash2]->friendNum - 1] = node1;
    // free memory for the old friend list
    delete [] oldFriendNameList2;
    
    cout << "Friendship relationship added!" << endl;
    
    return true;
}

// list friend information of a user
bool FriendshipGraph::listFriendsInfo(string name) {
    int hash = hashFun(name);
    while (nodes[hash] != NULL && nodes[hash]->name != name) {
        hash = linearProbing(hash);
    }
    if (nodes[hash] == NULL) {
        cout << "User not found!" << endl;
        return false;
    }
    else {
        // if user is found, check each friend of the user
        for (int i = 0; i < nodes[hash]->getFriendNum(); i++) {
            int friendHash = hashFun(nodes[hash]->friendNameList[i]);
            while (nodes[friendHash] != NULL && nodes[friendHash]->name != nodes[hash]->friendNameList[i]) {
                friendHash = linearProbing(friendHash);
            }
            if (nodes[friendHash] == NULL) {
                cout << "Something is wrong with listFriendsInfo" << endl;
                return false;
            }
            // print the friend info
            printInfoFromProfileData(nodes[friendHash]->getIndex(), PROFILE_DATA_PATH);
        }
    }
    return true;
}


void FriendshipGraph::printAll() {
    for (int i = 0; i < GRAPH_SIZE; i++) {
        if (nodes[i] != NULL) {
            // concatenate all the friends name
            string tmpFriendNameList = "";
            for (int j = 0; j < nodes[i]->getFriendNum(); j++) {
                if (j < nodes[i]->friendNum-1) {
                    tmpFriendNameList = tmpFriendNameList + nodes[i]->friendNameList[j] + ",";
                }
                else {
                    tmpFriendNameList = tmpFriendNameList + nodes[i]->friendNameList[j];
                }
            }
            cout << nodes[i]->name << "," << nodes[i]->getIndex() << "," << tmpFriendNameList << endl;
//            cout << nodes[i]->friendNum << endl;
        }
    }
}
