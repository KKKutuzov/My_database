#ifndef LINKEDLIST_H_INCLUDED
#define LINKEDLIST_H_INCLUDED

#include <cstdlib>
#include <vector>
#include <string>


class line
{
public:
  std::string name;
  int quant;
  line(){}
  line(const std::string namedet,int x)
  {
      name = namedet;
      quant = x;
  }
  line(const line &rhs)
  {
      name = rhs.name;
      quant = rhs.quant;
  }
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
	bool search(const line&) const;
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
