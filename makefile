CC=g++
NAME1=server
NAME2=client
DATA=database.cpp HashTable.cpp LinkedList.cpp

all:
	$(CC) server.cpp  $(DATA) -o $(NAME1)

	
