#include "LinkedList.h"
#include <iostream>

LinkedList::LinkedList() //default constructor
{
	front=NULL;
}

LinkedList::LinkedList(const LinkedList& ls) //copy constructor
{
	deepCopy(ls);
}

LinkedList::~LinkedList() //destructor
{
	deleteList();
}

LinkedList& LinkedList::operator=(const LinkedList& ls) //assignment operator
{
	if(this!=&ls)
	{
		deleteList();
		deepCopy(ls);
	}
	return *this;
}

bool LinkedList::insert(const line& s)
{
		Node* temp=front;
		while(temp!=NULL) //Traverse list
		{
			if(temp->data.name == s.name)
			{
					temp->data.quant+=s.quant;
					return true;
			}
			temp = temp->next;
		}
		front=new Node(s,front);
		return true;
}

int LinkedList::removepart(const line& s)
{
	Node* temp=front;
	if(temp==NULL) //list is empty
		return false;
	if(temp->data==s) //s is first string in list
	{
		if (temp->data.quant < s.quant ) {
				return -1;
		}
		if(temp->data.quant == s.quant ){
			front=temp->next;
			delete temp;
			return 2;
		}else{
			temp->data.quant -=s.quant;
			return 1;
		}
	}
	else
	{
		while(temp->next!=NULL){
			if(temp->next->data==s)
			{
				if (temp->next->data.quant < s.quant ) {
						return -1;
				}
				if(temp->next->data.quant == s.quant){
					Node* deletedNode=temp->next;
					temp->next=temp->next->next;
					delete deletedNode;
					return 2;
				}
				temp->next->data.quant -= s.quant;
				return 1;
			}
			temp=temp->next;
		}
		return 0;
	}
}

bool  LinkedList::remove(const line& s)
{
	Node* temp=front;
	if(temp==NULL) //list is empty
		return false;
	if(temp->data==s) //s is first string in list
	{

		front=temp->next;
		delete temp;
		return true;
	}
	else
	{
		while(temp->next!=NULL){
			if(temp->next->data==s)
			{
				Node* deletedNode=temp->next;
				temp->next=temp->next->next;
				delete deletedNode;
				return true;
			}
			temp=temp->next;
		}
		return false;
	}
}

void  LinkedList::printList() const
{
    Node* temp=front;
    while(temp!=NULL)
    {
        std::cout << temp->data.name << " v kolichesve " <<  temp->data.quant <<  std::endl;
        temp = temp->next;
    }
}

std::ostream& operator<< (std::ostream &out, const LinkedList &p)
{
		LinkedList::Node* temp=p.front;
		while(temp!=NULL)
		{
				out << temp->data.name <<", " <<  temp->data.quant <<  std::endl;
				temp = temp->next;
			}

    return out;
}

void LinkedList:: printListclient(int fd) const
{
    Node* temp=front;
    while(temp!=NULL)
    {
        std::string str = temp->data.name +  " " + std::to_string(temp->data.quant) + ";";
				char *chrstr = new char[str.length() + 1];
				int  zplen  =str.length();
			  strcpy(chrstr, str.c_str());
				send(fd, &zplen,4,MSG_WAITALL);
			  send(fd, chrstr, str.length(), MSG_WAITALL);
				delete [] chrstr;
        temp = temp->next;
    }
}


int LinkedList::searchP(const line& s) const
{
	Node* temp=front;
	while(temp!=NULL) //Traverse list
	{
		if(temp->data.name ==s.name)
		{
			if(temp->data.quant - s.quant < 0)
			{
					return s.quant - temp->data.quant;
			}
			return 0;
		}
		temp = temp->next;
	}
	return s.quant;
}
int LinkedList::search(const line& s) const
{
	Node* temp=front;
	while(temp!=NULL) //Traverse list
	{
		if(temp->data.name ==s.name)
			return temp->data.quant;
		temp = temp->next;
	}
	return 0;
}

std::vector<line> LinkedList::get() const
{
	std::vector<line> str_vec;
	for(Node* temp=front;temp!=NULL;temp=temp->next)
		str_vec.push_back(temp->data);
	return str_vec;
}

void LinkedList::deepCopy(const LinkedList& ls)
{
	front=NULL;
	if(ls.front!=NULL) //Only copy if ls is non-empty
	{
		Node* original=ls.front;
		Node* copy;
		copy=new Node(original->data, NULL);
		front=copy;
		original=original->next;
		while(original!=NULL) //Traverse the original copying each node
		{
			copy->next=new Node(original->data, NULL);
			copy=copy->next;
			original=original->next;
		}
	}
}


void LinkedList::deleteList()
{
	Node* tmp;
	while(front!=NULL){
		tmp=front->next;
		delete front;
		front=tmp;
	}
}
