#pragma once
#ifndef AVL_H
#define AVL_H

#include<iostream>
#include<stack>

template <class Type>
struct ANode {
	Type data;
	ANode<Type>* lChild;		// left child
	ANode<Type>* rChild;		// right child
	int height;			// height of a node
	// constructor
	ANode(const Type& data = 0) {
		this->data = data;
		lChild = rChild = nullptr;
		this->height = 0;		// height of leaf node is zero
	}
};

template <class Type>
class AVL {
private:
	ANode<Type> *root;				// root of tree

	// insert new node using recursion
	bool insert(ANode<Type>*& r, const Type& data)
	{
		if (r == nullptr) {
			r = new ANode<Type>(data);
			return true;
		}

		if (r->data == data) return false;		//Duplicate do nothing
		else if (r->data > data) insert(r->lChild, data);
		
		else if (r->data < data) insert(r->rChild, data);

		balance(r);			// balance tree
		return true;
	}

	// return number of childern of a node
	// degree of a node = number of its childern
	int numOfChilds(ANode<Type>* pptr) {
		if (pptr->rChild == nullptr && pptr->lChild == nullptr) return 0;
		if (pptr->rChild != nullptr && pptr->lChild != nullptr) return 2;
		return 1;
	}

	// return min value & remove the MIN node
	int removeMin(ANode<Type>* r)
	{
		while (r->lChild != nullptr) r = r->lChild;

		Type min = r->data;

		// remove the 'min 'node
		erase(min);

		return min;
	}

	// delete all nodes
	void deleteAllNodes(ANode<Type>* r) {
		if (r == nullptr) return;

		deleteAllNodes(r->lChild);			// delete left child
		deleteAllNodes(r->rChild);			// delete right child
		delete r;							// delete itself
	}

	// print BST in sorted order
	void printSorted(ANode<Type>* r) {
		if (r == nullptr) return;

		printSorted(r->lChild);
		std::cout << (r->data) << "\n";
		printSorted(r->rChild);
	}

	// return height of a node
	int height(ANode<Type>* r) {
		return r == nullptr ? -1 : r->height;
	}

	// single right rotation
	// Outside Case [Left-Left Imbalance]
	void rotateRight(ANode<Type>*& n)
	{
		if (n == nullptr) return;

		// update pointers
		ANode<Type>* p = n->lChild;
		n->lChild = p->rChild;
		p->rChild = n;

		// update heights
		n->height = std::max(height(n->lChild), height(n->rChild)) + 1;
		p->height = std::max(height(p->lChild), height(p->rChild)) + 1;

		n = p;
	}

	// single left rotation
	// Outside Case [Right-Right Imbalance]
	void rotateLeft(ANode<Type>*& n)
	{
		if (n == nullptr) return;

		// update pointers
		ANode<Type>* p = n->rChild;
		n->rChild = p->lChild;
		p->lChild = n;

		// update heights
		n->height = std::max(height(n->lChild), height(n->rChild)) + 1;
		p->height = std::max(height(p->lChild), height(p->rChild)) + 1;

		n = p;
	}

	// double rotation
	// Inside Case [Right-Left Imbalance]
	//	1)	First rotate left the left child of n
	//	2)	Then, rotate right the n node itself
	void doubleRotateLR(ANode<Type>*& n) {
		rotateLeft(n->lChild);
		rotateRight(n);
	}

	// double rotation
	// Inside Case [Left-Right Imbalance]
	//	1)	First rotate right the right child of n
	//	2)	Then, rotate left the n node itself
	void doubleRotateRL(ANode<Type>*& n) {
		rotateRight(n->rChild);
		rotateLeft(n);
	}

	// to make a tree balance tree using rotation
	void balance(ANode<Type>*& n)
	{
		if (n == nullptr) return;

		// Left Subtree is heavy
		if ((height(n->lChild) - height(n->rChild)) == 2)
		{
			//outside case	[left-left Imbalance] --> Rotate Right
			if (height(n->lChild->lChild) >= height(n->lChild->rChild)) {
				rotateRight(n);
			}
			//inside case	[left-right Imbalance] -- > Doubble Rotation Left Right
			else {
				doubleRotateLR(n);
			}
		}

		// Right Subtree is heavy
		else if ((height(n->rChild) - height(n->lChild)) == 2)
		{
			//outside case	[right-right Imbalance] --> Rotate Left
			if (height(n->rChild->rChild) >= height(n->rChild->lChild)) {
				rotateLeft(n);
			}
			//inside case	[right-left Imbalance] -- > Doubble Rotation Right Left
			else {
				doubleRotateRL(n);
			}
		}

		n->height = std::max(height(n->lChild), height(n->rChild)) + 1;
	}

public:

	// constructor
	AVL() {
		this->root = nullptr;
	}

	// return true, if key is found
	bool search(const Type& key, ANode<Type>*& foundAt) {
		ANode<Type>* r = root;
		while (r != nullptr) {
			if (r->data == key) {
				foundAt = r;
				return true;
			}

			if (r->data < key) r = r->rChild;
			else r = r->lChild;

			//r = (key > r->data) ? r->rChild : r->lChild;
		}
		return false;
	}

	// return true, if key is found
	bool search(const Type& key) {
		ANode<Type>* curr = nullptr;
		return search(key, curr);
	}

	// insert a new node
	bool insert(const Type& data) {
		return insert(this->root, data);
	}

	// to remove an element 
	void erase(ANode<Type>*& nodeToRemove, const Type& data)
	{
		if (nodeToRemove == nullptr) return;		// empty tree

		if (data < nodeToRemove->data) erase(nodeToRemove->lChild, data);
		else if (data > nodeToRemove->data) erase(nodeToRemove->rChild, data);

		// two childern
		else if (numOfChilds(nodeToRemove) == 2)
		{
			nodeToRemove->data = removeMin(nodeToRemove->rChild)->data;		// remove min Node in right Sub-tree 
			erase(nodeToRemove->rChild, nodeToRemove->data);
		}
		else
		{
			ANode<Type>* temp = nodeToRemove;
			nodeToRemove = (nodeToRemove->lChild != nullptr) ? nodeToRemove->lChild : nodeToRemove->rChild;

			delete temp;
		}

		balance(nodeToRemove);		// balance tree to maintain AVL properties
	}

	// to print BST in sorted order
	void printSorted() {
		printSorted(this->root);
	}

	// reverse inorder traversal
	void printBinaryTree(ANode<Type>* root, int space = 0, int height = 10)
	{
		// Base case
		if (root == nullptr)
			return;

		// increase distance between levels
		space += height;

		// print right child first
		printBinaryTree(root->rChild, space);
		std::cout << std::endl;

		// print current node after padding with spaces
		for (int i = height; i < space; i++)
			std::cout << ' ';
		std::cout << root->data;

		// print left child
		std::cout << std::endl;
		printBinaryTree(root->lChild, space);
	}

	void printBinaryTree() {
		printBinaryTree(root);
	}

	ANode<Type>* getRoot() { return this->root; }

	// destructor
	~AVL() {
		deleteAllNodes(root);
	}
};
#endif //!AVL_H