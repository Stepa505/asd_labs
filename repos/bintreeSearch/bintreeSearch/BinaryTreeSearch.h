#pragma once
#include "../../bintreeSearch/bintree/BinaryTree.h"
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
	void remove(Node* node, Node* node2);
	Node* findReplacementNode(Node* root) const;

	Node* find(Node* root, const int key) const;
	int level(const Node* root, const int key, const int level_) const;
};

class SearchTreeTester : public BinaryTreeTester {
public:
	SearchTreeTester(const bool useConsoleOutput, const bool enableAllTests = true);
	~SearchTreeTester() override = default;

protected:
	BinaryTree* allocateTree() override;
	void check_addAndCount(const BinaryTree* tree, const int size) override;
	void check_remove(BinaryTree* tree, const int key,
		const bool result, const int size) override;
	void check_clear(const BinaryTree* tree, const int size) override;
	void check_assign(const BinaryTree* first,
		const BinaryTree* second) override;

	void assign() override;
private:
	bool isSearchTree(const BinaryTree* tree);
	void treeKeysLnr(BinaryTree::Node* root, std::vector<int>& keys);
};
