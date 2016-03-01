


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
	parentNodePtr = NULL;
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

		newLeafNode->parentNodePtr = newInternalNode;	// point to the parent node

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
			int indexToInsert = getIndexToInsert(itemNode, name);
			// Push back data larger than the inserting item to have a space for the inserting data item
			for(int i=itemNode->numKey-1; i >= indexToInsert; i--)
			{
				itemNode->items[i+1] = itemNode->items[i];
			}
			// Insert the data item
			itemNode->items[indexToInsert] = data;
			itemNode->numKey++;
			cout << name << " (" << data.index << ") is inserted." << endl;
		}
		// Case 2b: The data node is full (numKey = l); another leaf node has to be created
		else
		{
			cout << "The leaf node is already full!" << endl;
			if(itemNode->numKey != this->l)
			{
				cout << "The counter is wrong..." << endl;
			}

			int indexToInsert = getIndexToInsert(itemNode, name);

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

			// Delete and recreate the original data/leaf node and have the first half data in temporary list inserting into the new data
			delete[] itemNode->items;
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

			delete[] tempItems;

			// cout << "box 2: " << newLeafNode->numKey << " items: " << newLeafNode->items[0].name << endl;
			// cout << "box 2: " << newLeafNode->numKey << " items: " << newLeafNode->items[1].name << endl;
			itemNode->nextNode = newLeafNode;
			newLeafNode->parentNodePtr = this->root;

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
			cout << "case 3a" << endl;

			int indexToInsert = getIndexToInsert(leafNodeToInsert, name);

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
		// Case 3b: leaf node to insert is full --> leaf node is going to split
		else
		{
			cout << "case 3b" << endl;
			//BTreeNode* parentNode = getInternalNodeToInsert(data, this->root);
			BTreeNode* parentNode = leafNodeToInsert->parentNodePtr;

			// Case 3b-a: Parent node has less than 4 keys: still have slot to hold extra leaf node
			if(parentNode->numKey < this->m-1)
			{
				cout << "case 3b-a" << endl;
				if(leafNodeToInsert->numKey != this->l)
				{
					cout << "The counter is wrong..." << endl;
				}
				// Find the index of the slot to insert
				int indexToInsert = getIndexToInsert(leafNodeToInsert, name);
				// Create a temporary list of items to hold all data including the newly inserted data
				item* tempItems = new item[this->l + 1];
				for(int i=0; i<indexToInsert; i++)
				{
					tempItems[i] = leafNodeToInsert->items[i];
				}
				tempItems[indexToInsert] = data;
				for(int i=indexToInsert+1; i < this->l + 1; i++)
				{
					tempItems[i] = leafNodeToInsert->items[i-1];
				}

				// Delete and recreate the original data/leaf node and have the first half data in temporary list inserting into the new data
				delete[] leafNodeToInsert->items;
				leafNodeToInsert->items = new item[l];
				leafNodeToInsert->numKey = 0;
				for(int i=0; i< (this->l)/2+1; i++)
				{
					leafNodeToInsert->items[i] = tempItems[i];
					leafNodeToInsert->numKey++;
				}

				// Create a new item/leaf node
				BTreeNode* newLeafNode = new BTreeNode(this->m, this->l, true);
				newLeafNode->numKey = 0;
				for(int i=(this->l)/2+1; i< (this->l)+1; i++)
				{
					newLeafNode->items[i-(this->l)/2-1] = tempItems[i];
					newLeafNode->numKey++;
				}
				newLeafNode->nextNode = leafNodeToInsert->nextNode;
				leafNodeToInsert->nextNode = newLeafNode;
				newLeafNode->parentNodePtr = parentNode;
				delete[] tempItems;

				// Revise the partent internal node
				parentNode->numKey++;
				//   re-arrange the keys and children
				int tempIndex = 0;
				//	 the first children pointer does not neet to change
				cout << parentNode->numKey << endl;
				while(tempIndex< parentNode->numKey)
				{
					parentNode->children[tempIndex+1] = parentNode->children[tempIndex]->nextNode;
					parentNode->keys[tempIndex] = parentNode->children[tempIndex+1]->items[0].name;
					tempIndex++;
				}

				cout << name << " (" << data.index << ") is inserted." << endl;
			}
			// Case 3b-b: Parent node has 4 keys: it is full... the parent node has to be split
			else
			{
				cout << "case 3b-b" << endl;
				/*
				int indexToInsert = getIndexToInsert(leafNodeToInsert, name);
				// Create a temporary list of items to hold all data including the newly inserted data
				item* tempItems = new item[this->l + 1];
				for(int i=0; i<indexToInsert; i++)
				{
					tempItems[i] = leafNodeToInsert->items[i];
				}
				tempItems[indexToInsert] = data;
				for(int i=indexToInsert+1; i < this->l + 1; i++)
				{
					tempItems[i] = leafNodeToInsert->items[i-1];
				}

				// Clean the original data/leaf node and have the first half data in temporary list inserting into the new data
				leafNodeToInsert->items = new item[l];
				leafNodeToInsert->numKey = 0;
				for(int i=0; i< (this->l)/2+1; i++)
				{
					leafNodeToInsert->items[i] = tempItems[i];
					leafNodeToInsert->numKey++;
				}

				// Create a new item/leaf node
				BTreeNode* newLeafNode = new BTreeNode(this->m, this->l, true);
				newLeafNode->numKey = 0;
				for(int i=(this->l)/2+1; i< (this->l)+1; i++)
				{
					newLeafNode->items[i-(this->l)/2-1] = tempItems[i];
					newLeafNode->numKey++;
				}
				newLeafNode->nextNode = leafNodeToInsert->nextNode;
				leafNodeToInsert->nextNode = newLeafNode;




				// Revise the partent internal node
				// Split the node
				// Refresh the orignal parent node
				BTreeNode* tempFirstChildNode = parentNode->children[0];
				parentNode = new BTreeNode(this->m, this->l, false);
				// Assign children and keys to the parent node
				parentNode->children[0] = tempFirstChildNode;
				for(int i=0; i< m/2; i++)	// i= 0, 1
				{
					parentNode->children[i+1] = parentNode->children[i]->nextNode;
					parentNode->keys[i] = parentNode->children[i+1]->items[0].name;
				}
				parentNode->numKey = this->m/2;	// 2 keys, 3 children

				// Create a new internal node
				BTreeNode* newInternalNode = new BTreeNode(this->m, this->l, false);
				// Assign children and keys to the new internal node
				newInternalNode->children[0] = parentNode->children[m/2]->nextNode;
				for(int i=m/2; i< m; i++)	// i=2, 3, 4
				{
					newInternalNode->children[i-m/2+1] = newInternalNode->children[i-m/2]->nextNode;
					newInternalNode->keys[i-m/2] = newInternalNode->children[i-m/2+1]->items[0].name;
				}
				newInternalNode->numKey = this->m/2;	// 2 keys, 3 children

				// The new internal node is still dangling...



				parentNode->numKey++;
				//   re-arrange the keys and children
				int tempIndex = 0;
				//	 the first children pointer does not neet to change
				cout << parentNode->numKey << endl;
				while(tempIndex< parentNode->numKey)
				{
					parentNode->children[tempIndex+1] = parentNode->children[tempIndex]->nextNode;
					parentNode->keys[tempIndex] = parentNode->children[tempIndex+1]->items[0].name;
					tempIndex++;
				}
				*/

			}
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


// Find the internal node who is the parent node which the data is about to insert
BTreeNode* BTree::getInternalNodeToInsert(item data, BTreeNode* node)
{
	string name = data.name;
	if(node != NULL && node->isLeaf != true && node->children[0]->isLeaf == false)
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
	if(node != NULL && node->isLeaf != true && node->children[0]->isLeaf == true)
	{
		return node;
	}
	cout << "Wrong use of this function.." << endl;
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


// Get the root node of the B Tree object
BTreeNode* BTree::getRootNode()
{
	return this->root;
}


// Get the index in the leaf node to insert the value
int BTree::getIndexToInsert(BTreeNode* leafNode, string value)
{
	int indexToInsert = 0;
	// Find the index of the slot to insert
	while(indexToInsert < leafNode->numKey
			&& (leafNode->items[indexToInsert]).name < value)
	{
		indexToInsert++;
	}
	return indexToInsert;
}


// Split this internal node into two, (and insert the new node to the parent node)
void BTree::splitInternalNode(BTreeNode* node, string name)
{
	if(node->isLeaf == true || node->numKey != this->m-1)
	{
		cout << "Error: wrong use of this function" << endl;
	}




}


BTree::~BTree()
{
	//cout << "~~~~" << endl;
	delete root;
	//cout << "!!!!" << endl;
}


BTreeNode::~BTreeNode()
{
	//cout << "Deallocating memory..." << endl;
	if(this != NULL)
	{
		//cout << "11" << endl;
		if(this->isLeaf)
		{
			//cout << "22" << endl;
			if(items != NULL)
			{
				//cout << items[0].name << endl;
				delete[] items;
				//cout << "222" << endl;
			}
			//cout << "2222" << endl;		
		}
		else
		{
			//cout << "33" << endl;
			if(children != NULL)
			{
				for(int i=0; i<=this->numKey;i++)
				{
					//cout << "i=" << i << endl;
					if(children[i] != NULL)
					{
						delete children[i];
					}
					
					//cout << "delete" << endl;
				}
				delete[] children;
			}
			
			//cout << "333" << endl;
			if(keys != NULL)
			{
				delete[] keys;
			}
		}
		//cout << "44" << endl;
		
		if(nextNode != NULL)
		{
			nextNode = NULL;
		}		
		//cout << "55" << endl;

		if(parentNodePtr != NULL)
		{
			parentNodePtr = NULL;
		}
		//cout << "66" << endl;
	}
	
}
