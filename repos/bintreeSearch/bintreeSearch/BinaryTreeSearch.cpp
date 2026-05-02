#include "BinaryTreeSearch.h"
#include <iostream>
#include <algorithm>  
#include <assert.h>

BinaryTreeSearch::BinaryTreeSearch() : BinaryTree() {}

BinaryTreeSearch::BinaryTreeSearch(const std::vector<int>& vector) {
	for (int a : vector) add(a);
}

BinaryTreeSearch::BinaryTreeSearch(const BinaryTreeSearch& other)
	: BinaryTree(other) {
}

BinaryTreeSearch::BinaryTreeSearch(BinaryTreeSearch&& other) noexcept
	: BinaryTree(other) {
}

int BinaryTreeSearch::min() const {
	return min(root());
}

int BinaryTreeSearch::min(Node* root) const {
	Node* node = root;

	if (!node) return INT32_MIN;

	while (node->leftChild())
		node = node->leftChild();

	return node->key();
}

int BinaryTreeSearch::max() const {
	return max(root());
}

int BinaryTreeSearch::max(Node* root) const {
	Node* node = root;

	if (!node) return INT32_MAX;

	while (node->rightChild())
		node = node->rightChild();

	return node->key();
}

BinaryTree::Node* BinaryTreeSearch::add(Node* root, const int key) {
	if (!root)
		root = new Node(key);
	else if (key < root->key())
		root->setLeftChild(add(root->leftChild(), key));
	else if (key > root->key())
		root->setRightChild(add(root->rightChild(), key));

	return root;
}

bool BinaryTreeSearch::remove(const int key) {
	Node* node = find(key);
	if (!node) return false;

	if (!node->leftChild() && !node->rightChild())
		remove(node, nullptr);
	else if (!node->leftChild() || !node->rightChild()) {
		Node* replacementNode = (node->leftChild()) ? node->leftChild() : node->rightChild();
		remove(node, replacementNode);
	}
	else {
		Node* replacementNode = findReplacementNode(node);
		Node* nodeParent = findParent(replacementNode);

		node->setKey(replacementNode->key());

		if (!replacementNode->leftChild() && !replacementNode->rightChild())
			remove(replacementNode, nullptr);
		else if (!replacementNode->leftChild() || !replacementNode->rightChild()) {
			Node* replacementNode2 = (replacementNode->leftChild()) ? replacementNode->leftChild() : replacementNode->rightChild();
			remove(replacementNode, replacementNode2);
		}
	}

	return true;
}

void BinaryTreeSearch::remove(Node* node, Node* node2) {
	Node* nodeParent = findParent(node);

	if (nodeParent->leftChild() == node)
		nodeParent->setLeftChild(node2);
	else
		nodeParent->setRightChild(node2);

	delete node;
}

BinaryTree::Node* BinaryTreeSearch::findReplacementNode(Node* root) const {
	if (!root) return nullptr;

	if (root->leftChild()) {
		Node* maxInLeft = root->leftChild();
		int result = max(maxInLeft);
		if (result != INT32_MAX)
			return maxInLeft;
	}
	else if (root->rightChild()) {
		Node* minInRight = root->rightChild();
		int result = min(minInRight);
		if (result != INT32_MIN)
			return minInRight;
	}

	return nullptr;
}

BinaryTree::Node* BinaryTreeSearch::find(const int key) const {
	return find(root(), key);
}

BinaryTree::Node* BinaryTreeSearch::find(Node* root, const int key) const {
	if (!root || root->key() == key)
		return root;

	if (key < root->key())
		return find(root->leftChild(), key);
	else
		return find(root->rightChild(), key);
}

int BinaryTreeSearch::level(const int key) const {
	return level(root(), key, 0);
}

int BinaryTreeSearch::level(const Node* node, const int key, const int level_) const {
	if (!node) return -1;
	if (node->key() == key)
		return level_;

	if (key < node->key())
		return level(node->leftChild(), key, level_ + 1);
	else
		return level(node->rightChild(), key, level_ + 1);
}

std::vector<int> BinaryTreeSearch::getVector() const {
	return BinaryTree::getVector();
}

BinaryTreeSearch& BinaryTreeSearch::operator = (const BinaryTreeSearch& other) {
	BinaryTree::operator=(other);
	return *this;
}

BinaryTreeSearch& BinaryTreeSearch::operator = (BinaryTreeSearch&& other) noexcept {
	BinaryTree::operator=(other);
	return *this;
}

//SearchTreeTester
SearchTreeTester::SearchTreeTester(const bool useConsoleOutput, const bool enableAllTests)
	: BinaryTreeTester(useConsoleOutput, enableAllTests)
{
}

BinaryTree* SearchTreeTester::allocateTree() {
	return new BinaryTreeSearch;
}

void SearchTreeTester::check_addAndCount(const BinaryTree* tree, const int size) {
	BinaryTreeTester::check_addAndCount(tree, size);
	assert(isSearchTree(tree));
}

void SearchTreeTester::check_remove(BinaryTree* tree, const int key, const bool result, const int size) {
	BinaryTreeTester::check_remove(tree, key, result, size);
	assert(isSearchTree(tree));
}

void SearchTreeTester::check_clear(const BinaryTree* tree, const int size) {
	BinaryTreeTester::check_clear(tree, size);
	assert(isSearchTree(tree));
}

void SearchTreeTester::check_assign(const BinaryTree* first, const BinaryTree* second) {
	BinaryTreeTester::check_assign(first, second);
	assert(isSearchTree(first));
	assert(isSearchTree(second));
}


void SearchTreeTester::assign() {
	if (!assignCheckEnabled()) return;

	BinaryTreeSearch tree1;

	std::vector<int> nodeKeys = generateKeys();
	for (int i = 0; i < m_maxSize; ++i)
		tree1.add(nodeKeys[i]);

	BinaryTreeSearch tree2 = tree1; //Конструктор копирования
	check_assign(&tree1, &tree2);

	tree1 = tree1; //Присваивание самому себе
	check_assign(&tree1, &tree2); //Проверяем, что tree1 не сломалось

	tree1 = tree2; //Присваивание одинаковых по размеру деревьев
	check_assign(&tree1, &tree2);

	BinaryTreeSearch tree3;
	tree1 = tree3; //Присваивание дерева меньшего размера
	check_assign(&tree1, &tree3);

	tree3 = tree2; //Присваивание дерева большего размера
	check_assign(&tree2, &tree3);
}

bool SearchTreeTester::isSearchTree(const BinaryTree* tree) {
	if (useConsoleOutput()) {
		tree->printHorizontal();
		std::cout << "======\n";
	}

	std::vector<int> keys;
	treeKeysLnr(tree->root(), keys);
	return std::is_sorted(keys.begin(), keys.end());
}

void SearchTreeTester::treeKeysLnr(BinaryTree::Node* root, std::vector<int>& keys) {
	if (!root) return;

	treeKeysLnr(root->leftChild(), keys);
	keys.push_back(root->key());
	treeKeysLnr(root->rightChild(), keys);
}