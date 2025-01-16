#pragma once
#include "Sequence.h"
#include <stdexcept>

template <class T>
class DynamicArray : public Sequence<T>
{
private:
    T* data;
    int size;
    int capacity;

    void Resize(int newCapacity)
    {
        if (newCapacity < size)
            newCapacity = size;

        T* newData = new T[newCapacity];
        for (int i = 0; i < size; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    class DynamicArrayIterator : public Sequence<T>::Iterator {
    private:
        T* current;
    public:
        DynamicArrayIterator(T* current) : current(current) {}

        bool operator==(const typename Sequence<T>::Iterator& other) const override {
            const DynamicArrayIterator* otherIt = dynamic_cast<const DynamicArrayIterator*>(&other);
            return (otherIt && current == otherIt->current);
        }

        bool operator!=(const typename Sequence<T>::Iterator& other) const override {
            return !(*this == other);
        }

        T& operator*() override {
            return *current;
        }

        typename Sequence<T>::Iterator& operator++() override {
            current++;
            return *this;
        }
    };

    DynamicArray() : size(0), capacity(10) {
        data = new T[capacity];
    }

    DynamicArray(int initialCapacity) : size(0), capacity(initialCapacity) {
        if (capacity <= 0) capacity = 10;
        data = new T[capacity];
    }

    DynamicArray(const DynamicArray<T>& other) {
        size = other.size;
        capacity = other.capacity;
        data = new T[capacity];
        for (int i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    DynamicArray<T>& operator=(const DynamicArray<T>& other) {
        if (this == &other) {
            return *this; 
        }

        delete[] data;
        size = other.size;
        capacity = other.capacity;
        data = new T[capacity];
        for (int i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
        return *this;
    }

    DynamicArray(T* items, int itemsSize) {
        size = itemsSize;
        capacity = itemsSize * 2;
        if (capacity < 10) capacity = 10;
        data = new T[capacity];
        for (int i = 0; i < size; ++i) {
            data[i] = items[i];
        }
    }

    ~DynamicArray() {
        delete[] data;
    }

    // Константный вариант
    const T& operator[](int index) const {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    T& operator[](int index) {
        if (index < 0 || index >= size)
            throw std::out_of_range("Index out of range");
        return data[index];
    }

    T& GetFirstElem() override {
        if (size == 0) throw std::out_of_range("Array is empty");
        return data[0];
    }

    T& GetLastElem() override {
        if (size == 0) throw std::out_of_range("Array is empty");
        return data[size - 1];
    }

    T& GetElem(int index) override {
        if (index < 0 || index >= size)
            throw std::out_of_range("Index out of range");
        return data[index];
    }

    const T& GetElem(int index) const override {
        if (index < 0 || index >= size)
            throw std::out_of_range("Index out of range");
        return data[index];
    }

    void Swap(T& a, T& b) override {
        T temp = a;
        a = b;
        b = temp;
    }

    int GetLength() override {
        return size;
    }

    int GetLength() const override {
        return size;
    }

    void Append(T dataElem) override {
        Insert(dataElem, size);
    }

    void Prepend(T dataElem) override {
        Insert(dataElem, 0);
    }

    void Insert(T dataElem, int index) override {
        if (index < 0 || index > size) {
            throw std::out_of_range("Insert index out of range");
        }
        if (size >= capacity) {
            Resize(capacity * 2);
        }
        for (int i = size; i > index; i--) {
            data[i] = data[i - 1];
        }
        data[index] = dataElem;
        size++;
    }

    void Union(Sequence<T>* seq) override {
        for (int i = 0; i < seq->GetLength(); i++) {
            Append(seq->GetElem(i));
        }
    }

    void Reserve(int newCapacity) {
        if (newCapacity > capacity) {
            Resize(newCapacity);
        }
    }

    DynamicArray<T>* GetSubsequence(int startIndex, int endIndex) {
        if (startIndex < 0 || endIndex >= size || startIndex > endIndex) {
            throw std::out_of_range("Invalid subsequence indices");
        }
        int length = endIndex - startIndex + 1;
        T* items = new T[length];
        for (int i = 0; i < length; i++) {
            items[i] = data[startIndex + i];
        }
        return new DynamicArray<T>(items, length);
    }

    void RemoveAt(int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("RemoveAt: index out of range");
        }
        for (int i = index; i < size - 1; i++) {
            data[i] = data[i + 1];
        }
        size--;
    }

    void Clear() {
        delete[] data;
        data = new T[capacity];
        size = 0;
    }

    typename Sequence<T>::Iterator* ToBegin() override {
        return new DynamicArrayIterator(data);
    }

    typename Sequence<T>::Iterator* ToEnd() override {
        return new DynamicArrayIterator(data + size);
    }
};
