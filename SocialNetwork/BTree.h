#ifndef SOCIALNETWORK_BTREE_H_
#define SOCIALNETWORK_BTREE_H_

#include <string>
using namespace std;


struct item {
	string name;
	int index;
};

class BTreeNode {
  private:
  	string* keys;	// array of keys
  	BTreeNode** children;	// array of pointers to children nodes
  	item* items;	// array items node
  	int numKey;	// current number of keys/items in the node
  	int m;		// each non-leaf node can have ceil(m/2) to m children (except root: 2 to m children)
  				//   and it can has at most m-1 keys
  	int l;		// each leaf node can contain ceil(l/2) to l items
  	bool isLeaf;	// true if node is a leaf, false otherwise
  	BTreeNode* nextNode;	// pointer to the next right node
  	BTreeNode* parentNodePtr;	// pointer to the parent node

  public:
  	BTreeNode(int m, int l, bool leaf);
  	//  Constructor: initialize the BTreeNode
  	~BTreeNode();



  friend class BTree;	// BTree class can access private members in BTreeNode clas
};



class BTree {
  private:
  	BTreeNode* root;	// pointer to the root node of the tree
  	int m;		// each non-leaf node can have ceil(m/2) to m children (except root: 2 to m children)
  				//   and it can has at most m-1 keys
  	int l;		// each leaf node can contain ceil(l/2) to l items
    // Find the leaf node which the data is about to insert
  	BTreeNode* getLeafNodeToInsert(item , BTreeNode* node);
  	// Get the index in the leaf node to insert the value
  	int getIndexToInsert(BTreeNode* leafNode, string value);
  	// Find the internal node who is the parent node which the data is about to insert
    BTreeNode* getInternalNodeToInsert(item data, BTreeNode* node);
    // Split this internal node into two, (and insert the new node)
    void splitInternalNode(BTreeNode* node, string name);
    // Get the key for the currentInternalNode at the index of i (1,2,3...)
    string getKeyToFill(BTreeNode* currentInternalNode, int i);


  public:
  	BTree(int m, int l);
  	// Constructor: initialize the BTree
  	void insert(item data);
  	// Insert the data into the BTree
  	void traverse(BTreeNode* node);
  	// Traverse the subtree of node to print out all nodes' values in order
  	BTreeNode* getRootNode();
  	// Get the root node of the B Tree object
  	~BTree();
};




#endif  // SOCIALNETWORK_BTREE_H_
