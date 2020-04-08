#include "database.h"
#include <sstream>

Database::Database(const std::string &filename_items, const std::string &filename_recipes)
{
  std::ifstream fin;
  fin.open(filename_items);
  std::string str;
  while(getline(fin,str))
  {
      std::stringstream ss(str);
      line x;
      std::getline(ss,x.name,',');
      ss  >> x.quant;
      data.insert(x);
  }
  //data.print();
  std::ifstream fin1;
  fin1.open(filename_recipes);
  while(getline(fin1,str))
  {
      std::stringstream ss(str);
      std::string f;
      std::getline(ss,f,',');
      ss.ignore(1);
      DeviceName dname = f;
      Recipe rrecipe;
      while(std::getline(ss,f,','))
      {
          int quantt;
          ss >> quantt;
          ss.ignore(1);
          rrecipe[f] = quantt;
          ss.ignore(1);
      }
      known_recipes[dname] = rrecipe;
  }

}

void Database::DatabaseToFile(const std::string &filename_items, const std::string &filename_recipes) const
{
      data.HashTableToFile(filename_items);
      std::ofstream ofs(filename_recipes);
      for(auto elm: known_recipes)
      {
        ofs << elm.first << ", ";
        for(auto items: elm.second)
        {
            ofs << items.first << ", " << items.second <<", ";
        }
        ofs << std::endl;
      }

}

void Database::print() const
{
    std::cout << "Items in storage:\n";
    data.print();
    std::cout << "Known recipes:\n";
    for(auto elm: known_recipes)
    {
      std::cout << elm.first << " - ";
      for(auto items: elm.second)
      {
          std::cout << items.first << " " << items.second <<"; ";
      }
      std::cout << std::endl;
    }

}

void Database::AddDetail(const std::string &det_name,int quant)
{
    data.insert(line(det_name, quant));
}

void Database::AddRecipe(std::string Dname,std::map<std::string,int> Rec)
{
     known_recipes[Dname] = Rec;
}
