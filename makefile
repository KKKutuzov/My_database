CC=g++
NAME1=server
NAME2=client
DATA=database.cpp HashTable.cpp LinkedList.cpp
FLAGS	 = -O2

all:
	$(CC) server.cpp  $(DATA) $(FLAGS) -o $(NAME1)
