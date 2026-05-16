#include "BinaryTreeSearch.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>

using namespace std;

BinaryTreeSearch createTree(int n) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, 100);
    vector<int> vec;
    for (int i = 0; i < n; ++i) {
        vec.push_back(dist(gen));
    }
    BinaryTreeSearch tree(vec);
    return tree;
}

void testTree(BinaryTreeSearch & t) {
    cout << "Tree: "; t.printHorizontal(); cout << endl;
    cout << "countOfNodes: " << t.countOfNodes() << endl;
    cout << "isEmpty: " << t.isEmpty() << endl;
    cout << "height: " << t.height() << endl;
    cout << "min: " << t.min() << endl;
    cout << "max: " << t.max() << endl;
    cout << "isBalanced: " << t.isBalanced() << endl;
    if (!t.isEmpty()) {
        int rootKey = t.root()->key();
        cout << "find(" << rootKey << "): " << (t.find(rootKey) ? "yes" : "no") << endl;
        cout << "level(" << rootKey << "): " << t.level(rootKey) << endl;
        cout << "remove(" << rootKey << "): " << t.remove(rootKey) << endl;
    }
    t.add(45);
    cout << "add(999): ";
    t.printHorizontal();
    cout << endl;
}

int main() {
    srand(time(0));
    for (int i = 0; i < 10; ++i) {
        cout << "Test number: " << i + 1 << "\n";
        BinaryTreeSearch t = createTree(10);
        testTree(t);
        cout << "\n";
    }
    return 0;
}
