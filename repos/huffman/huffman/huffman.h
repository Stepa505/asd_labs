#ifndef HuffmanTree_hpp
#define HuffmanTree_hpp

#include <fstream>

#include "Set.hpp"

class HuffmanTree {
public:
    class Node;
public:
    HuffmanTree() = default;
    ~HuffmanTree() { clear(m_root); }

    void clear(Node* current);

    std::vector<Node* > getLeafs(Node* root) const;
    Node* root() const { return m_root; }

    void build(const std::string& inputFileName);
    ///returns compression ratio in percent
    int encode(const std::string& inputFileName, std::string& outputFileName);
    bool decode(const std::string& encodedFileName, std::string& decodedFileName);

    void exportTree(const std::string& fileName = "exportedTree.txt") const;
    void importTree(const std::string& fileName = "exportedTree.txt");

    void printHorizontal(Node* root, int marginLeft = 3, int levelSpacing = 5) const;
    void printHorizontalUnicode(Node* root, const std::string& prefix = "", bool isLeft = false) const;

private:
    void doCoding(Node* current, BoolVector currentCode, BoolVector& encoded, char symbol) const;
    void m_getLeafs(Node* root, std::vector<Node* >& leafs) const;

private:
    Node* m_root = nullptr;
};

class HuffmanTree::Node {
public:
    explicit Node(std::string symbols = {}, int frequency = 1)
        : m_data(symbols), m_frequency(frequency)
    {
    }
    explicit Node(Set symbols, int frequency)
        : m_data(symbols), m_frequency(frequency)
    {
    }
    ~Node() = default;

    bool contains(const char symbol) const { return m_data.contains(symbol); }
    void add(const Set& data) { m_data |= Set(data); }

    Set data() const { return m_data; }
    int frequency() const { return m_frequency; }
    Node* left() const { return m_left; }
    Node* right() const { return m_right; }

    void setData(const std::string& data) { m_data = data; }
    void setFrequency(const int value) { m_frequency = value; }
    void setLeft(Node* node) { m_left = node; }
    void setRight(Node* node) { m_right = node; }

    void incrementFrequency() { ++m_frequency; }

private:
    Set m_data = {};
    int m_frequency = 1;
    Node* m_left = nullptr;
    Node* m_right = nullptr;
};

#endif /* HuffmanTree_hpp */
