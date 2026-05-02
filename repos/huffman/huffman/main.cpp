#include "huffman.h"
#include <iostream>
#include <fstream>

using namespace std;

int main() {
    /*setlocale(LC_ALL, "ru");*/
    string text = "abracadabra";
    ofstream("input.txt", ios::binary) << text;
    cout << "Start: " << text << " (" << text.size() << " bytes)" << endl;
    HuffmanTree huff;
    huff.encode("input.txt", "compressed.bin");
    ifstream comp("compressed.bin", ios::binary | ios::ate);
    cout << "After compressed: " << comp.tellg() << " bytes" << endl;
    comp.close();
    huff.exportTree("freq.txt");
    HuffmanTree decoder;
    decoder.importTree("freq.txt");
    decoder.decode("compressed.bin", "output.txt");
    ifstream out("output.txt", ios::binary);
    string result;
    char ch;
    while (out.get(ch)) result += ch;
    out.close();
    cout << "Unpacking: " << result << endl;
    if (result == text)
        cout << "Succes!\n" << endl;
    else
        cout << "Error, string not compared!\n" << endl;



    return 0;
}