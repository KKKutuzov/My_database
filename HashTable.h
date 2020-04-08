#ifndef HASHTABLE_H_INCLUDED
#define HASHTABLE_H_INCLUDED

#include "LinkedList.h"

class HashTable
{
public:
	HashTable(); //default constructor
	HashTable(int); //one parameter constructor
	HashTable(const HashTable&); //copy constructor
	~HashTable(); //destructor
	HashTable& operator=(const HashTable&); //assignment operator
	bool insert(const line&);
	bool remove(const line&);
	bool search(const line&) const;
	int size() const; //return numOfItems
	int maxSize() const; //return arrSize
	double loadFactor() const;
	std::vector<line> intersection(const HashTable&) const;
	std::vector<line> unions(const HashTable&) const;
	std::vector<line> difference(const HashTable&) const;
	void HashTableToFile(const std::string &filename_items) const;
	void print() const;

private:
	LinkedList* arr;
	int arrSize;
	int numOfItems;
	int hashFunc(const line&) const;
	int getPrime(int) const;
	bool isPrime(int) const;
	void deepCopy(const HashTable& h);
	std::vector<line> get() const; //returns a vector of all the lines in the HashTable
};
#endif
