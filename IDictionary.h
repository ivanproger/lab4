#pragma once
#include "DynamicArray.h"
#include "Pair.h"

template <typename Key, typename Value>
class IDictionary {
public:
    virtual ~IDictionary() {}

    virtual void insert(const Key& key, const Value& value) = 0;
    virtual bool exist(const Key& key) const = 0;
    virtual Value get(const Key& key) const = 0;
    virtual bool remove(const Key& key) = 0;
    virtual void Clear() = 0;
    virtual void getAllPairs(DynamicArray<Pair<Key, Value>>& arr) const = 0;
};