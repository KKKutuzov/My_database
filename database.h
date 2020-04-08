#ifndef DATABASE_H_INCLUDED
#define DATABASE_H_INCLUDED

#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <list>
#include "HashTable.h"



typedef std::string DeviceName;
typedef std::map<std::string,int> Recipe;

class Database
{
private:
  HashTable data;
  std::map<DeviceName, Recipe> known_recipes;
public:
  Database();
  Database(const std::string &filename_items, const std::string &filename_recipes);
  void print() const;
  void DatabaseToFile(const std::string &filename_items, const std::string &filename_recipes) const;
  void AddDetail(const std::string &det_name,int quant);
  void AddRecipe(std::string Dname,std::map<std::string,int> Rec);
  /*Database& operator= (const Database& other);
  Database(const Database& other, SearchConditions criteria);
  int AddDetail(const Detail &det);
  bool CanMake(const Detail &det) const;
  void MakeDetail(const Detal &det);
  int deleteDetail(const Detail &det);
  int dbsize() const;
  void SaveToFile(const std::string &filename);
  Detail &GetDetail(const string &name);
  void ChangeQuantity(const std::string &name, int newQuant);
  */
};

#endif
