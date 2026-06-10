#include <iostream>
#include <fstream>
#include <string>
#include "huffman.h"

namespace {
    void generateTestFile(const std::string& fileName) {
        std::ofstream file(fileName);
        file << "hello huffman! this is a simple compression test. aaaaabbbccddddeeee";
        file.close();
    }

    std::streamsize fileSize(const std::string& name) {
        std::ifstream f(name, std::ios::binary | std::ios::ate);
        if (!f.is_open())
            return -1;
        return f.tellg();
    }
}

int main() {
    std::string original = "demo.txt";
    std::string encoded = "demo_encoded.txt";
    std::string decoded = "demo_decoded.txt";

    generateTestFile(original);

    // --- Build and encode ---
    HuffmanTree tree;
    tree.build(original);
    std::cout << "Char frequencies:" << std::endl;
    auto leafs = tree.getLeafs(tree.root());
    for (auto& leaf : leafs) {
        char c = leaf->data().min();
        std::cout << "  '" << c << "' : " << leaf->frequency() << std::endl;
    }
    std::cout << "Char set:" << std::endl;
    leafs = tree.getLeafs(tree.root());
    for (auto& leaf : leafs) {
        std::cout << "  '" << leaf->data().min() << "' : ";
        const Set& s = leaf->data();
        for (int i = 0; i < Set::MAX_CARDINALIS; ++i)
            std::cout << (s.contains(static_cast<char>(i)) ? "1" : "0");
        std::cout << std::endl;
    }
    int ratio = tree.encode(original, encoded);
    std::cout << "Compression ratio : " << ratio << "%" << std::endl;

    // --- Decode ---
    bool ok = tree.decode(encoded, decoded);
    std::cout << "Decode status     : " << (ok ? "OK" : "FAILED") << std::endl;

    // --- File size comparison ---
    std::streamsize origSize = fileSize(original);
    std::streamsize encSize = fileSize(encoded);
    std::cout << "\nOriginal size     : " << origSize << " bytes" << std::endl;
    std::cout << "Encoded size      : " << encSize << " bytes" << std::endl;
    std::cout << "Saved             : " << origSize - encSize << " bytes" << std::endl;

    // --- Compare content ---
    std::ifstream fin(original), fdec(decoded);
    std::string src((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
    std::string dst((std::istreambuf_iterator<char>(fdec)), std::istreambuf_iterator<char>());

    std::cout << "\nOriginal          : " << src << std::endl;
    std::cout << "Decoded           : " << dst << std::endl;
    std::cout << "Match             : " << (src == dst ? "YES" : "NO") << std::endl;

    // --- Print tree ---
    std::cout << "\nHuffman tree:" << std::endl;
    tree.printHorizontalUnicode(tree.root());

    // --- Export / import tree ---
    tree.exportTree("tree.txt");
    HuffmanTree tree2;
    tree2.importTree("tree.txt");

    std::string decoded2 = "demo_decoded2.txt";
    std::string encoded2 = "demo_encoded2.txt";
    tree2.encode(original, encoded2);
    tree2.decode(encoded2, decoded2);

    std::ifstream fdec2(decoded2);
    std::string dst2((std::istreambuf_iterator<char>(fdec2)), std::istreambuf_iterator<char>());
    std::cout << "\nExport/import match: " << (src == dst2 ? "YES" : "NO") << std::endl;

    return 0;
}