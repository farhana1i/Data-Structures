#include<iostream>
#include<stack>
#include<queue>

using namespace std;

template <typename k, typename v>
struct TNode
{
	k key;
	v value;
	TNode<k, v>* leftChild;
	TNode<k, v>* rightChild;
};

template <typename k, typename v>
class BST {
private:
	TNode<k, v>* root;		//root of the tree

	// wrapper function for inserting recursively
	void insertRec(TNode<k, v>*& r, k const  key, v const value) {
		if (r == nullptr) {
			r = new TNode<k, v>;
			r->key = key;
			r->value = value;
			r->leftChild = r->rightChild = nullptr;
		}
		else {
			if (r->key == key) r->value = value;		// update value only
			else if (r->key > key) insertRec(r->leftChild, key, value);
			else insertRec(r->rightChild, key, value);
		}
	}

	// wrapper function for searching recursively
	v* searchRec(TNode<k, v>* r, k const  key) {
		if (r == nullptr)  return nullptr;
		else {
			if (r->key == key) return &r->value;
			else if (r->key > key) return searchRec(r->leftChild, key);
			else return searchRec(r->rightChild, key);
		}
	}

	// wrapper function for recursively printing key using inorder traversal
	void inorderPrintKeysRec(TNode<k, v>* r) const {
		if (r == nullptr) return;

		inorderPrintKeysRec(r->leftChild);
		std::cout << r->key << " ";
		inorderPrintKeysRec(r->rightChild);
	}

	// wrapper function for counting total nodes using recursion
	void NoofNodesRec(TNode<k, v>* r, int& totalNodes) const {
		if (r == nullptr) return;

		totalNodes++;
		if (r->leftChild == nullptr && r->rightChild == nullptr) return;

		NoofNodesRec(r->leftChild, totalNodes);
		NoofNodesRec(r->rightChild, totalNodes);
	}

	// to return height of a node
	int getHeight(TNode<k, v>* r)const {
		if (r == nullptr) return -1;

		return 1 + std::max(getHeight(r->leftChild), getHeight(r->rightChild));
	}

	// delete all nodes
	void deleteAllNodes(TNode<k, v>* r) {
		if (r == nullptr) return;

		deleteAllNodes(r->leftChild);			// delete left child
		deleteAllNodes(r->rightChild);			// delete right child
		delete r;								// delete itself
	}
public:
	// constructor
	BST() {
		this->root = nullptr;
	}

	// insert a new data item using recursion
	void insertRec(k const  key, v const value) {
		insertRec(root, key, value);
	}

	// insert a new data item using iteration
	void insert(k const  key, v const value) {
		TNode<k, v>* r = new TNode<k, v>;
		r->key = key;
		r->value = value;
		r->leftChild = r->rightChild = nullptr;

		// empty tree
		if (root == nullptr) root = r;

		else {
			TNode<k, v>* ptr = root;
			TNode<k, v>* pptr = nullptr;

			while (ptr != nullptr) {
				pptr = ptr;

				if (key == ptr->key) {
					ptr->value = value;
					return;
				}
				else if (key > ptr->key) ptr = ptr->rightChild;
				else ptr = ptr->leftChild;
			}

			if (key > pptr->key) pptr->rightChild = r;
			else if (key < pptr->key) pptr->leftChild = r;
		}
	}

	// search recursively
	v* searchRec(k key) {
		return searchRec(root, key);
	}

	// search iteratively
	v* search(k key) {
		// empty tree
		if (root == nullptr) return nullptr;

		else {
			TNode<k, v>* ptr = root;

			while (ptr != nullptr) {
				if (key == ptr->key) return &ptr->value;
				else if (key > ptr->key) ptr = ptr->rightChild;
				else ptr = ptr->leftChild;
			}
		}
		return nullptr;
	}

	// recursively print keys using inorder traversal
	void inorderPrintKeysRec() const {
		inorderPrintKeysRec(root);
	}

	// iteratively print keys using inorder traversal
	void inorderPrintKeys() const {
		if (root == nullptr) return;		// empty tree

		std::stack<TNode<k, v>*> s;
		TNode<k, v>* temp = root;
		while (!s.empty() || temp != nullptr)
		{
			while (temp != nullptr)
			{
				s.push(temp);
				temp = temp->leftChild;
			}

			temp = s.top();
			s.pop();

			std::cout << temp->key << " ";

			temp = temp->rightChild;

		}
	}

	// return count of total nodes using recursion
	int NoofNodesRec() const {
		int totalNodes = 0;
		NoofNodesRec(root, totalNodes);
		return totalNodes;
	}

	// return count of total nodes iteratively
	// using level order traversal
	int NoofNodes() const {
		if (root == nullptr) return 0;		// empty tree

		std::queue<TNode<k, v>*> q;
		q.push(root);
		int totalNodes = 0;

		while (!q.empty()) {
			TNode<k, v>* temp = q.front();
			q.pop();

			if (temp->leftChild != nullptr) q.push(temp->leftChild);
			if (temp->rightChild != nullptr) q.push(temp->rightChild);

			++totalNodes;
		}
		return totalNodes;
	}

	// return height of tree using recursion
	int heightofBSTRec() const {
		return getHeight(root);
	}

	// return height of tree using iteration
	int heightofBST() const {
		if (root == nullptr) return -1;

		int maxHeight = -1;
		std::queue<TNode<k, v>*> q;
		q.push(root);
		int totalNodes = 0;

		while (!q.empty()) {

			int nodeCount = q.size();

			maxHeight++;

			for (int i = 0; i < nodeCount; ++i) {
				TNode<k, v>* temp = q.front();
				q.pop();

				if (temp->leftChild != nullptr) q.push(temp->leftChild);
				if (temp->rightChild != nullptr) q.push(temp->rightChild);
			}
		}
		return maxHeight;
	}

	TNode<k, v>* getRoot() { return this->root; }

	// destructor
	~BST() {
		deleteAllNodes(root);
	}
};

//int main()
//{
//	BST<int, int> tree; //the key and value both are of type int
//
//	tree.insert(500, 500);
//	tree.insertRec(1000, 1000);
//	tree.insert(1, 1);
//	tree.insert(600, 600);
//	tree.insertRec(700, 700);
//	tree.insert(10, 10);
//	tree.insert(30, 30);
//	tree.insertRec(9000, 9000);
//	tree.insert(50000, 50000);
//	tree.insertRec(20, 20);
//
//
//	cout << "Printing keys using iterative inorder traversal: ";
//	tree.inorderPrintKeys();
//
//	cout << endl << endl << "Printing keys using recursive inorder traversal: ";
//	tree.inorderPrintKeysRec();
//
//	int* val = tree.search(1);
//
//	if (val != nullptr)
//	{
//		cout << "\n1 found" << endl;
//	}
//
//	val = tree.search(123);
//
//	if (val == nullptr)
//	{
//		cout << "123 not found" << endl;
//	}
//
//	cout << endl;
//	cout << tree.NoofNodes() << "\n";  //display number of nodes
//	cout << tree.heightofBST() << "\n"; //display height of BST
//
//	system("pause");
//}