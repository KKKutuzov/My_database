#ifndef LINKEDLIST_H_INCLUDED
#define LINKEDLIST_H_INCLUDED

#include <cstdlib>
#include <vector>
#include <string>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <iostream>
#include <arpa/inet.h>
#include <netdb.h>

struct line //The device of parts along with its name
{
  std::string name;
  int quant;
  line& operator=(const line& rhs)
  {
      name = rhs.name;
      quant = rhs.quant;
      return *this;
  }
  bool  operator==(const line &rhs)
  {
      return name == rhs.name;
  }
};

class LinkedList
{
public:
	LinkedList(); //default constructor
	LinkedList(const LinkedList& ls);//copy constructor
	~LinkedList(); //destructor
	LinkedList& operator=(const LinkedList&); //assignment operator
	bool insert(const line&);
	bool remove(const line&);
  void printListclient(int fd) const;
  int removepart(const line&);
	int search(const line&) const;
  int searchP(const line&) const; // Takes into account the number of parts returns how much is missing
	std::vector<line> get() const;
  void printList() const;

private:
	class Node
	{
	public:
		line data;
		Node* next;
		Node(line s)
    {
        data = s;
        next = NULL;
    }
		Node(line s,Node* nd)
    {
        data = s;
        next = nd;
    }
	};
	Node* front;
	void deepCopy(const LinkedList& ls);
	void deleteList();
  friend class HashTable;
  friend std::ostream& operator<< (std::ostream &out, const LinkedList &point);
};

#endif
