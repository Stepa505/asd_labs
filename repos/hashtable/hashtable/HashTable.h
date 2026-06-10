#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <cmath>
#include <algorithm>

static int myNumber = 17;

static int mod(const int K, const int N) {
	int result = K % N;
	return (result < 0 ? (result * (-1)) : result);
}

class IHashFunction {
public:
	virtual int hash(const int key, const int size) const = 0;
	virtual IHashFunction* clone() const = 0;
};

class HashFunction1 : public IHashFunction {
public:
	int hash(const int key, const int size) const override {
		return mod(mod(key, size) + c + d, size);
	}

	HashFunction1* clone() const override {
		return new HashFunction1();
	}

private:
	int c = mod(myNumber, 5),
		d = mod(myNumber, 7);
};

class HashFunction2 : public IHashFunction {
public:
	int hash(const int key, const int size) const override {
		return mod(mod(key, size) * a * size, size);
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
		return mod(mod(key, size) + 1 * (1 + mod(key, size - 2)), size);
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
	HashTable(int size = 0, IHashFunction* function = getDefaultHashFunction());
	HashTable(const HashTable& other);
	~HashTable();

	void resize(const int size);
	int size() const;

	void print() const;

	void add(const int key, const T& value);
	void remove(const int key);
	bool contains(const int key) const;

	void swap(HashTable& other);
	void changeHashFunction(IHashFunction* function);

	HashTable& operator = (const HashTable& other);
	T& operator [](const int key);

	const std::vector<std::list<std::pair<int, T>>>& table() const;

private:
	std::vector<std::list<std::pair<int, T>>> m_hashTable;
	int m_size;
	IHashFunction* m_function;
};

template<typename T>
HashTable<T>::HashTable(int size, IHashFunction* function)
	: m_size(size), m_function(function) {
	m_hashTable.resize(m_size);
}

template<typename T>
HashTable<T>::HashTable(const HashTable& other)
	: m_hashTable(other.m_hashTable), m_size(other.m_size)
	, m_function(other.m_function->clone()) {
}

template<typename T>
HashTable<T>::~HashTable() {
}

template<typename T>
void HashTable<T>::resize(const int size) {
	if (size == m_size) return;

	std::vector<std::list<std::pair<int, T>>> new_hashTable(size);
	for (int i = 0; i < m_size; i++) {
		for (auto it = m_hashTable[i].begin(); it != m_hashTable[i].end(); ++it) {
			int index = m_function->hash(it->first, size);
			new_hashTable[index].push_back(*it);
		}
	}

	m_size = size;
	m_hashTable = new_hashTable;
}

template<typename T>
int HashTable<T>::size() const {
	return m_size;
}

template<typename T>
void HashTable<T>::add(const int key, const T& value) {
	if (contains(key)) return;

	if (m_size == 0) return;

	int index = m_function->hash(key, m_size);
	m_hashTable[index].push_back(std::make_pair(key, value));
}

template<typename T>
void HashTable<T>::remove(const int key) {
	if (m_size == 0) return;
	int index = m_function->hash(key, m_size);
	for (auto it = m_hashTable[index].begin(); it != m_hashTable[index].end(); ++it) {
		if (it->first == key) {
			m_hashTable[index].erase(it);
			return;
		}
	}
}

template<typename T>
bool HashTable<T>::contains(const int key) const {
	if (m_size == 0) return false;
	int index = m_function->hash(key, m_size);
	for (auto it = m_hashTable[index].begin(); it != m_hashTable[index].end(); ++it)
		if (it->first == key)
			return true;

	return false;
}

template<typename T>
void HashTable<T>::print() const {
	for (int i = 0; i < m_size; ++i) {
		std::cout << "[" << i << "]: ";
		for (auto it = m_hashTable[i].begin(); it != m_hashTable[i].end(); ++it)
			std::cout << "(" << it->first << ", " << it->second << ") ";

		std::cout << std::endl;
	}
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

	std::vector<std::list<std::pair<int, T>>> newTable(m_size);
	for (int i = 0; i < m_size; i++) {
		for (auto it = m_hashTable[i].begin(); it != m_hashTable[i].end(); ++it) {
			int newIndex = newFunction->hash(it->first, m_size);
			newTable[newIndex].push_back(*it);
		}
	}

	m_function = newFunction;
	m_hashTable = newTable;
}

template<typename T>
HashTable<T>& HashTable<T>::operator = (const HashTable& other) {
	if (this != &other) {
		m_size = other.m_size;
		delete m_function;
		m_hashTable = other.m_hashTable;
		m_function = other.m_function->clone();
	}

	return *this;
}

template<typename T>
T& HashTable<T>::operator [](const int key) {
	int index = m_function->hash(key, m_size);
	for (auto it = m_hashTable[index].begin(); it != m_hashTable[index].end(); ++it)
		if (it->first == key)
			return it->second;

	this->add(key, T());
	return m_hashTable[index].back().second;
}

template<typename T>
const std::vector<std::list<std::pair<int, T>>>& HashTable<T>::table() const {
	return m_hashTable;
}
