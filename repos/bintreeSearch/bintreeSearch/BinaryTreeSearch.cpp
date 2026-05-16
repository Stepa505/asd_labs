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
		_remove(node, nullptr);
	else if (!node->leftChild() || !node->rightChild()) {
		Node* replacementNode = (node->leftChild()) ? node->leftChild() : node->rightChild();
		_remove(node, replacementNode);
	}
	else {
		Node* replacementNode = findReplacementNode(node);
		Node* nodeParent = findParent(replacementNode);

		node->setKey(replacementNode->key());

		if (!replacementNode->leftChild() && !replacementNode->rightChild())
			_remove(replacementNode, nullptr);
		else if (!replacementNode->leftChild() || !replacementNode->rightChild()) {
			Node* replacementNode2 = (replacementNode->leftChild()) ? replacementNode->leftChild() : replacementNode->rightChild();
			_remove(replacementNode, replacementNode2);
		}
	}

	return true;
}

void BinaryTreeSearch::_remove(Node* node, Node* node2) {
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
