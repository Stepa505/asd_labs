#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <cmath>
#include <algorithm>
#include <stdexcept>

static int myNumber = 17;

static int mod(const int K, int N) {
    if (N <= 0) {
        N *= -1;
    }
    long long result = static_cast<long long>(K) % N;
    return static_cast<int>(result < 0 ? -result : result);
}

class IHashFunction {
public:
    virtual int hash(const int key, const int size) const = 0;
    virtual IHashFunction* clone() const = 0;
    virtual ~IHashFunction() = default;
};

class HashFunction1 : public IHashFunction {
public:
    int hash(const int key, const int size) const override {
        if (size <= 0) {
            std::cerr << "Error in HashFunction1: Size <= 0!!!\n";
            return 0;
        }
        return mod(mod(key, size) + c + d, size);
    }

    HashFunction1* clone() const override {
        return new HashFunction1();
    }

private:
    int c = mod(myNumber, 5);
    int d = mod(myNumber, 7);
};

class HashFunction2 : public IHashFunction {
public:
    int hash(const int key, const int size) const override {
        if (size <= 0) {
            std::cerr << "Error in HashFunction2: Size <= 0!!!\n";
            return 0;
        }
        
        double fraction = key * a - std::floor(key * a);
        return static_cast<int>(fraction * size);
    }

    HashFunction2* clone() const override {
        return new HashFunction2();
    }

private:
    const double a = (std::sqrt(5) - 1) / 2;
};

class HashFunction3 : public IHashFunction {
public:
    int hash(const int key, const int size) const override {
        if (size <= 0) {
            std::cerr << "Error in HashFunction3: Size <= 0!!!\n";
            return 0;
        }
        if (size <= 2) {
            return mod(key, size);
        }
        return mod(mod(key, size) + 1 + mod(key, size - 2), size);
    }

    HashFunction3* clone() const override {
        return new HashFunction3();
    }
};

template<typename T>
class HashTable {
public:
    static IHashFunction* getDefaultHashFunction() {
        static HashFunction1 hashFunction;
        return &hashFunction;
    }

    HashTable(int size = 0, IHashFunction* function = nullptr);
    HashTable(const HashTable& other);
    ~HashTable();

    void resize(const int size);
    int size() const { return m_size; }

    void print() const;

    bool add(const int key, const T& value);
    bool remove(const int key);
    bool contains(const int key) const;

    void swap(HashTable& other);
    void changeHashFunction(IHashFunction* function);

    HashTable& operator=(const HashTable& other);
    T& operator[](const int key);
    const T& operator[](const int key) const;

    const std::vector<std::list<std::pair<int, T>>>& table() const { return m_hashTable; }

private:
    std::vector<std::list<std::pair<int, T>>> m_hashTable;
    int m_size;
    IHashFunction* m_function;

    void rehash(int newSize);
};

template<typename T>
HashTable<T>::HashTable(int size, IHashFunction* function)
    : m_size(size > 0 ? size : 0), m_function(nullptr) {
    if (function) {
        m_function = function->clone();
    }
    else {
        m_function = getDefaultHashFunction()->clone();
    }
    m_hashTable.resize(m_size > 0 ? m_size : 0);
}

template<typename T>
HashTable<T>::HashTable(const HashTable& other)
    : m_hashTable(other.m_hashTable), m_size(other.m_size)
    , m_function(other.m_function ? other.m_function->clone() : nullptr) {
}

template<typename T>
HashTable<T>::~HashTable() {
    delete m_function;
}

template<typename T>
void HashTable<T>::rehash(int newSize) {
    if (newSize <= 0) {
        m_hashTable.clear();
        m_size = 0;
        return;
    }

    std::vector<std::list<std::pair<int, T>>> newTable(newSize);
    for (int i = 0; i < m_size; ++i) {
        for (auto& pair : m_hashTable[i]) {
            int newIndex = m_function->hash(pair.first, newSize);
            newTable[newIndex].push_back(pair);
        }
    }
    m_hashTable = std::move(newTable);
    m_size = newSize;
}

template<typename T>
void HashTable<T>::resize(int size) {
    if (size == m_size) return;
    if (size < 0) {
        size *= -1;
    }
    rehash(size);
}

template<typename T>
void HashTable<T>::print() const {
    for (int i = 0; i < m_size; ++i) {
        std::cout << "[" << i << "]: ";
        for (auto& pair : m_hashTable[i]) {
            std::cout << "(" << pair.first << ", " << pair.second << ") ";
        }
        std::cout << std::endl;
    }
}

template<typename T>
bool HashTable<T>::add(const int key, const T& value) {
    if (m_size == 0) return false;
    if (contains(key)) return false;

    int index = m_function->hash(key, m_size);
    m_hashTable[index].push_back(std::make_pair(key, value));
    return true;
}

template<typename T>
bool HashTable<T>::remove(const int key) {
    if (m_size == 0) return false;
    int index = m_function->hash(key, m_size);
    auto& bucket = m_hashTable[index];
    for (auto it = bucket.begin(); it != bucket.end(); ++it) {
        if (it->first == key) {
            bucket.erase(it);
            return true;
        }
    }
    return false;
}

template<typename T>
bool HashTable<T>::contains(const int key) const {
    if (m_size == 0) return false;
    int index = m_function->hash(key, m_size);
    for (auto& pair : m_hashTable[index]) {
        if (pair.first == key) return true;
    }
    return false;
}

template<typename T>
void HashTable<T>::swap(HashTable<T>& other) {
    std::swap(m_hashTable, other.m_hashTable);
    std::swap(m_size, other.m_size);
    std::swap(m_function, other.m_function);
}

template<typename T>
void HashTable<T>::changeHashFunction(IHashFunction* newFunction) {
    if (!newFunction) return;
    if (m_size <= 0) {
        delete m_function;
        m_function = newFunction->clone();
        return;
    }

    IHashFunction* cloned = newFunction->clone();
    std::vector<std::list<std::pair<int, T>>> newTable(m_size);
    for (int i = 0; i < m_size; ++i) {
        for (auto& pair : m_hashTable[i]) {
            int newIndex = cloned->hash(pair.first, m_size);
            newTable[newIndex].push_back(pair);
        }
    }
    delete m_function;
    m_function = cloned;
    m_hashTable = std::move(newTable);
}

template<typename T>
HashTable<T>& HashTable<T>::operator=(const HashTable& other) {
    if (this != &other) {
        HashTable temp(other);
        swap(temp);
    }
    return *this;
}

template<typename T>
T& HashTable<T>::operator[](const int key) {
    if (m_size == 0) {
        resize(1);
    }
    int index = m_function->hash(key, m_size);
    for (auto& pair : m_hashTable[index]) {
        if (pair.first == key) {
            return pair.second;
        }
    }
    
    m_hashTable[index].push_back(std::make_pair(key, T()));
    return m_hashTable[index].back().second;
}

template<typename T>
const T& HashTable<T>::operator[](const int key) const {
    if (m_size == 0) {
        throw std::out_of_range("HashTable is empty");
    }
    int index = m_function->hash(key, m_size);
    for (auto& pair : m_hashTable[index]) {
        if (pair.first == key) {
            return pair.second;
        }
    }
    throw std::out_of_range("Key not found");
}