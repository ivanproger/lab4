#pragma once
#include "HashTable.h"
#include <iostream>

template <typename Key, typename Value, typename HashFunc = DefaultHash<Key>>
void DisplayHashTable(const HashTable<Key, Value, HashFunc>& ht) {
    std::cout << "HashTable Contents:\n";
    int capacity = ht.getCapacity();
    for (int i = 0; i < capacity; ++i) {
        try {
            auto entry = ht.getEntry(i);
            if (entry.status == EntryStatus::OCCUPIED) {
                std::cout << "Index " << i
                    << ": Key = " << entry.pair.key
                    << ", Value = " << entry.pair.value
                    << "\n";
            }
        }
        catch (...) {
            std::cout << "error DisplayHashTable\n"; 
        }
    }
}