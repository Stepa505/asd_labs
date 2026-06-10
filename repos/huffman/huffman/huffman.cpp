#include <list>
#include <assert.h>
#include <algorithm>

#include "huffman.h"

namespace {
    void pushBack(std::list<HuffmanTree::Node*>& nodes, const char value) {
        if (!nodes.empty()) {
            for (auto it = nodes.begin(); it != nodes.end(); ++it) {
                if ((*it)->contains(value)) {
                    (*it)->incrementFrequency();
                    return;
                }
            }
        }

        nodes.push_back(new HuffmanTree::Node(std::string(1, value)));
        return;
    }

    auto min(const std::list<HuffmanTree::Node*>::iterator& start, const std::list<HuffmanTree::Node*>::iterator& stop) {
        auto min = start;
        for (auto it = start; it != stop; ++it) {
            if ((*it)->frequency() < (*min)->frequency()) {
                min = it;
            }
        }

        return min;
    }
    void sort(std::list<HuffmanTree::Node*>& nodes) { 
        for (auto it = nodes.begin(); it != nodes.end(); ++it) {
            std::swap(*it, *min(it, nodes.end()));
        }
    }

    void add(std::list<HuffmanTree::Node*>& nodes, HuffmanTree::Node* node) {
        if (nodes.empty() || node->frequency() <= nodes.front()->frequency()) {
            nodes.push_front(node);
            return;
        }

        if (node->frequency() >= nodes.back()->frequency()) {
            nodes.push_back(node);
            return;
        }

        for (auto it = nodes.begin(); it != nodes.end(); ++it) {
            if (node->frequency() <= (*it)->frequency()) {
                nodes.insert(it, node);
                return;
            }
        }

        nodes.push_back(node);
    }

    void print(const std::list<HuffmanTree::Node*>& nodes) {
        std::cout << '[';
        for (auto it = nodes.begin(); it != nodes.end();) {
            std::cout << (*it)->data() << (++it == nodes.end()
                ? ""
                : ", ");
        }
        std::cout << ']' << std::endl;
    }

    void addToFile(std::ofstream* file, const Set& set) {
        for (int i = 0; i < Set::MAX_CARDINALIS; ++i) {
            if (set.contains(i)) {
                (*file) << static_cast<char>(i);
            }
        }

    }

    int bytes(const std::string& fileName) {
        std::ifstream file(fileName);
        int result = 0;

        while (!file.eof()) {
            file.get();
            ++result;
        }
        --result;

        file.close();
        return result;
    }
};

void HuffmanTree::clear(Node* current) {
    if (!current) {
        return;
    }

    clear(current->left());
    clear(current->right());
    delete current;
}

std::vector<HuffmanTree::Node* > HuffmanTree::getLeafs(Node* root) const {
    if (!root) {
        return {};
    }

    std::vector<Node* > leafs;

    m_getLeafs(root, leafs);

    return leafs;
}


void HuffmanTree::build(const std::string& inputFileName) {
    std::list<Node*> nodes;
    std::ifstream input(inputFileName, std::ios_base::binary);
    if (!input.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return;
    }

    char buffer = {};
    while (input >> std::noskipws >> buffer) {
        pushBack(nodes, buffer);
    }
    input.close();

    if (nodes.empty()) {
        std::cerr << "File is empty" << std::endl;
        return;
    }

    sort(nodes);

    int currentSize = static_cast<int>(nodes.size());
    Node* parent = nullptr;
    while (currentSize > 1) {
        auto first = nodes.front(); nodes.pop_front();
        auto second = nodes.front(); nodes.pop_front();

        parent = new Node(first->data() | second->data(),
            first->frequency() + second->frequency());
        parent->setLeft(first);
        parent->setRight(second);
        add(nodes, parent);

        currentSize--;
    }
    m_root = nodes.front();

    input.close();
}

int HuffmanTree::encode(const std::string& inputFileName, std::string& outputFileName) {
    if (!m_root) {
        build(inputFileName);
        if (!m_root) {
            return -1;
        }
    }

    std::ofstream output(outputFileName);

    BoolVector encoded(0, 0);

    std::ifstream input(inputFileName);

    char current = {};
    while (input >> std::noskipws >> current) {
        doCoding(m_root, BoolVector(0, 0), encoded, current);
    }
    uint8_t* coded = encoded.cells();
    output << encoded.unsignificantRankCount();
    int bytes = 0;
    for (int i = 0; i < encoded.cellCount(); ++i) {
        output << coded[i];
        ++bytes;
    }

    output.close();
    return static_cast<float>(static_cast<float>(bytes) - 1.0 - ((1.0 - static_cast<float>(encoded.unsignificantRankCount())) / 8)) / static_cast<float>(m_root->frequency()) * 100;
}

bool HuffmanTree::decode(const std::string& inputFileName, std::string& outpuFileName) {
    if (!m_root) {
        return false;
    }

    if (inputFileName.empty()) {
        return false;
    }

    std::ifstream input(inputFileName);
    if (!input.is_open()) {
        return false;
    }

    BoolVector code(0, 0);
    int unsignificantRankCount = 0;
    input >> unsignificantRankCount;

    {
        char current;
        while (input >> std::noskipws >> current) {
            code.add(current);
        }
    }
    code.setUnsignificantRankCount(unsignificantRankCount);

    std::ofstream* output = new std::ofstream(outpuFileName);
    if (!output->is_open()) {
        return false;
    }

    int pos = 0;
    Node* current = m_root;
    if (!current->left() && !current->right()) {
        for (int i = 0; i < m_root->frequency(); ++i) {
            addToFile(output, current->data());
        }
    }
    while (pos <= code.lenth()) {
        if (!current->left() && !current->right()) {
            addToFile(output, current->data());
            current = m_root;
        }

        if (pos == code.lenth())
            break;
        if (code[pos] == false)
            current = current->left();
        else
            current = current->right();
        ++pos;
    }

    output->close();
    delete output;
    input.close();
    return true;
}

void HuffmanTree::exportTree(const std::string& fileName) const {
    auto leafs = getLeafs(m_root);

    std::ofstream output(fileName);
    if (!output.is_open()) {
        return;
    }

    Node* current = m_root;
    for (int i = 0; i < Set::MAX_CARDINALIS; ++i) {
        if (m_root->data().contains(static_cast<char>(i))) {
            while (current->data() != static_cast<char>(i)) {
                current = (current->left()->data().contains(static_cast<char>(i))
                    ? current->left()
                    : current->right());
            }
            output << current->frequency();
            current = m_root;
        }
        else {
            output << 0;
        }
    }

    output.close();
}

void HuffmanTree::importTree(const std::string& fileName) {

    std::ifstream input(fileName);
    std::string supportFileName = "support_file.txt";
    std::ofstream support(supportFileName);
    if (!input.is_open() || !support.is_open()) {
        return;
    }

    char current = {};
    int corrected = {};
    int i = 0;
    while (input >> current) {
        corrected = (int)current - 48;
        while (corrected) {
            support << static_cast<char>(i);
            --corrected;
        }
        ++i;
    }
    support.close();
    this->build(supportFileName);

    input.close();
    remove(supportFileName.c_str());
}

void HuffmanTree::printHorizontal(Node* root, int marginLeft, int levelSpacing) const {

    if (root == nullptr) {
        return;
    }

    printHorizontal(root->right(), marginLeft + levelSpacing, levelSpacing);
    std::cout << std::string(marginLeft, ' ') << root->data() << std::endl;
    printHorizontal(root->left(), marginLeft + levelSpacing, levelSpacing);
}

void HuffmanTree::printHorizontalUnicode(Node* root, const std::string& prefix, bool isLeft) const {
    if (!root) {
        return;
    }

    std::cout << prefix;

    std::cout << (isLeft ? "├──" : "└──");

    std::cout << root->data() << std::endl;

    printHorizontalUnicode(root->left(), prefix + (isLeft ? "│   " : "    "), true);
    printHorizontalUnicode(root->right(), prefix + (isLeft ? "│   " : "    "), false);
}

void HuffmanTree::doCoding(Node* current, BoolVector currentCode, BoolVector& encoded, char symbol) const {
    if (!current) {
        return;
    }

    if (!current->left() && !current->right()) {
        encoded += currentCode;
    }
    if (current->left() && current->left()->contains(symbol)) {
        doCoding(current->left(), currentCode + false, encoded, symbol);
    }
    else if (current->right() && current->right()->contains(symbol)) {
        doCoding(current->right(), currentCode + true, encoded, symbol);
    }
}

void HuffmanTree::m_getLeafs(Node* root, std::vector<Node* >& leafs) const {
    if (!root) {
        return;
    }
    else if (!root->left() && !root->right()) {
        leafs.push_back(root);
    }
    else {
        m_getLeafs(root->left(), leafs);
        m_getLeafs(root->right(), leafs);
    }
}
