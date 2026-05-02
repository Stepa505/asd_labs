#include "huffman.h"
#include <bitset>
#include <algorithm>

HuffmanTree::Node::Node(unsigned char symbol, int frequency)
    : m_symbol(symbol)
    , m_frequency(frequency)
    , m_left(nullptr)
    , m_right(nullptr) {
}

HuffmanTree::Node::Node(int frequency, Node* left, Node* right)
    : m_symbol(0)
    , m_frequency(frequency)
    , m_left(left)
    , m_right(right) {
}

HuffmanTree::~HuffmanTree() {
    _clear(m_root);
}

void HuffmanTree::_clear(Node* node) {
    if (!node) return;
    _clear(node->getLeft());
    _clear(node->getRight());
    delete node;
}

bool HuffmanTree::CompareNode::operator()(Node* a, Node* b) {
    return a->getFrequency() > b->getFrequency();
}

void HuffmanTree::build(const std::string& inputFilename) {
    _clear(m_root);
    m_root = nullptr;
    m_codes.clear();
    m_frequencies.assign(256, 0);
    std::ifstream inputFile(inputFilename, std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Can't open file: " << inputFilename << std::endl;
        return;
    }
    unsigned char ch;
    while (inputFile.read(reinterpret_cast<char*>(&ch), 1)) {
        m_frequencies[ch]++;
    }
    inputFile.close();
    _buildTree();
}

void HuffmanTree::_buildTree() {
    std::priority_queue<Node*, std::vector<Node*>, CompareNode> pq;
    for (int i = 0; i < 256; ++i) {
        if (m_frequencies[i] > 0) {
            pq.push(new Node(static_cast<unsigned char>(i), m_frequencies[i]));
        }
    }
    if (pq.empty()) {
        m_root = nullptr;
        return;
    }
    if (pq.size() == 1) {
        Node* leaf = pq.top();
        pq.pop();
        m_root = new Node(leaf->getFrequency(), leaf, nullptr);
        _generateCodes(m_root, "");
        return;
    }
    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        Node* parent = new Node(left->getFrequency() + right->getFrequency(), left, right);
        pq.push(parent);
    }

    m_root = pq.top();
    pq.pop();
    _generateCodes(m_root, "");
}

void HuffmanTree::_generateCodes(Node* node, const std::string& code) {
    if (!node) return;
    if (node->isLeaf()) {
        m_codes[node->getSymbol()] = code;
        return;
    }
    _generateCodes(node->getLeft(), code + "0");
    _generateCodes(node->getRight(), code + "1");
}

float HuffmanTree::encode(const std::string& inputFilename, const std::string& outputFilename) {
    if (!m_root) {
        build(inputFilename);
    }
    if (m_codes.empty()) {
        std::cerr << "No symbols to encode" << std::endl;
        return -1;
    }
    std::ifstream inputFile(inputFilename, std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Can't open input file: " << inputFilename << std::endl;
        return -1;
    }
    std::ofstream outputFile(outputFilename, std::ios::binary);
    if (!outputFile.is_open()) {
        std::cerr << "Can't open output file: " << outputFilename << std::endl;
        return -1;
    }
    std::string bitBuffer;
    unsigned long long bytesWritten = 0;
    unsigned long long charsRead = 0;
    unsigned char ch;
    while (inputFile.read(reinterpret_cast<char*>(&ch), 1)) {
        charsRead++;
        const std::string& code = m_codes[ch];
        bitBuffer += code;
        while (bitBuffer.length() >= 8) {
            std::bitset<8> bits(bitBuffer.substr(0, 8));
            unsigned char byte = static_cast<unsigned char>(bits.to_ulong());
            outputFile.write(reinterpret_cast<const char*>(&byte), 1);
            bytesWritten++;
            bitBuffer.erase(0, 8);
        }
    }

    inputFile.close();
    if (!bitBuffer.empty()) {
        int padding = 8 - bitBuffer.length();
        bitBuffer.append(padding, '0');
        std::bitset<8> bits(bitBuffer);
        unsigned char byte = static_cast<unsigned char>(bits.to_ulong());
        outputFile.write(reinterpret_cast<const char*>(&byte), 1);
        bytesWritten++;
        outputFile.close();
        std::string tempFilename = "temp_" + outputFilename;
        std::ofstream tempFile(tempFilename, std::ios::binary);
        std::ifstream encodedFile(outputFilename, std::ios::binary);
        unsigned char paddingInfo = static_cast<unsigned char>(padding);
        tempFile.write(reinterpret_cast<const char*>(&paddingInfo), 1);
        tempFile << encodedFile.rdbuf();

        tempFile.close();
        encodedFile.close();
        std::remove(outputFilename.c_str());
        std::rename(tempFilename.c_str(), outputFilename.c_str());
    }
    else {
        outputFile.close();
        std::string tempFilename = "temp_" + outputFilename;
        std::ofstream tempFile(tempFilename, std::ios::binary);
        std::ifstream encodedFile(outputFilename, std::ios::binary);

        unsigned char paddingInfo = 0;
        tempFile.write(reinterpret_cast<const char*>(&paddingInfo), 1);
        tempFile << encodedFile.rdbuf();

        tempFile.close();
        encodedFile.close();

        std::remove(outputFilename.c_str());
        std::rename(tempFilename.c_str(), outputFilename.c_str());
    }
    float compressionRatio = static_cast<float>(bytesWritten) / static_cast<float>(charsRead) * 100.0f;
    return compressionRatio;
}

bool HuffmanTree::decode(const std::string& encodedFilename, const std::string& decodedFilename) {
    if (!m_root) {
        std::cerr << "Tree is not built. Call build() or importTree() first." << std::endl;
        return false;
    }
    std::ifstream inputFile(encodedFilename, std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Can't open encoded file: " << encodedFilename << std::endl;
        return false;
    }
    unsigned char paddingInfo;
    inputFile.read(reinterpret_cast<char*>(&paddingInfo), 1);
    int paddingBits = paddingInfo;
    std::ofstream outputFile(decodedFilename, std::ios::binary);
    if (!outputFile.is_open()) {
        std::cerr << "Can't create decoded file: " << decodedFilename << std::endl;
        return false;
    }
    std::string bits;
    unsigned char byte;
    while (inputFile.read(reinterpret_cast<char*>(&byte), 1)) {
        std::bitset<8> bitset(byte);
        bits += bitset.to_string();
    }
    inputFile.close();
    if (paddingBits > 0) {
        bits.erase(bits.length() - paddingBits, paddingBits);
    }
    Node* currentNode = m_root;
    if (m_root->isLeaf()) {
        unsigned char symbol = m_root->getSymbol();
        for (size_t i = 0; i < bits.length(); ++i) {
            outputFile.write(reinterpret_cast<const char*>(&symbol), 1);
        }
        outputFile.close();
        return true;
    }
    for (char bit : bits) {
        if (bit == '0') {
            currentNode = currentNode->getLeft();
        }
        else {
            currentNode = currentNode->getRight();
        }
        if (currentNode->isLeaf()) {
            unsigned char symbol = currentNode->getSymbol();
            outputFile.write(reinterpret_cast<const char*>(&symbol), 1);
            currentNode = m_root; 
        }
    }

    outputFile.close();
    return true;
}

void HuffmanTree::exportTree(const std::string& filename) {
    if (m_frequencies.empty()) {
        std::cerr << "No frequency data to export" << std::endl;
        return;
    }

    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Can't create file: " << filename << std::endl;
        return;
    }
    for (int freq : m_frequencies) {
        file << freq << " ";
    }
    file.close();
}

void HuffmanTree::importTree(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Can't open file: " << filename << std::endl;
        return;
    }
    _clear(m_root);
    m_root = nullptr;
    m_codes.clear();
    m_frequencies.resize(256);
    for (int i = 0; i < 256; ++i) {
        file >> m_frequencies[i];
    }
    file.close();
    _buildTree();
}

void HuffmanTree::printHorizontal(int levelSpacing) const {
    _printHorizontal(m_root, 0, levelSpacing);
}

void HuffmanTree::_printHorizontal(Node* node, int marginLeft, int levelSpacing) const {
    if (!node) return;
    _printHorizontal(node->getRight(), marginLeft + levelSpacing, levelSpacing);
    if (node->isLeaf()) {
        std::cout << std::string(marginLeft, ' ')
            << "[" << node->getSymbol() << ":" << node->getFrequency() << "]"
            << std::endl;
    }
    else {
        std::cout << std::string(marginLeft, ' ')
            << "[" << node->getFrequency() << "]"
            << std::endl;
    }
    _printHorizontal(node->getLeft(), marginLeft + levelSpacing, levelSpacing);
}