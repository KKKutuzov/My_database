#ifndef DATAGENERATOR_H_INCLUDED
#define DATAGENERATOR_H_INCLUDED

//////////////////////////////////////////////////////////
//                                                      //
//      The function generates a random database        //
//                                                      //
//////////////////////////////////////////////////////////

#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <ctime>
#include <algorithm>
#include <set>






void makedatabase(int size_, int levels,const std::string &namefileI, const std::string &namefile)
{
    std::string s = "abridgment";
    std::vector<std::string> items;
    for(int j=0;j<(size_/levels);j++)
    {
        std::next_permutation(s.begin(), s.end());
        items.push_back(s);
    }
    std::ofstream ofs(namefile);
    for(int i=0;i<levels;i++)
    {
        std::vector<std::string> tmp;
        for(int j=0;j<(size_/levels);j++)
        {
            std::next_permutation(s.begin(), s.end());
            tmp.push_back(s);
        }
        srand(i*i +12);
        for(auto elm: tmp)
        {
            ofs << elm << ", ";
            int rec = rand() % 10;
            rec++;
            for(int k=0;k<rec-1;k++)
            {
                ofs << items[rand() % items.size()]  << ", " <<  rand() % 100 +1  << ", ";
            }
            ofs << items[rand() % items.size()]  << ", " <<  rand() % 100  + 1<< "\n";

        }
        for(auto elm: tmp)
        {
            items.push_back(elm);
        }
    }
    std::ofstream offs(namefileI);
    std::string sp = "abridgment";
    for(int i=0;i<size_;i++)
    {
        offs << sp << ", " << rand() % 100 +1<< "\n";
        next_permutation(sp.begin(), sp.end());
    }
    ofs.close();
    offs.close();
}


#endif
