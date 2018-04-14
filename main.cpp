#include <iostream>
#include "HashTable.h"

using namespace std;

int main() {

    HashTable<int> hashTable;

    int collisions = 0;

    hashTable.insert(99, 1111, collisions);
    hashTable.insert(99, 2222, collisions);

    for (int i = 0; i < 50; i++) {
        hashTable.insert(rand() % 10000000, 9001, collisions);
        if (i % 7 == 0 && i > 0) {
            hashTable.deleteRandomElement();
        }
    }
    cout << "Hash table size: " << hashTable.getSize() << endl;
    cout << "Hash table capacity: " << hashTable.getCapacity() << endl;
    cout << "Hash table alpha: " << hashTable.alpha() << endl;
    cout << "Hash table summary: " << hashTable << endl;
    cout << "------------------------------------------------------" << endl << endl;


    return 0;
}