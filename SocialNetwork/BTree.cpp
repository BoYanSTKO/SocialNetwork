


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

				// Revise the parent internal node
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
				// Double check
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

				// Delete and recreate the original data/leaf node and have 
				//    the first half data in temporary list inserting into the new data
				delete[] leafNodeToInsert->items;
				leafNodeToInsert->items = new item[l];
				leafNodeToInsert->numKey = 0;
				for(int i=0; i< (this->l)/2+1; i++)
				{
					leafNodeToInsert->items[i] = tempItems[i];
					leafNodeToInsert->numKey++;
				}
				// Create a new item/leaf node and insert the second half data
				//	  into the new leaf node
				BTreeNode* newLeafNode = new BTreeNode(this->m, this->l, true);
				newLeafNode->numKey = 0;
				for(int i=(this->l)/2+1; i< (this->l)+1; i++)
				{
					newLeafNode->items[i-(this->l)/2-1] = tempItems[i];
					newLeafNode->numKey++;
				}
				newLeafNode->nextNode = leafNodeToInsert->nextNode;
				leafNodeToInsert->nextNode = newLeafNode;
				//newLeafNode->parentNodePtr = parentNode;
				delete[] tempItems;

				// Revise the parent internal node
				// Double check
				if(leafNodeToInsert->parentNodePtr->numKey != this->m-1)
				{
					cout << "Something is wrong";
				}
				cout << "@@@@@@@" << endl;
				// Split the full parent internal node into half
				BTreeNode*	currentInternalNode = leafNodeToInsert->parentNodePtr;
				while(currentInternalNode != NULL && currentInternalNode->numKey == this->m-1)
				// Iterative until parent node is not full or hit the root
				{
					cout << "1111" << endl;
					// Revise the parent node (current internal node);
					//	 insert first half of original children
					for(int i=1; i<this->m; i++)	// The first child is always kept
					{
						currentInternalNode->children[i] = NULL;
					}
					delete[] currentInternalNode->keys;
					currentInternalNode->keys = new string[m-1];;
					cout << "111122" << endl;
					for(int i=1; i<(this->m+1)/2; i++)
					{
						cout << "11113333" << endl;
						currentInternalNode->children[i] = currentInternalNode->children[i-1]->nextNode;
						cout << "11114444" << endl;
						currentInternalNode->keys[i-1] = this->getKeyToFill(currentInternalNode, i-1);						
					}
					currentInternalNode->numKey = (this->m+1)/2-1;
					cout << "2222" << endl;
					// Create the new internal node
					//	 insert second half of the original children
					BTreeNode* newInternalNode = new BTreeNode(this->m, this->l, false);
					newInternalNode->children[0] = currentInternalNode->children[(this->m+1)/2-1]->nextNode;
					newInternalNode->children[0]->parentNodePtr = newInternalNode;
					for(int i=1; i<(this->m+1)/2; i++)
					{
						newInternalNode->children[i] = newInternalNode->children[i-1]->nextNode;
						newInternalNode->keys[i-1] = this->getKeyToFill(newInternalNode, i-1);
						newInternalNode->children[i]->parentNodePtr = newInternalNode;
					}
					newInternalNode->numKey = (this->m+1)/2-1;					
					cout << "3333" << endl;
					// Connect original node to the new node and the new node to the following node
					newInternalNode->nextNode = currentInternalNode->nextNode;
					currentInternalNode->nextNode = newInternalNode;
					cout << "4444" << endl;
					if(currentInternalNode->parentNodePtr == NULL) // It is head!
					{
						// Create a new internal node as the head node
						BTreeNode* newRoot = new BTreeNode(this->m, this->l, false);
						newRoot->children[0] = currentInternalNode;
						newRoot->children[1] = currentInternalNode->nextNode;
						newRoot->numKey = 1;
						newRoot->keys[0] = this->getKeyToFill(currentInternalNode, 0);
						newRoot->children[0]->parentNodePtr = newRoot;
						newRoot->children[1]->parentNodePtr = newRoot;
						this->root = newRoot;
						break;
					}
					else
					{
						if(currentInternalNode->parentNodePtr->numKey == this->m-1)
						// The parent node is a full node
						{
						}
						else	// parent node is not full
						{
							BTreeNode* tempParentNode = currentInternalNode->parentNodePtr;
							if(tempParentNode->numKey < this->m-1 
								&& tempParentNode->numKey >= 0)
							{
								// Double check
								cout << "Something is wrong here" << endl;
							}
							for(int i=1; i<=tempParentNode->numKey+1; i++)
							{
								tempParentNode->children[i] = tempParentNode->children[i-1]->nextNode;
								tempParentNode->children[i]->parentNodePtr = tempParentNode;
								tempParentNode->keys[i-1] = this->getKeyToFill(tempParentNode, i-1);
							}
							tempParentNode->numKey++;
							break;
						}
					}
					currentInternalNode = currentInternalNode->parentNodePtr;
					cout << "4444" << endl;
				}
				cout << name << " (" << data.index << ") is inserted." << endl;
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
	cout << "Wrong use of this function.." << endl;
	return NULL;
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
	return NULL;
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


// Get the key for the currentInternalNode at the index of keyIndex (0,1,2...)
string BTree::getKeyToFill(BTreeNode* currentInternalNode, int keyIndex)
{
	string key = "";
	cout << "AAAA" << endl;
	BTreeNode*  node = currentInternalNode;
	if(node != NULL)
	{
		cout << "BBBB" << endl;
		node = node->children[keyIndex+1];
		while(node->isLeaf == false)
		{
			cout << "CCCC" << endl;
			node = node->children[0];
		}
		cout << "DDDD" << endl;
		key = (node->items[0]).name;
	}
	cout << "EEEE" << endl;
	if(key == "")
	{
		cout << "Something is wrong." << endl;
	}
	cout << "key = " << key << endl;
	return key;
}
