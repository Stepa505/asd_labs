#include <iostream>
#include <string>
#include "HashTable.h"

void demonstrateHashFunction(IHashFunction* func, const std::string& funcName) {
    std::cout << "  Hash Function: " << funcName << "\n";

    HashTable<std::string> table(7, func);

    // 1. Adding elements
    std::cout << "\n1. Adding elements:\n";
    table.add(10, "ten");
    table.add(20, "twenty");
    table.add(30, "thirty");
    table.add(41, "forty one");
    table.add(52, "fifty two");
    table.add(63, "sixty three");
    table.add(74, "seventy four");
    std::cout << "   Added 7 elements (keys: 10, 20, 30, 41, 52, 63, 74)\n";

    // 2. Table contents
    std::cout << "\n2. Table contents (distribution across buckets):\n";
    table.print();

    // 3. Search
    std::cout << "\n3. Searching:\n";
    std::cout << "   Contains key 20? " << (table.contains(20) ? "yes" : "no") << "\n";
    std::cout << "   Contains key 99? " << (table.contains(99) ? "yes" : "no") << "\n";

    // 4. operator[]
    std::cout << "\n4. Using operator[]:\n";
    std::cout << "   table[30] = " << table[30] << "\n";
    table[30] = "THIRTY (modified)";
    std::cout << "   After modification: table[30] = " << table[30] << "\n";

    // 5. Remove
    std::cout << "\n5. Removing key 20:\n";
    table.remove(20);
    std::cout << "   Contains key 20 after removal? " << (table.contains(20) ? "yes" : "no") << "\n";
    table.print();
}

int main() {
    HashFunction1 hf1;
    HashFunction2 hf2;
    HashFunction3 hf3;

    demonstrateHashFunction(&hf1, "HashFunction1 (division + offset: (key%N + c + d) % N)");
    demonstrateHashFunction(&hf2, "HashFunction2 (multiplication: (key%N * a * N) % N)");
    demonstrateHashFunction(&hf3, "HashFunction3 (double hashing: (key%N + 1*(1 + key%(N-2))) % N)");

    std::cout << "  changeHashFunction() demo\n";

    HashTable<std::string> table(7, &hf1);
    table.add(10, "ten");
    table.add(20, "twenty");
    table.add(30, "thirty");
    table.add(41, "forty one");
    table.add(52, "fifty two");

    std::cout << "\nInitial table (HashFunction1):\n";
    table.print();

    std::cout << "\nAfter changeHashFunction -> HashFunction2:\n";
    table.changeHashFunction(&hf2);
    table.print();

    std::cout << "\nAfter changeHashFunction -> HashFunction3:\n";
    table.changeHashFunction(&hf3);
    table.print();

    return 0;
}