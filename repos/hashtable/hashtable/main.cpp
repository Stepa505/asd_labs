#include <iostream>
#include "HashTable.h"

int main()
{
    HashTable<std::string> ht(7);

    ht.insert(1, "one");
    ht.insert(8, "eight");
    ht.insert(15, "fifteen");
    ht.insert(22, "twenty-two");
    ht.insert(2, "two");

    std::cout << "Initial table:\n";
    ht.print();

    std::cout << "\nAfter remove(15):\n";
    ht.remove(15);
    ht.print();

    std::cout << "\nOperator[]: ht[8] = " << ht[8] << "\n";
    ht[30] = "thirty";
    std::cout << "After ht[30] = \"thirty\":\n";
    ht.print();

    std::cout << "\nChecking exists:\n";
    std::cout << "Key 1 exists: " << (ht.exists(1) ? "yes" : "no") << "\n";
    std::cout << "Key 999 exists: " << (ht.exists(999) ? "yes" : "no") << "\n";

    std::cout << "\nSwap test:\n";
    HashTable<std::string> ht2(3);
    ht2.insert(100, "hundred");
    ht2.insert(200, "two hundred");
    std::cout << "Before swap - ht2:\n";
    ht2.print();
    ht.swap(ht2);
    std::cout << "After swap - ht:\n";
    ht.print();

    std::cout << "\nHash functions test:\n";
    int key = 15;
    std::cout << "Testing hash functions for key = " << key << " with N = " << ht.getSize() << ":\n";
    for (int i = 0; i < 3; ++i) {
        std::cout << "h1(" << key << ", " << i << ") = " << ht.getHash1(key, i) << "\n";
        std::cout << "h2(" << key << ", " << i << ") = " << ht.getHash2(key, i) << "\n";
        std::cout << "h3(" << key << ", " << i << ") = " << ht.getHash3(key, i) << "\n";
    }

    return 0;
}