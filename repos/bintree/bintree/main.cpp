#include <iostream>
#include "BinaryTree.h"

int main() {
    setlocale(LC_ALL, "ru");
    srand(time(0));

    BinaryTreeTester tester(true);

    tester.test(100);

    return 0;
}