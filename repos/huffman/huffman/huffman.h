#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <fstream>
#include <iostream>

class HuffmanTree {
public:
    class Node;
public:
    HuffmanTree() = default;
    ~HuffmanTree();

    void build(const std::string& inputFilename);

    float encode(const std::string& inputFilename, const std::string& outputFilename);

    bool decode(const std::string& encodedFilename, const std::string& decodedFilename);

    void exportTree(const std::string& filename);
    void importTree(const std::string& filename);

    void printHorizontal(int levelSpacing = 4) const;

private:
    void _buildTree();
    void _generateCodes(Node* node, const std::string& code);
    void _clear(Node* node);
    void _printHorizontal(Node* node, int marginLeft, int levelSpacing) const;
    struct CompareNode {
        bool operator()(Node* a, Node* b);
    };

private:
    Node* m_root = nullptr;
    std::vector<int> m_frequencies;
    std::unordered_map<unsigned char, std::string> m_codes;
};

class HuffmanTree::Node {
public:
    Node(unsigned char symbol, int frequency);
    Node(int frequency, Node* left, Node* right);

    ~Node() = default;

    bool isLeaf() const { return m_left == nullptr && m_right == nullptr; }

    unsigned char getSymbol() const { return m_symbol; }
    int getFrequency() const { return m_frequency; }
    Node* getLeft() const { return m_left; }
    Node* getRight() const { return m_right; }

    void setLeft(Node* left) { m_left = left; }
    void setRight(Node* right) { m_right = right; }

private:
    unsigned char m_symbol = 0;
    int m_frequency = 0;
    Node* m_left = nullptr;
    Node* m_right = nullptr;
};