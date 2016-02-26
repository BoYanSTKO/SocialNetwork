


#include <iostream>

#include "BTree.h"

using namespace std;

//  Constructor: initialize the BTreeNode
BTreeNode::BTreeNode(int m, int l, bool leaf){
	this->m = m;
	this->l = l;
	this->isLeaf = leaf;

	this->numKey = 0;
	nextNode = NULL;
	if(leaf == true)	// a leaf node
	{
		this->keys = NULL;
		this->children = NULL;
		this->items = new item[l];
	}
	else				// an internal node
	{
		this->keys = new string[m-1];
		this->children = new BTreeNode*[m];	//* need to initialize each one later
		this->items = NULL;
	}
}


// Constructor: initialize the BTree
BTree::BTree(int m, int l){
	this->root = NULL;
	this->m = m;
	this->l = l;
}


// Insert the data into the BTree
void BTree::insert(item data){
	string name = data.name;
	cout << "inserting... " << name << endl;	
	
	// Case 1: No data yet...
	if(this->root == NULL)
	{
		// Create a leaf node
		BTreeNode* newLeafNode = new BTreeNode(this->m, this->l, true);
		newLeafNode->items[0] = data;
		newLeafNode->numKey++;	// increse the number items

		// Create a internal node pointing to the leaf node
		BTreeNode* newInternalNode = new BTreeNode(this->m, this->l, false);
		newInternalNode->children[0] = newLeafNode;	// point to the leaf node

		// Pointing root to the new node
		this->root = newInternalNode;
		cout << data.name << " (" << data.index << ") is inserted in a new node." << endl;
	}
	// Case 2: There is only one item/leaf node
	else if(this->root->numKey == 0)	
	{
		BTreeNode* itemNode = this->root->children[0];	// pointer to the item/leaf node
		
		// Case 2a: The data node is less than full; no need to create another data node
		if(itemNode->numKey < this->l)	
		{
			int indexToInsert = 0;
			// Find the index of the slot to insert
			while(indexToInsert < itemNode->numKey 
						&& (itemNode->items[indexToInsert]).name < name)
			{
				indexToInsert++;
			}
			// Push back data larger than the inserting item to have a space for the inserting data item
			for(int i=itemNode->numKey-1; i >= indexToInsert; i--)
			{
				itemNode->items[i+1] = itemNode->items[i];
			}
			// Insert the data item
			itemNode->items[indexToInsert] = data;
			itemNode->numKey++;
			cout << data.name << " (" << data.index << ") is inserted." << endl;
		}
		// Case 2b: The data node is full (numKey = l); another leaf node has to be created
		else
		{
			cout << "The leaf node is already full!" << endl;
			if(itemNode->numKey != this->l)
			{
				cout << "The counter is wrong..." << endl;
			}
			// Find the index of the slot to insert
			int indexToInsert = 0;
			while(indexToInsert < itemNode->numKey
					&& (itemNode->items[indexToInsert]).name < name)	// find the index of the slot to insert
			{
				indexToInsert++;
			}
			// Create a temporary list of items to hold all data including the newly inserted data
			item* tempItems = new item[this->l + 1];
			for(int i=0; i<indexToInsert; i++)
			{
				tempItems[i] = itemNode->items[i];
			}
			tempItems[indexToInsert] = data;
			for(int i=indexToInsert+1; i < this->l + 1; i++)
			{
				tempItems[i] = itemNode->items[i-1];
			}

			// Clean the original data/leaf node and have the first half data in temporary list inserting into the new data
			itemNode->items = new item[l];
			itemNode->numKey = 0;
			for(int i=0; i< (this->l)/2+1; i++)
			{
				itemNode->items[i] = tempItems[i];
				itemNode->numKey++;
			}
			// cout << "box 1: " << itemNode->numKey << " items: " << itemNode->items[0].name << endl;
			// cout << "box 1: " << itemNode->numKey << " items: " << itemNode->items[1].name << endl;
			
			// Create a new item/leaf node
			BTreeNode* newLeafNode = new BTreeNode(this->m, this->l, true);
			newLeafNode->numKey = 0;
			for(int i=(this->l)/2+1; i< (this->l)+1; i++)
			{
				newLeafNode->items[i-(this->l)/2-1] = tempItems[i];
				newLeafNode->numKey++;
			}
			// cout << "box 2: " << newLeafNode->numKey << " items: " << newLeafNode->items[0].name << endl;
			// cout << "box 2: " << newLeafNode->numKey << " items: " << newLeafNode->items[1].name << endl;
			itemNode->nextNode = newLeafNode;

			// Revise the partent internal node
			this->root->keys[0] = (newLeafNode->items[0]).name;
			this->root->children[1] = newLeafNode;
			this->root->numKey++;
			//cout << "root node: " << this->root->numKey << " children" << endl;
			cout << data.name << " (" << data.index << ") is inserted...after a new leaf node is created" << endl;
		}
	}
	// Case 3: Nomal condition (after initilization)
	else
	{
		cout << "In case 3..." << endl;
		// Find the leaf node to insert the data
		BTreeNode* leafNodeToInsert = this->getLeafNodeToInsert(data, this->root);
		//cout << nodeToInsert->items[0].name << endl;
		// Double check if this is a leaf node
		if(leafNodeToInsert->isLeaf != true)
		{
			cout << "Error: Something is wrong with the getLeafNodeToInsert()..." << endl;
		}

		// Case 3a: leaf node to insert is not full
		if(leafNodeToInsert->numKey < this->l)
		{
			int indexToInsert = 0;
			// Find the index of the slot to insert
			while(indexToInsert < leafNodeToInsert->numKey
					&& (leafNodeToInsert->items[indexToInsert]).name < name)
			{
				indexToInsert++;
			}
			// Push back data larger than the inserting item to have a space for the inserting data item
			for(int i=leafNodeToInsert->numKey-1; i >= indexToInsert; i--)
			{
				leafNodeToInsert->items[i+1] = leafNodeToInsert->items[i];
			}
			// Insert the data item
			leafNodeToInsert->items[indexToInsert] = data;
			leafNodeToInsert->numKey++;
			cout << name << " (" << data.index << ") is inserted." << endl;
		}
		// Case 3b: leaf node to insert is full
		else
		{

		}
	}


}


// Find the leaf node which the data is about to insert
BTreeNode* BTree::getLeafNodeToInsert(item data, BTreeNode* node)
{
	string name = data.name;
	if(node != NULL && node->isLeaf != true)
	{
		for(int i=0; i<node->numKey; i++)
		{
			if(name <= node->keys[i])
			{
				return getLeafNodeToInsert(data, node->children[i]);
			}
			if(name > node->keys[node->numKey-1])
			{
				return getLeafNodeToInsert(data, node->children[node->numKey]);
			}
		}
	}
	if(node != NULL && node->isLeaf == true)
	{
		return node;
	}
}


// Traverse the tree to print out all nodes' values in order
void BTree::traverse(BTreeNode* node)
{
	//cout << "Traversing the tree..." << endl;
	if(node == NULL)
	{
		cout << "This is an empty tree" << endl;
	}
	else
	{
		if(node->isLeaf == true)
		{	
			item* tempData = node->items;
			cout << node->numKey << endl;
			for(int i=0; i< node->numKey; i++)
			{				
				cout << tempData[i].name << " (" << tempData[i].index << ")\t";
			}
			cout << endl;
		}
		else
		{
			for(int i=0; i<= node->numKey; i++)
			{
				this->traverse(node->children[i]);
			}
		}		
	}
}


BTreeNode* BTree::getRootNode()
{
	return this->root;
}
  	


