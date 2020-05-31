#ifndef HASHTABLE_H_INCLUDED
#define HASHTABLE_H_INCLUDED

//////////////////////////////////////////////////////////
//                                                      //
//     							Just a hash set                     //
//                                                      //
//////////////////////////////////////////////////////////

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
	void printclient(int fd) const;
	bool remove(const line&);
 	int removepart(const line& s);
	int search(const line&) const;
  int searchP(const line&) const; // Takes into account the number of parts returns how much is missing
	int size() const; //return numOfItems
	int maxSize() const; //return arrSize
	double loadFactor() const;
	//std::vector<line> intersection(const HashTable&) const;
	//std::vector<line> unions(const HashTable&) const;
//	std::vector<line> difference(const HashTable&) const;
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
