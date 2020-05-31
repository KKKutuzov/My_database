#ifndef DATABASE_H_INCLUDED
#define DATABASE_H_INCLUDED

#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <list>
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
#include "HashTable.h"

typedef std::string DeviceName;
typedef std::map<std::string,int> Recipe;

enum canmake_status{all_right, no_recipe, no_details};

struct MakeInfo
{
  int type;
  std::vector<line> deficit;
   MakeInfo& operator=(const  MakeInfo& rhs)
   {
      type = rhs.type;
      deficit = rhs.deficit;
      return *this;
   }
};

struct MakeFromInfo
{
    bool all_right;
    std::map<DeviceName,std::vector<DeviceName>> Deficit;
};




class Database
{
private:
  HashTable data;  // This is where information about the parts in the warehouse is stored
  std::map<DeviceName, Recipe> known_recipes;  // Information about all recipes is stored here
public:
  Database(); //Default constructor does nothing
  Database(const std::string &filename_items, const std::string &filename_recipes);  // Reads the database from files
  void print() const;  // It's clear what he's doing)
  void send_to_client(int fd) const;
  void DatabaseToFile(const std::string &filename_items, const std::string &filename_recipes) const;  // Writes the database to files
  void DatabaseFromFile(const std::string &filename_items, const std::string &filename_recipes); // Reads the database from files
  int GetQuant(const DeviceName &name) const;
  void AddDetail(const DeviceName &name, int quant);  // Adds device in the specified quantity
  void AddRecipe(const DeviceName &name, Recipe rec);  // Adds a recipe
  int deleteDetail(const DeviceName &name);  // Removes an item from the warehouse
  int deleteDetail(const DeviceName &name, int quant); // Deletes device in the desired quantity
  MakeInfo CanMake(const DeviceName &name, int quant); //Checking whether a part can be created
  int dbsize() const; // Number of part types
  void MakeDetail(const DeviceName &name, int quant); //Creates a device
  MakeFromInfo MakeFrom(const std::vector<DeviceName> &details); // Second type of request

};

#endif
