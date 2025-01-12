#pragma once

template <typename T>
class Sequence {
public:
    class Iterator {
    public:
        virtual ~Iterator() {}
        virtual bool operator==(const Iterator& other) const = 0;
        virtual bool operator!=(const Iterator& other) const = 0;
        virtual T& operator*() = 0;
        virtual Iterator& operator++() = 0;
    };

    virtual ~Sequence() {}

    virtual T& GetFirstElem() = 0;
    virtual T& GetLastElem() = 0;
    virtual T& GetElem(int index) = 0;
    virtual const T& GetElem(int index) const = 0;
    virtual int GetLength() = 0;
    virtual int GetLength() const = 0;

    virtual void Append(T dataElem) = 0;
    virtual void Prepend(T dataElem) = 0;
    virtual void Insert(T dataElem, int index) = 0;
    virtual void Union(Sequence<T>* seq) = 0;
    virtual void Swap(T& a, T& b) = 0;

    virtual Iterator* ToBegin() = 0;
    virtual Iterator* ToEnd() = 0;
};