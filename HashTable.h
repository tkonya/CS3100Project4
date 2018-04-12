//
// Created by Trevor on 4/10/2018.
//

#ifndef CS3100PROJECT4_HASHTABLE_H
#define CS3100PROJECT4_HASHTABLE_H


#include "Slot.h"
#define MAXHASH int 1000;

class HashTable {
private:
    Slot data[MAXHASH];
public:
    bool insert();
    bool find();
    bool remove();
    float alpha();
};


#endif //CS3100PROJECT4_HASHTABLE_H
