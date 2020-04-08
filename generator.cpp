#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <ctime>
#include <algorithm>
#include <set>



using namespace std;



void makedatabase(int size_, int levels, string namefile)
{
    string s = "abridgment";
    vector<string> items;
    ifstream fin;
    namefile+=".txt";
    fin.open("text.txt");
    string line;
    while(getline(fin, line))
    {
        items.push_back(line);
    }
    ofstream ofs(namefile);
    for(int i=0;i<levels;i++)
    {
        vector<string> tmp;
        for(int j=0;j<(size_/levels);j++)
        {
            next_permutation(s.begin(), s.end());
            tmp.push_back(s);
        }
        srand(i*i +12);
        for(auto elm: tmp)
        {
            ofs << elm << ", ";
            int rec = rand() % 10 + 1;
            for(int k=0;k<rec-1;k++)
            {
                ofs << items[rand() % items.size()]  << ", " <<  rand() % 100  << ", ";
            }
            ofs << items[rand() % items.size()]  << ", " <<  rand() % 100  << endl;

        }
        for(auto elm: tmp)
        {
            items.push_back(elm);
        }
    }
}


int main()
{
    makedatabase(25,5,"test2");
    return 0;
}
