#include "BinaryTree.h"
#include <iostream>
#include <queue>
#include <string.h>
#include <assert.h>
#include <algorithm>

//Node functions 
BinaryTree::Node::Node(int key, Node* leftChild, Node* rightChild)
    :m_key(key), m_leftChild(leftChild), m_rightChild(rightChild) {
}

int BinaryTree::Node::key() const {
    return m_key;
}
void BinaryTree::Node::setKey(const int& newKey) {
    m_key = newKey;
}

BinaryTree::Node* BinaryTree::Node::leftChild() const {
    return m_leftChild;
}
BinaryTree::Node* BinaryTree::Node::rightChild() const {
    return m_rightChild;
}

void BinaryTree::Node::setLeftChild(Node* newLeftChild) {
    m_leftChild = newLeftChild;
}
void BinaryTree::Node::setRightChild(Node* newRightChild) {
    m_rightChild = newRightChild;
}

//Binary Tree
BinaryTree::BinaryTree(const std::vector<int>& vector) {
    for (int a : vector) add(a);
}
BinaryTree::BinaryTree(const BinaryTree& other) {
    m_root = copy(other.m_root);
}

BinaryTree::BinaryTree(BinaryTree&& other) noexcept {
    std::swap(m_root, other.m_root);
}

BinaryTree::~BinaryTree() {
    clear();
}

void BinaryTree::clear() {
    clearAfter(m_root);
    m_root = nullptr;
}

void BinaryTree::clearAfter(Node* root) {
    if (root) {
        clearAfter(root->leftChild());
        clearAfter(root->rightChild());
        delete root;
    }
}

bool BinaryTree::isEmpty() const {
    return (!m_root);
}

BinaryTree::Node* BinaryTree::root() const {
    return m_root;
}

int BinaryTree::height() const {
    return height(m_root);
}

int BinaryTree::height(const Node* root) const {
    if (!root) return 0;
    bool condition = (height(root->leftChild()) > height(root->rightChild()));
    return (condition ? height(root->leftChild()) : height(root->rightChild())) + 1;
}

int BinaryTree::countOfNodes() const {
    return _countOfNodes(m_root);
}

int BinaryTree::_countOfNodes(Node* root) const {
    if (!root) return 0;
    return 1 + _countOfNodes(root->leftChild()) + _countOfNodes(root->rightChild());
}

std::vector<int> BinaryTree::getVector() const {
    std::vector<int> keys;
    getVector(m_root, keys);
    std::sort(keys.begin(), keys.end());
    return keys;
}

void BinaryTree::getVector(const Node* root, std::vector<int>& keys) const {
    if (!root) return;
    getVector(root->leftChild(), keys);
    keys.push_back(root->key());
    getVector(root->rightChild(), keys);
}

int BinaryTree::min() const {
    std::vector<int> vec = getVector();
    return vec.back();
}

int BinaryTree::max() const {
    std::vector<int> vec = getVector();
    return vec[0];
}

BinaryTree::Node* BinaryTree::add(const int key) {
    if (m_root)
        return add(m_root, key);
    else
        return m_root = new Node(key);
}

BinaryTree::Node* BinaryTree::add(Node* root, int key) {
    if (!root)
        root = new Node(key);
    else if (rand() % 2)
        root->setLeftChild(add(root->leftChild(), key));
    else
        root->setRightChild(add(root->rightChild(), key));

    return root;
}

bool BinaryTree::remove(const int key) {
    Node* node = find(key);
    if (!node) return false;

    if (!node->leftChild() && !node->rightChild()) {
        Node* nodeParent = findParent(node);

        if (nodeParent->leftChild() == node)
            nodeParent->setLeftChild(nullptr);
        else
            nodeParent->setRightChild(nullptr);

        delete node;
    }
    else {
        Node* replacementNode = findReplacementNode(node);
        Node* nodeParent = findParent(replacementNode);

        node->setKey(replacementNode->key());

        if (nodeParent->leftChild() == replacementNode)
            nodeParent->setLeftChild(nullptr);
        else
            nodeParent->setRightChild(nullptr);

        delete replacementNode;
    }

    return true;
}

BinaryTree::Node* BinaryTree::findParent(const Node* node) const {
    if (!node || !m_root || m_root == node) return nullptr;
    return _findParent(m_root, node);
}

BinaryTree::Node* BinaryTree::_findParent(Node* current, const Node* target) const {
    if (!current) return nullptr;

    if (current->leftChild() == target || current->rightChild() == target)
        return current;

    Node* leftResult = _findParent(current->leftChild(), target);
    if (leftResult) return leftResult;

    return _findParent(current->rightChild(), target);
}
BinaryTree::Node* BinaryTree::findReplacementNode(Node* root) const {
    if (!root) return nullptr;

    Node* result = nullptr;

    if ((rand() % 2 && root->leftChild()) || !root->rightChild())
        result = findReplacementNode(root->leftChild());
    else
        result = findReplacementNode(root->rightChild());

    if (!result && !root->leftChild() && !root->rightChild())
        return root;

    return result;
}

BinaryTree::Node* BinaryTree::find(const int key) const {
    return nlrSearch(m_root, key);
}

BinaryTree::Node* BinaryTree::nlrSearch(Node* root, const  int key) const {
    if (!root || root->key() == key)
        return root;

    Node* subTreeSearchResult = nlrSearch(root->leftChild(), key);
    if (!subTreeSearchResult)
        subTreeSearchResult = nlrSearch(root->rightChild(), key);

    return subTreeSearchResult;
}

bool BinaryTree::isBalanced() const {
    return isBalanced(m_root);
}

bool BinaryTree::isBalanced(const Node* root) const {
    if (!root) return true;
    int left = height(root->leftChild());
    int right = height(root->rightChild());
    bool balanceChilds = ((left - right) <= 1) && ((right - left) <= 1);
    return balanceChilds && isBalanced(root->leftChild()) && isBalanced(root->rightChild());
}

int BinaryTree::level(const int key) const {
    return level(m_root, key, 0);
}

int BinaryTree::level(const Node* node, const  int key, const  int level_) const {
    if (!node) return -1;
    if (node->key() == key)
        return level_;

    int indexKey = level(node->leftChild(), key, level_ + 1);
    if (indexKey == -1)
        indexKey = level(node->rightChild(), key, level_ + 1);
    else
        return indexKey;
}

void BinaryTree::printHorizontal() const {
    printHorizontal(m_root, 1, 5);
}

void BinaryTree::printHorizontal(const Node* root, const int marginLeft, const int levelSpacing) const {
    if (!root) return;

    printHorizontal(root->rightChild(), marginLeft + levelSpacing, levelSpacing);
    std::cout << std::string(marginLeft, ' ') << root->key() << std::endl;
    printHorizontal(root->leftChild(), marginLeft + levelSpacing, levelSpacing);
}

void BinaryTree::lrnPrint() const {
    lrnPrint(m_root);
}

void BinaryTree::lrnPrint(const Node* root) const {
    if (!root) return;

    lrnPrint(root->leftChild());
    lrnPrint(root->rightChild());
    std::cout << root->key() << ' ';
}

BinaryTree::Node* BinaryTree::copy(const Node* root) const {
    if (!root) return nullptr;
    return new Node(root->key(), copy(root->leftChild()), copy(root->rightChild()));
}

BinaryTree& BinaryTree::operator = (const BinaryTree& other) {
    if (this != &other) {
        clear();
        m_root = copy(other.m_root);
    }
    return *this;
}

BinaryTree& BinaryTree::operator = (BinaryTree&& other) noexcept {
    if (this != &other)
        std::swap(m_root, other.m_root);
    return *this;
}

//BinaryTreeTester
BinaryTreeTester::BinaryTreeTester(const bool useConsoleOutput, const bool enableAllTests)
    : m_useConsoleOutput(useConsoleOutput)
    , m_addAndCountCheckEnabled(enableAllTests)
    , m_destructorCheckEnabled(enableAllTests)
    , m_removeCheckEnabled(enableAllTests)
    , m_clearCheckEnabled(enableAllTests)
    , m_assignCheckEnabled(enableAllTests)
    , m_heightCheckEnabled(enableAllTests)
{
}

void BinaryTreeTester::test(const int size) {
    m_maxSize = size;

    addAndCount();
    destructor();
    remove();
    clear();
    assign();
    height();

    std::cout << "Test succes";
}

bool BinaryTreeTester::useConsoleOutput() const { return m_useConsoleOutput; }
bool BinaryTreeTester::addAndCountCheckEnabled() const { return m_addAndCountCheckEnabled; }
bool BinaryTreeTester::destructorCheckEnabled() const { return m_destructorCheckEnabled; }
bool BinaryTreeTester::removeCheckEnabled() const { return m_removeCheckEnabled; }
bool BinaryTreeTester::clearCheckEnabled() const { return m_clearCheckEnabled; }
bool BinaryTreeTester::assignCheckEnabled() const { return m_assignCheckEnabled; }
bool BinaryTreeTester::heightCheckEnabled() const { return m_heightCheckEnabled; }

void BinaryTreeTester::setUseConsoleOutput(const bool enabled) { m_useConsoleOutput = enabled; }
void BinaryTreeTester::setAddAndCountCheckEnabled(const bool enabled) { m_addAndCountCheckEnabled = enabled; }
void BinaryTreeTester::setDestructorCheckEnabled(const bool enabled) { m_destructorCheckEnabled = enabled; }
void BinaryTreeTester::setRemoveCheckEnabled(const bool enabled) { m_removeCheckEnabled = enabled; }
void BinaryTreeTester::setClearCheckEnabled(const bool enabled) { m_clearCheckEnabled = enabled; }
void BinaryTreeTester::setAssignCheckEnabled(const bool enabled) { m_assignCheckEnabled = enabled; }
void BinaryTreeTester::setHeightCheckEnabled(const bool enabled) { m_heightCheckEnabled = enabled; }

int BinaryTreeTester::invalidKey() const {
    return -(rand() % m_maxSize + 1);
}

BinaryTree* BinaryTreeTester::allocateTree() {
    return new BinaryTree;
}

void BinaryTreeTester::deallocateTree(BinaryTree* tree) {
    delete tree;
}

void BinaryTreeTester::addAndCount() {
    if (!m_addAndCountCheckEnabled) return;

    BinaryTree* tree = allocateTree();
    check_addAndCount(tree, 0);

    std::vector<int> nodeKeys = generateKeys();
    for (int i = 0; i < m_maxSize; ++i) {
        tree->add(nodeKeys[i]);
        check_addAndCount(tree, i + 1);
    }

    deallocateTree(tree);
}

void BinaryTreeTester::check_addAndCount(const BinaryTree* tree, const int size) {
    assert(tree->countOfNodes() == size);
}

void BinaryTreeTester::destructor() {
    if (!m_destructorCheckEnabled) return;

    const int runsCount = 200;
    for (int i = 0; i < runsCount; i++)
    {
        BinaryTree* tree = allocateTree();
        std::vector<int> nodeKeys = generateKeys();
        for (int i = 0; i < m_maxSize; ++i) {
            tree->add(nodeKeys[i]);
        }
        deallocateTree(tree);
    }
    std::cout << "BinaryTreeTester::destructor ended. Press any key to continue..." << std::endl;
    getchar();

}

/*
 * [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
 * removedNodeIndex = 3
 * nodeKeys[removedNodeIndex] = 3
 *
 * [0, 1, 2, 4, 5, 6, 7, 8, 9]
 * removedNodeIndex = 4
 * nodeKeys[removedNodeIndex] = 5
 *
 * [0, 1, 2, 4, 6, 7, 8, 9]
 *
 */

void BinaryTreeTester::remove() {
    if (!m_removeCheckEnabled) return;

    BinaryTree* tree = allocateTree();
    check_remove(tree, invalidKey(), false, 0);

    std::vector<int> nodeKeys = generateKeys();
    for (int i = 0; i < m_maxSize; ++i)
        tree->add(nodeKeys[i]);

    while (!nodeKeys.empty()) {
        int removedNodeIndex = rand() % nodeKeys.size();
        if (m_useConsoleOutput) {
            tree->printHorizontal();
        }
            std::cout << "\n-----------------------------------------------------\n";
        check_remove(tree, invalidKey(), false, nodeKeys.size());
        check_remove(tree, nodeKeys[removedNodeIndex], true, nodeKeys.size() - 1);
        nodeKeys.erase(nodeKeys.begin() + removedNodeIndex);


    }

    if (m_useConsoleOutput) {
        tree->printHorizontal();
        std::cout << "\n-----------------------------------------------------\n";
    }

    check_remove(tree, invalidKey(), false, nodeKeys.size());
    deallocateTree(tree);
}

void BinaryTreeTester::check_remove(BinaryTree* tree, const int key, const bool result, const int size) {
    assert(tree->remove(key) == result);
    assert(tree->countOfNodes() == size);
}

void BinaryTreeTester::clear() {
    if (!m_clearCheckEnabled) return;

    BinaryTree* tree = allocateTree();
    for (int i = 0; i < 200; i++) {
        std::vector<int> nodeKeys = generateKeys();
        for (int i = 0; i < m_maxSize; ++i) {
            tree->add(nodeKeys[i]);
        }
        tree->clear();
        check_clear(tree, 0);
    }
    deallocateTree(tree);
    std::cout << "BinaryTreeTester::clear ended. Press any key to continue..." << std::endl;
    getchar();
}

void BinaryTreeTester::check_clear(const BinaryTree* tree, const int size) {
    assert(tree->countOfNodes() == size);
}

void BinaryTreeTester::assign() {
    if (!m_assignCheckEnabled) return;

    BinaryTree tree1;

    std::vector<int> nodeKeys = generateKeys();
    for (int i = 0; i < m_maxSize; ++i)
        tree1.add(nodeKeys[i]);

    BinaryTree tree2 = tree1; //Конструктор копирования
    check_assign(&tree1, &tree2);

    tree1 = tree1; //Присваивание самому себе
    check_assign(&tree1, &tree2); //Проверяем, что tree1 не сломалось

    tree1 = tree2; //Присваивание одинаковых по размеру деревьев
    check_assign(&tree1, &tree2);

    BinaryTree tree3;
    tree1 = tree3; //Присваивание дерева меньшего размера
    check_assign(&tree1, &tree3);

    tree3 = tree2; //Присваивание дерева большего размера
    check_assign(&tree2, &tree3);
}

void BinaryTreeTester::check_assign(const BinaryTree* first, const BinaryTree* second) {
    const int size = first->countOfNodes();
    assert(size == second->countOfNodes());

    TreeNodes firstTreeNodes = treeNodes(first);
    TreeNodes secondTreeNodes = treeNodes(second);

    for (int i = 0; i < size; ++i) {
        assert(firstTreeNodes[i] != secondTreeNodes[i]);
        assert(firstTreeNodes[i]->key() == secondTreeNodes[i]->key());
    }
}

BinaryTreeTester::TreeNodes BinaryTreeTester::treeNodes(const BinaryTree* tree) {
    TreeNodes nodes;

    TreeNodes nodesToProcess;
    nodesToProcess.push_back(tree->root());
    while (!nodesToProcess.empty()) {
        BinaryTree::Node* node = nodesToProcess.front();
        if (node != nullptr) {
            nodesToProcess.push_back(node->leftChild());
            nodesToProcess.push_back(node->rightChild());
            nodes.push_back(node);
        }
        nodesToProcess.erase(nodesToProcess.begin());
    }

    return nodes;
}

std::vector<int> BinaryTreeTester::generateKeys() {
    std::vector<int> orderedKeys;
    for (int i = 0; i < m_maxSize; ++i)
        orderedKeys.push_back(i);

    std::vector<int> keys;
    while (!orderedKeys.empty()) {
        int i = rand() % orderedKeys.size();
        keys.push_back(orderedKeys[i]);
        orderedKeys.erase(orderedKeys.begin() + i);
    }

    return keys;
}

void BinaryTreeTester::height() {
    if (!m_heightCheckEnabled) return;

    height_trivialCases();
    height_longOnlyLeftSubtree();
    height_longOnlyRightSubtree();
    height_longOnlyLeftAndRightSubtrees();
    height_longRandomZigzagSubtrees();
}

void BinaryTreeTester::check_height(const BinaryTree& tree, const int height) {
    assert(tree.height() == height);
}

void BinaryTreeTester::height_trivialCases() {
    BinaryTree tree;
    check_height(tree, 0);
    tree.add(0);
    check_height(tree, 1);
}

void BinaryTreeTester::height_longOnlyLeftSubtree() {
    BinaryTree longTree;
    longTree.add(0);
    BinaryTree::Node* runner = longTree.root();
    for (int i = 1; i < m_maxSize; ++i) {
        runner->setLeftChild(new BinaryTree::Node(i));
        runner = runner->leftChild();
        check_height(longTree, i + 1);
    }
}

void BinaryTreeTester::height_longOnlyRightSubtree() {
    BinaryTree longTree;
    longTree.add(0);
    BinaryTree::Node* runner = longTree.root();
    for (int i = 1; i < m_maxSize; ++i) {
        runner->setRightChild(new BinaryTree::Node(i));
        runner = runner->rightChild();
        check_height(longTree, i + 1);
    }
}

void BinaryTreeTester::height_longOnlyLeftAndRightSubtrees() {
    BinaryTree longTree;
    longTree.add(0);
    BinaryTree::Node* leftRunner = longTree.root();
    BinaryTree::Node* rightRunner = longTree.root();
    for (int i = 1; i < m_maxSize / 2; ++i) {
        leftRunner->setLeftChild(new BinaryTree::Node(i));
        leftRunner = leftRunner->leftChild();
        check_height(longTree, i + 1);

        rightRunner->setRightChild(new BinaryTree::Node(i));
        rightRunner = rightRunner->rightChild();
        check_height(longTree, i + 1);
    }
}

void BinaryTreeTester::height_longRandomZigzagSubtrees() {
    BinaryTree longTree;
    longTree.add(0);
    BinaryTree::Node* leftRunner = longTree.root();
    BinaryTree::Node* rightRunner = longTree.root();

    leftRunner->setLeftChild(new BinaryTree::Node(1));
    leftRunner = leftRunner->leftChild();
    rightRunner->setRightChild(new BinaryTree::Node(1));
    rightRunner = rightRunner->rightChild();

    for (int i = 2; i < m_maxSize / 2; ++i) {
        if (rand() % 2 == 0)
        {
            leftRunner->setLeftChild(new BinaryTree::Node(i));
            leftRunner = leftRunner->leftChild();
        }
        else
        {
            leftRunner->setRightChild(new BinaryTree::Node(i));
            leftRunner = leftRunner->rightChild();
        }
        check_height(longTree, i + 1);

        if (rand() % 2 == 0)
        {
            rightRunner->setLeftChild(new BinaryTree::Node(i));
            rightRunner = rightRunner->leftChild();
        }
        else
        {
            rightRunner->setRightChild(new BinaryTree::Node(i));
            rightRunner = rightRunner->rightChild();
        }
        check_height(longTree, i + 1);
    }
}