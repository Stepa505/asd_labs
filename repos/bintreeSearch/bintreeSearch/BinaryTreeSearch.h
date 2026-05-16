#pragma once
#include "BinaryTree.h"
#include <iostream>
#include <vector>
#include <stack>

class BinaryTreeSearch :public BinaryTree {
public:
	BinaryTreeSearch();
	BinaryTreeSearch(const std::vector<int>& vector);
	BinaryTreeSearch(const BinaryTreeSearch& other);
	BinaryTreeSearch(BinaryTreeSearch&& other) noexcept;
	~BinaryTreeSearch() override = default;

	int min() const;
	int max() const;

	using BinaryTree::add;
	/*void add(int key);*/
	bool remove(const int key);

	Node* find(const int key) const;
	int level(const int key) const;
	std::vector<int> getVector() const;

	BinaryTreeSearch& operator = (const BinaryTreeSearch& other);
	BinaryTreeSearch& operator = (BinaryTreeSearch&& other) noexcept;

private:
	int min(Node* root) const;
	int max(Node* root) const;

	Node* add(Node* root, const int key);
	void _remove(Node* node, Node* node2);
	Node* findReplacementNode(Node* root) const;

	Node* find(Node* root, const int key) const;
	int level(const Node* root, const int key, const int level_) const;
};
