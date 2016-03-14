#include "FriendshipGraph.h"
#include "BTree.h"
#include "Util.h"

#include <limits>

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

string GraphNode::getName() {
   return this->name;
}

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
        hash = (hash * 101 + name[i]) % GRAPH_SIZE;
    }
    return hash;
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
    //cout << name << endl;
    int hash = hashFun(name);
    //cout << "init hash " << hash << endl;
    while (nodes[hash] != NULL && nodes[hash]->name != name) {
//        cout << " here" << endl;
        hash = linearProbing(hash);
        //cout << " hash " << hash << endl;
    }
    if (nodes[hash] == NULL) {
        cout << "User not found!" << endl;
        return false;
    }
    else {
        //cout << " hash " << hash << endl;
        //cout << "here " << nodes[hash]->getIndex() << endl;
        // if user is found, check each friend of the user
        if(nodes[hash]->getFriendNum()>=1){
            cout << "List the friends of " << name << ":" << endl;
        }        

        for (int i = 0; i < nodes[hash]->getFriendNum(); i++) {
            //cout << "friend " << nodes[hash]->friendNameList[i] << endl;
            int friendHash = hashFun(nodes[hash]->friendNameList[i]);
            //cout << "init friend hash " << friendHash << endl;
            while (nodes[friendHash] != NULL && nodes[friendHash]->name != nodes[hash]->friendNameList[i]) {
                friendHash = linearProbing(friendHash);
                //cout << " friend hash " << friendHash << endl;
            }
            if (nodes[friendHash] == NULL) {
                cout << "Something is wrong with listFriendsInfo" << endl;
                return false;
            }
            // print the friend info
            //cout << nodes[friendHash]->getIndex() << endl;            
            printInfoFromProfileData(nodes[friendHash]->getIndex(), PROFILE_DATA_PATH);
        }
    }
    return true;
}


// find common friends of two users
bool FriendshipGraph::findCommonFriends(string node1, string node2) {
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
    for (int i = 0; i < nodes[hash1]->getFriendNum(); i++) {
        for (int j = 0; j < nodes[hash2]->getFriendNum(); j++) {
            if (nodes[hash1]->friendNameList[i] == nodes[hash2]->friendNameList[j]) {
                int friendHash = hashFun(nodes[hash2]->friendNameList[j]);
                while (nodes[friendHash] != NULL && nodes[friendHash]->name != nodes[hash2]->friendNameList[j]) {
                    friendHash = linearProbing(friendHash);
                }
                if (nodes[friendHash] == NULL) {
                    cout << "Something is wrong with findCommonFriends" << endl;
                    return false;
                }
                printInfoFromProfileData(nodes[friendHash]->getIndex(), PROFILE_DATA_PATH);
            }
        }
    }
    
    return true;
}


// find the node with min dist value, from a set of vertices not yet included in the shortest path tree (spt)
int FriendshipGraph::minDist(int dist[], bool sptSet[]) {
    int min = numeric_limits<int>::max(); // initialize min value to max integer
    int min_index = -1;
    for (int i = 0; i < GRAPH_SIZE; i++) {
        if (sptSet[i] == false && dist[i] <= min) {
            min = dist[i];
            min_index = i;
        }
    }
    if (min_index == -1) {
        cout << "Something wrong with minDist()" << endl;
    }
    return min_index;
}


// find degree of separation between to users using Dijkstra’s shortest path algorithm
int FriendshipGraph::degreeOfSeparation(string node1, string node2) {
    
    int dist[GRAPH_SIZE];  // store distance, dist[i] stores dist from node1 to i
    bool sptSet[GRAPH_SIZE];  //sptSet[i] is true if node[i] is included in the spt or shortest dist from source to i is finalized
    for (int i = 0; i < GRAPH_SIZE; i++) {
        dist[i] = numeric_limits<int>::max();  // initialize distance with infinity
        sptSet[i] = false;
    }
    // hash to find the position of node1
    int hash1 = hashFun(node1);
    while (nodes[hash1] != NULL && nodes[hash1]->getName() != node1) {
        hash1 = linearProbing(hash1);
    }
    // if the user does not exist
    if (nodes[hash1] == NULL) {
        cout << node1 << " not found!" << endl;
        return numeric_limits<int>::max();
    }
    // hash to find the position of node2
    int hash2 = hashFun(node2);
    while (nodes[hash2] != NULL && nodes[hash2]->getName() != node2) {
        hash2 = linearProbing(hash2);
    }
    // if user does not exist
    if (nodes[hash2] == NULL) {
        cout << node2 << " not found!" << endl;
        return numeric_limits<int>::max();
    }
    
    
    // set dist from node1 to node1 to 0
    dist[hash1] = 0;
    
    for (int count = 0; count < GRAPH_SIZE - 1 ; count++) {
        // pick the min dist node from the set of nodes not processed yet
        int pos = minDist(dist, sptSet);
        
        // mark the node as processed
        sptSet[pos] = true;
        
        // if position is empty, continue to the next loop
        if (nodes[pos] == NULL) {
            continue;
        }
        
        // update the dist value of the adjacent nodes (friends) of the processed node (user)
        for (int j = 0; j < nodes[pos]->getFriendNum(); j++) {
            // hash to find the position of each friend
            int tmpHash = hashFun(nodes[pos]->friendNameList[j]);
            while (nodes[tmpHash] != NULL && nodes[tmpHash]->getName() != nodes[pos]->friendNameList[j]) {
                tmpHash = linearProbing(tmpHash);
            }
            if (nodes[tmpHash] == NULL) {
                cout << "Something is wrong with degreeOfSeparation" << endl;
            }
            //
            if (!sptSet[tmpHash] && dist[pos] != numeric_limits<int>::max() && dist[pos] + 1 < dist[tmpHash]) {
                dist[tmpHash] = dist[pos] + 1;
            }
        }
    }

    return dist[hash2];  // return the shortest path from node1 to node2
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


