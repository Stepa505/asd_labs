#pragma once
#include <iostream>
#include <vector>
#include <list>

class BinaryTree {
public:
	class Node;
public:
	BinaryTree() = default;
	BinaryTree(const std::vector<int>& vector);
	BinaryTree(const BinaryTree& other);
	BinaryTree(BinaryTree&& other) noexcept;
	virtual ~BinaryTree();

	void clear();
	void clearAfter(Node* root);

	bool isEmpty() const;

	Node* root() const;
	int height() const;
	int countOfNodes() const;

	std::vector<int> getVector() const;

	virtual int min() const;
	virtual int max() const;

	Node* add(const int key);
	virtual bool remove(const int key);

	virtual Node* find(const int key) const;
	virtual int level(const int key) const;

	bool isBalanced() const;

	void printHorizontal() const;
	void lrnPrint() const;

	BinaryTree& operator = (const BinaryTree& other);
	BinaryTree& operator = (BinaryTree&& other) noexcept;

protected:
	Node* copy(const Node* root) const;

	Node* findParent(const Node* root) const;
	Node* _findParent(Node* current, const Node* target) const;
	virtual Node* findReplacementNode(Node* root) const;

	int height(const Node* root) const;
	void getVector(const Node* node, std::vector<int>& keys) const;

	virtual Node* add(Node* root, const  int key);
	Node* nlrSearch(Node* root, const  int key) const;

	bool isBalanced(const Node* root) const;
	int level(const Node* root, const int key, const int level_) const;

	void printHorizontal(const Node* root, const int marginLeft, const int levelSpacing) const;
	void lrnPrint(const Node* root) const;

private:
	Node* m_root = nullptr;
};

class BinaryTree::Node {
public:
	Node(int key = 0, Node* leftChild = nullptr, Node* rightChild = nullptr);
	~Node() = default;

	int key() const;
	void setKey(const int& newKey);

	Node* leftChild() const;
	Node* rightChild() const;

	void setLeftChild(Node* newLeftChild);
	void setRightChild(Node* newRightChild);

private:
	int m_key;
	Node* m_leftChild;
	Node* m_rightChild;
};

class BinaryTreeTester {
public:
	BinaryTreeTester(const bool useConsoleOutput, const bool enableAllTests = true);
	virtual ~BinaryTreeTester() = default;

	void test(const int size);

	bool useConsoleOutput() const;
	bool addAndCountCheckEnabled() const;
	bool destructorCheckEnabled() const;
	bool removeCheckEnabled() const;
	bool clearCheckEnabled() const;
	bool assignCheckEnabled() const;
	bool heightCheckEnabled() const;

	void setUseConsoleOutput(const bool enabled);
	void setAddAndCountCheckEnabled(const bool enabled);
	void setDestructorCheckEnabled(const bool enabled);
	void setRemoveCheckEnabled(const bool enabled);
	void setClearCheckEnabled(const bool enabled);
	void setAssignCheckEnabled(const bool enabled);
	void setHeightCheckEnabled(const bool enabled);

protected:
	using TreeNodes = std::vector<BinaryTree::Node*>;

protected:
	int invalidKey() const;

	virtual BinaryTree* allocateTree();
	virtual void check_addAndCount(const BinaryTree* tree, const int size);
	virtual void check_remove(BinaryTree* tree, const int key, const bool result, const int size);
	virtual void check_clear(const BinaryTree* tree, const int size);
	virtual void check_assign(const BinaryTree* first, const BinaryTree* second);
	virtual void check_height(const BinaryTree& tree, const int size);

protected:
	virtual void assign();
	std::vector<int> generateKeys();
	int m_maxSize;

private:
	void deallocateTree(BinaryTree* tree);
	void addAndCount();
	void destructor();
	void remove();
	void clear();
	void height();
	void height_trivialCases();
	void height_longOnlyLeftSubtree();
	void height_longOnlyRightSubtree();
	void height_longOnlyLeftAndRightSubtrees();
	void height_longRandomZigzagSubtrees();
	TreeNodes treeNodes(const BinaryTree* tree);

private:
	bool m_useConsoleOutput;
	bool m_addAndCountCheckEnabled;
	bool m_destructorCheckEnabled;
	bool m_removeCheckEnabled;
	bool m_clearCheckEnabled;
	bool m_assignCheckEnabled;
	bool m_heightCheckEnabled;
};