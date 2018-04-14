//
// Created by Trevor on 4/10/2018.
//

#ifndef CS3100PROJECT4_HASHTABLE_H
#define CS3100PROJECT4_HASHTABLE_H

#pragma once
#include <string>
#include <iostream>
#include <iomanip>
#include <cmath>

#include "Slot.h"
#define MAXHASH 1000;

template <class T> class HashTable {
private:
    Slot<T>* data;
    int size;
    int currentCapacity;
    static const int MERSENNE_PRIME_1 = 131071;
    static const int MERSENNE_PRIME_2 = 524287;

    /**
     * Hashes the key for primary placement
     * @param key
     * @return
     */
    int primaryHash(int key) {
        return abs(((key - MERSENNE_PRIME_1) * MERSENNE_PRIME_1) + MERSENNE_PRIME_1) % currentCapacity;
    }

    /**
     * Hashes the key for secondary placement after primary
     * @param key
     * @return
     */
    int secondaryHash(int key, int collisions) {
        return abs((key - MERSENNE_PRIME_2) * ((MERSENNE_PRIME_2 + collisions) * collisions) + MERSENNE_PRIME_2) % currentCapacity;
    }

    /**
     * Find the location of a key, or -1 if the key is not found
     * Used by both find and delete functions
     * @param key
     * @return
     */
    int getKeyLocation(int key) {
        // figure out location of key
        int location = NULL;
        int attempts = 0;

        // the number of attempts made to find an empty spot scales up as the table gets more full, from 1*currentCapacity to 3*currentCapacity
        while (attempts < ceil(currentCapacity * ((alpha() + 0.5) * 2))) {

            location = (primaryHash(key) + (attempts * secondaryHash(key, attempts)) % currentCapacity);

            if (data[location].isNormal() && data[location].getKey() == key) {
                return location;
            } else if (data[location].isEmpty()) {
                // if we hit an empty spot, we're done
                return -1;
            }

            ++attempts;
        }

        return -1;
    }

public:
    HashTable() {
        currentCapacity = MAXHASH;
        data = new Slot<T>[currentCapacity];
        size = 0;
    }

    HashTable(int desiredCapacity) {
        currentCapacity = desiredCapacity;
        data = new Slot<T>[currentCapacity];
        size = 0;
    }

    void deleteRandomElement() {
        int attempts = 0;
        int index;
        while (attempts < ceil(currentCapacity * ((alpha() + 0.5) * 2))) {
            index = rand() % currentCapacity;
            if (data[index].isNormal()) {
                --size;
                data[index].kill();
                cout << "deleted element at " << index << endl;
                break;
            }
        }
    }

    int getSize() {
        return size;
    }

    int getCapacity() {
        return currentCapacity;
    }

    /**
     * If a record with the given key is found in the hash table, the function returns true and a copy of the value is returned in value. Otherwise the function returns false.

     * @param key
     * @param value
     * @return
     */
    bool find(int key, T& value) {

        int location = getKeyLocation(key);
        if (location < 0) {
            return false; // no key found, do nothing
        } else {
            value = data[location].getValue(); // set the value since we found it
            return true;
        }
    }

    /**
     * returns the current loading factor, α, of the hash table
     * @return
     */
    float alpha() {
        return size / currentCapacity;
    }

    friend ostream& operator<< (ostream& out, const HashTable& hashTable) {
        for (int i = 0; i < hashTable.currentCapacity ; i++) {
            out << "[" << i << "] : " << hashTable.data[i] << endl;
        }
        return out;
    }

    /**
     * If there is a record with the given key in the hash table, it is deleted and the function returns true; otherwise the function returns false.
     * @param key
     * @return
     */
    bool remove(int key) {
        int location = getKeyLocation(key);
        if (location < 0) {
            return false; // no key found, do nothing
        } else {
            data[location].kill(); // mark it killed, this is akin to deleting it
            --size;
            return true;
        }
    }

    /**
     * Insert a new key/value pair into the table. Duplicate keys are not allowed.
     * This function should return true if the key/value pair is successfully inserted into the hash table, and false if the pair could not be inserted.
     * If the insertion is successful, the number of collisions occuring during the insert operation should be returned in collisions.
     *
     * @param key the key to insert
     * @param value the value to insert
     * @param collisions the location where the number of collisions will be placed
     * @return
     */
    bool insert(int key, T value, int& collisions) {

        collisions = 0;

        // check if currentCapacity has been reached
        if (size == currentCapacity) {
            cout << "Capacity reached, cannot insert!" << endl;
            return false;
        }

        // is this key already here
        int foundValue;
        if (find(key, foundValue)) {
            cout << "Key " << key << " already exists in table, cannot insert" << endl;
            return false;
        }

        // figure out location of
        int candidateLocation;

        // the number of attempts made to find an empty spot scales up as the table gets more full, from 1*currentCapacity to 3*currentCapacity
        while (collisions < ceil(currentCapacity * ((alpha() + 0.5) * 2))) {

            candidateLocation = (primaryHash(key) + (collisions * secondaryHash(key, collisions))) % currentCapacity;

            cout << "Candidate location for " << key << " : " << candidateLocation << endl;
            if (data[candidateLocation].isEmpty() || data[candidateLocation].isTombstone()) {

                ++size; // increment size
                data[candidateLocation] = Slot<T>(key, value);
                cout << "Final location for " << key << " : " << candidateLocation << ", probe tries: " << collisions + 1 << endl;
                return true;
            }

            cout << "Collision encountered" << endl;
            collisions++;
        }

        return false;
    }



};


#endif //CS3100PROJECT4_HASHTABLE_H
