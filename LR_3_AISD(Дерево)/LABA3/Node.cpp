#include <stdio.h>
#include <tchar.h>
#include <stack>
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
using namespace std;

template <typename T>
struct Node {
	T key;
	Node<T>* leftNode;
	Node<T>* rightNode;

	Node(T key) {
		this->key = key;
		leftNode = nullptr;
		rightNode = nullptr;
	}
};

template <typename T>
class BinarySearchTree {
private:
	Node<T>* root_;
	void addNode(Node<T>*& node, const T& key);
	Node<T>* removeNode(Node<T>*& node, const T& key);
	Node<T>* findMin(Node<T>* node);
	Node<T>* findNode(const T& key) const;
	void deleteTree(Node<T>* node);
	void inorderPrint(Node<T>* node, unsigned int level);
	int findVertex(Node<T>* node, int k, int& idx);
	int countOfChildren(Node<T>* node);
	unsigned int Height(const Node<T>* node) const;


public:
	BinarySearchTree();
	~BinarySearchTree();

	T key() const;

	Node<T>* leftNode() const;
	Node<T>* rightNode() const;
	Node<T>* root() const;

	void remove(const T& value);
	bool isFound(const T& value) const;
	void paintTree();
	unsigned int getHeight();
	int fileIn(string filename);
	void find(int k);
	void count();
};

template<typename T>
void BinarySearchTree<T>::addNode(Node<T>*& node, const T& key)
{
	if (node) {
		if (key < node->key) {
			addNode(node->leftNode, key);
		}

		else if (key >= node->key) {
			addNode(node->rightNode, key);
		}

		else {
			return;
		}
	}
	else {
		node = new Node<T>(key);
	}
}

template<typename T>
Node<T>* BinarySearchTree<T>::removeNode(Node<T>*& node, const T& key)
{
	if (node == NULL)
		return node;
	if (key < node->key)
		node->leftNode = removeNode(node->leftNode, key);
	else if (key > node->key)
		node->rightNode = removeNode(node->rightNode, key);
	else {
		if ((node->leftNode == NULL) || node->rightNode == NULL) {
			Node<T>* tmp = node->leftNode ? node->leftNode :
				node->rightNode;
			if (tmp == NULL) {
				tmp = node;
				node = NULL;
			}
			else *node = *tmp;
			delete tmp;
		}
		else {
			Node<T>* tmp = findMin(node->rightNode);
			node->key = tmp->key;
			node->rightNode = removeNode(node->rightNode, tmp->key);
		}
	}
	return node;
}

template<typename T>
Node<T>* BinarySearchTree<T>::findMin(Node<T>* node)
{
	if (node->leftNode != NULL)
		findMin(node->leftNode);
	else return node;
}

template<typename T>
Node<T>* BinarySearchTree<T>::findNode(const T& key) const
{
	Node<T>* currentNode = root_;

	while (currentNode) {
		if (currentNode->key == key) break;
		else {
			if (currentNode->key < key) currentNode = currentNode->rightNode;
			else currentNode = currentNode->leftNode;
		}
	}

	return currentNode;
}

template<typename T>
void BinarySearchTree<T>::deleteTree(Node<T>* node)
{
	if (!node) return;
	deleteTree(node->leftNode);
	deleteTree(node->rightNode);
}

template<typename T>
int BinarySearchTree<T>::countOfChildren(Node<T>* node)
{
	Node<T>* current = node;
	if (!current) return 0;

	int left = 0;
	int right = 0;

	left += countOfChildren(current->leftNode);
	right += countOfChildren(current->rightNode);

	if (abs(left - right) == 1) {
		cout << current->key << " ";
	}
	return left + right + 1;
}


template<typename T>
int BinarySearchTree<T>::findVertex(Node<T>* node, int k, int& idx)
{
	if (!node) return 0;
	findVertex(node->leftNode, k, idx);
	if (idx == k && node->leftNode == NULL && node->rightNode == NULL) {
		cout << "Лист:" << node->key << endl;
	}

	if (node->leftNode == NULL && node->rightNode == NULL) {
		idx++;
	}

	findVertex(node->rightNode, k, idx);
}

template<typename T>
void BinarySearchTree<T>::inorderPrint(Node<T>* node, unsigned int level)
{
	if (!node) { return; }

	level++;
	inorderPrint(node->rightNode, level);

	for (unsigned int i = 0; i < level; ++i) {
		cout << ">";
	}
	cout << node->key << endl;

	inorderPrint(node->leftNode, level);
	level--;
}

template<typename T>
BinarySearchTree<T>::BinarySearchTree()
{
	root_ = nullptr;
}

template<typename T>
BinarySearchTree<T>::~BinarySearchTree()
{
	deleteTree(root_);
}

template<typename T>
T BinarySearchTree<T>::key() const
{
	return root_->key;
}

template<typename T>
unsigned int BinarySearchTree<T>::Height(const Node<T>* node) const
{
	int level = 1;
	int level_other = 0;
	int max_level = 0;

	bool flag = true;

	stack <Node<T>*> steck;
	stack <int> levels;

	if (!node) {
		return 0;
	}

	while (node) {

		if ((node->leftNode) && (node->rightNode)) {
			level++;
			steck.push(node->rightNode);
			levels.push(level);
			node = node->leftNode;
		}
		else if ((node->leftNode) || (node->rightNode)) {
			node = node->leftNode ? node->leftNode : node->rightNode;
			level++;
		}
		else {

			if (steck.empty())
				break;
			else if (steck.size() == 1 && flag) {

				level_other = level;
				level = 1;
				flag = false;
			}
			node = steck.top();
			if (level > max_level)
				max_level = level;
			level = levels.top();
			steck.pop();
		}
	}
	return max(max_level, level_other);
}

template<typename T>
Node<T>* BinarySearchTree<T>::leftNode() const
{
	return root_->leftNode;
}

template<typename T>
Node<T>* BinarySearchTree<T>::rightNode() const
{
	return root_->rightNode;
}

template<typename T>
Node<T>* BinarySearchTree<T>::root() const
{
	return root_;
}

template<typename T>
void BinarySearchTree<T>::count()
{
	countOfChildren(root_);
}

template<typename T>
void BinarySearchTree<T>::find(int k)
{
	int idx = 1;

	findVertex(root_, k, idx);
}

template<typename T>
void BinarySearchTree<T>::remove(const T& key)
{
	if (isFound(key)) {
		removeNode(root_, key);
		return;
	}
	else {
		cout << key << " отсутствует в дереве\n";
		return;
	}
}

template<typename T>
bool BinarySearchTree<T>::isFound(const T& key) const
{
	Node<T>* retNode = findNode(key);
	if (retNode)
		return true;
	else
		return false;
}

template<typename T>
void BinarySearchTree<T>::paintTree()
{
	unsigned int level = 0;
	inorderPrint(root_, level);
}

template<typename T>
unsigned int BinarySearchTree<T>::getHeight()
{
	return Height(root_);
}

template<typename T>
int BinarySearchTree<T>::fileIn(string filename)
{
	ifstream inFile;
	inFile.open(filename);
	if (!inFile) {
		return 1;
	}
	T key;
	unsigned int count;
	inFile >> count;
	while (count--) {
		inFile >> key;
		addNode(root_, key);
	}
	inFile.close();
	return 0;
}
