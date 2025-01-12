#pragma once

template <typename Key, typename Value>
struct Pair {
    Key key;
    Value value;

    Pair() : key(Key()), value(Value()) {}
    Pair(const Key& k, const Value& v) : key(k), value(v) {}

    bool operator==(const Pair<Key, Value>& other) const {
        return (key == other.key) && (value == other.value);
    }

    bool operator!=(const Pair<Key, Value>& other) const {
        return !(*this == other);
    }

    bool operator<(const Pair<Key, Value>& other) const {
        if (key < other.key) {
            return true;
        }
        else if (key == other.key) {
            return value < other.value;
        }
        return false;
    }

    bool operator>(const Pair<Key, Value>& other) const {
        return other < *this;
    }

    bool operator<=(const Pair<Key, Value>& other) const {
        return !(other < *this);
    }

    bool operator>=(const Pair<Key, Value>& other) const {
        return !(*this < other);
    }
};
