#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <iomanip>
#include <sstream>

typedef int (*HashFunc)(int K, int N, int i);

template<typename T>
class HashTable
{
private:
	struct Node
	{
		int key;
		T value;
		Node(int k, const T& v) : key(k), value(v) {}
	};

	std::vector<std::list<Node>> table;
	int N;
	HashFunc currentHashFunc;

	int h1(int K, int i) const;
	int h2(int K, int i) const;
	int h3(int K, int i) const;
	int hashWithCurrentFunc(int K, int i) const;
	int findHash(int K, int& index) const;

public:
	explicit HashTable(int size = 10);
	HashTable(const HashTable& other);

	~HashTable();

	HashTable& operator=(const HashTable& other);

	T& operator[](int key);

	void clear();

	void insert(int key, const T& value);

	bool remove(int key);

	bool exists(int key) const;

	void swap(HashTable& other);

	void resize(int newSize);

	void print() const;

	void setHashFunction(HashFunc func);

	int getSize() const;
	int getElementCount() const;

	int getHash1(int key, int i) const;
	int getHash2(int key, int i) const;
	int getHash3(int key, int i) const;
};

template<typename T>
int HashTable<T>::h1(int K, int i) const
{
	int c = 2;
	int d = 0;
	int h_plus = K % N;
	return (h_plus + c * i + d * i * i) % N;
}

template<typename T>
int HashTable<T>::h2(int K, int i) const
{
	int a = 2;
	int h_minus = K % N;
	return (h_minus ^ (a * N)) % N;
}

template<typename T>
int HashTable<T>::h3(int K, int i) const
{
	return ((K % N) + i * (1 + (K % (N - 2 > 0 ? N - 2 : 1)))) % N;
}

template<typename T>
int HashTable<T>::hashWithCurrentFunc(int K, int i) const
{
	if (currentHashFunc == nullptr)
		return h1(K, i);
	return currentHashFunc(K, N, i);
}

template<typename T>
int HashTable<T>::findHash(int K, int& index) const
{
	for (int i = 0; i < N; ++i)
	{
		int idx = h1(K, i);
		for (auto& node : table[idx])
		{
			if (node.key == K)
			{
				index = idx;
				return idx;
			}
		}
	}
	return -1;
}

template<typename T>
HashTable<T>::HashTable(int size) : N(size > 0 ? size : 10), currentHashFunc(nullptr)
{
	table.resize(N);
}

template<typename T>
HashTable<T>::HashTable(const HashTable& other) : N(other.N), currentHashFunc(other.currentHashFunc), table(other.table)
{
}

template<typename T>
HashTable<T>::~HashTable()
{
	clear();
}

template<typename T>
HashTable<T>& HashTable<T>::operator=(const HashTable& other)
{
	if (this != &other)
	{
		clear();
		N = other.N;
		currentHashFunc = other.currentHashFunc;
		table = other.table;
	}
	return *this;
}

template<typename T>
void HashTable<T>::clear()
{
	for (auto& chain : table)
	{
		chain.clear();
	}
}

template<typename T>
void HashTable<T>::insert(int key, const T& value)
{
	for (int i = 0; i < N; ++i)
	{
		int idx = h1(key, i);
		auto& chain = table[idx];

		auto it = chain.begin();
		while (it != chain.end())
		{
			if (it->key == key)
			{
				it->value = value;
				return;
			}
			++it;
		}

		chain.push_back(Node(key, value));
		return;
	}
}
template<typename T>
bool HashTable<T>::remove(int key)
{
	for (int i = 0; i < N; ++i)
	{
		int idx = h1(key, i);
		auto& chain = table[idx];

		auto it = chain.begin();
		while (it != chain.end())
		{
			if (it->key == key)
			{
				chain.erase(it);
				return true;
			}
			++it;
		}
	}
	return false;
}

template<typename T>
bool HashTable<T>::exists(int key) const
{
	for (int i = 0; i < N; ++i)
	{
		int idx = h1(key, i);
		const auto& chain = table[idx];

		for (const auto& node : chain)
		{
			if (node.key == key)
				return true;
		}
	}
	return false;
}

template<typename T>
T& HashTable<T>::operator[](int key)
{
	for (int i = 0; i < N; ++i)
	{
		int idx = h1(key, i);
		auto& chain = table[idx];

		for (auto& node : chain)
		{
			if (node.key == key)
				return node.value;
		}

		chain.push_back(Node(key, T()));
		return chain.back().value;
	}

	static T dummy;
	return dummy;
}

template<typename T>
void HashTable<T>::swap(HashTable& other)
{
	std::swap(N, other.N);
	std::swap(currentHashFunc, other.currentHashFunc);
	table.swap(other.table);
}

template<typename T>
void HashTable<T>::resize(int newSize)
{
	if (newSize <= 0) return;

	HashTable<T> newTable(newSize);
	for (const auto& chain : table)
	{
		for (const auto& node : chain)
		{
			newTable.insert(node.key, node.value);
		}
	}
	*this = newTable;
}

template<typename T>
void HashTable<T>::print() const
{
	for (int i = 0; i < N; ++i)
	{
		std::cout << "h(" << i << ") = " << i << ": ";
		if (table[i].empty())
		{
			std::cout << "(empty)";
		}
		else
		{
			bool first = true;
			for (const auto& node : table[i])
			{
				if (!first) std::cout << ", ";
				std::cout << "[" << node.key << ": " << node.value << "]";
				first = false;
			}
		}
		std::cout << "\n";
	}
}

template<typename T>
void HashTable<T>::setHashFunction(HashFunc func)
{
	currentHashFunc = func;
}

template<typename T>
int HashTable<T>::getSize() const
{
	return N;
}

template<typename T>
int HashTable<T>::getElementCount() const
{
	int count = 0;
	for (const auto& chain : table)
	{
		count += chain.size();
	}
	return count;
}

template<typename T>
int HashTable<T>::getHash1(int key, int i) const
{
	return h1(key, i);
}

template<typename T>
int HashTable<T>::getHash2(int key, int i) const
{
	return h2(key, i);
}

template<typename T>
int HashTable<T>::getHash3(int key, int i) const
{
	return h3(key, i);
}