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
#include <string>
#include <iostream>
#include "database.h"
#include "datagenerator.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#define PORT    5555
#define BUFLEN  512
using namespace std;


int   readFromClient(int fd, string &str);
void writeClient(int fd,const string &str);
bool is_number(const string& s);



int  main (void)
{

    int     i, err, opt=1;
    int     sock, new_sock;
    struct  sockaddr_in  addr;
    struct  sockaddr_in  client;
    char    buf[BUFLEN];
    socklen_t  size=0;
    sock = socket (PF_INET, SOCK_STREAM, 0);
    if ( sock<0 ) {
        perror ("Server: cannot create socket");
        exit (EXIT_FAILURE);
    }
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,(const void*)&opt,sizeof(opt));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    err = bind(sock,(struct sockaddr*)&addr,sizeof(addr));
    if ( err<0 ) {
        perror ("Server: cannot bind socket");
        exit (EXIT_FAILURE);
    }
    err = listen(sock,3);
    if ( err<0 ) {
        perror ("Server: listen queue failure");
        exit(EXIT_FAILURE);
    }
    pollfd  act_set[100];
    act_set[0].fd = sock;
    act_set[0].events = POLLIN;
    act_set[0].revents = 0;
    int num_set = 1;
    //makedatabase(20,5,"items.txt","recipe.txt");
    Database x("test_data.txt","test_recipe.txt");
    while (1) {
        // Проверим, не появились ли данные в каком-либо сокете.
        // В нашем варианте ждем до фактического появления данных.
        int ret = poll (act_set, num_set, -1);
        if (ret < 0) {
            perror("Server: poll  failure");
            exit (EXIT_FAILURE);
        }
        if (ret > 0) {
           for (i=0; i<num_set; i++) {
              if (act_set[i].revents & POLLIN) {
                  printf("get POLLIN at fd %d\n", act_set[i].fd);
                  act_set[i].revents &= ~POLLIN;
                  if (i==0) {
                      new_sock = accept(act_set[i].fd,(struct sockaddr*)&client,&size);
                      printf("new client at port %u\n", ntohs(client.sin_port));
                      if (num_set < 100) {
                         act_set[num_set].fd = new_sock;
                         act_set[num_set].events = POLLIN;
                         act_set[num_set].revents = 0;
                         num_set++;
                      } else {
                         printf("no more sockets for client");
                         close(new_sock);
                      }
                   } else {
                      // пришли данные в уже существующем соединени
                      string str;
                      err = readFromClient(act_set[i].fd,str);
                      cout << "im get :" << str<< endl;
                      if ( err<0 || str == "stop")  {
                          printf("get stop\n");
                          close (act_set[i].fd);
                          if (i < num_set-1) {
                             act_set[i] = act_set[num_set - 1];
                             num_set--;
                             i--;
                          }
                      } else {
                          int  zplen  =str.length();
                          stringstream ss(str);
                          string zapros;
                          ss >> zapros;
                          std::transform(zapros.begin(), zapros.end(), zapros.begin(), ::tolower);
                          if(zapros == "canmake")
                          {
                              line det;
                              ss.ignore(1);
                              getline(ss,det.name,',');
                              cout << det.name << endl;
                              string check;
                              ss >> check;
                              if(check.size()==0 ||  !is_number(check))
                              {
                                  int k=1;
                                  send(act_set[i].fd, &k,4,MSG_WAITALL);
                                  writeClient(act_set[i].fd,"Input error");
                                  continue;
                              }
                              det.quant =  atoi(check.c_str());
                              ss >> check;
                              if(check != "end")
                              {
                                  int k=1;
                                  send(act_set[i].fd, &k,4,MSG_WAITALL);
                                  writeClient(act_set[i].fd,"Input error");
                                  continue;
                              }
                              MakeInfo inf;
                              inf =  x.CanMake(det.name,det.quant);
                              switch (inf.type)
                              {
                                case 0:
                                {
                                    int k=1;
                                    send(act_set[i].fd, &k,4,MSG_WAITALL);
                                    writeClient(act_set[i].fd,"You can make this!");
                                    break;
                                }
                                case 1:
                                {
                                      int k=1;
                                      send(act_set[i].fd, &k,4,MSG_WAITALL);
                                      writeClient(act_set[i].fd,"I don't have a recipe. You can add it using the command \"AddRecipe DETAILNAME, FIRTST_ITEM, QUANT, SECOND_ITEM, QUANT ,... \"");
                                      break;
                                }
                                case 2:
                                {
                                    int k=1;
                                    send(act_set[i].fd, &k,4,MSG_WAITALL);
                                    string neh = "Not enough details: ";
                                    for(int i=0;i<inf.deficit.size()-1;i++)
                                    {
                                          neh += inf.deficit[i].name + ", " + to_string(inf.deficit[i].quant) + ", ";
                                    }
                                    neh += inf.deficit[inf.deficit.size()-1].name + ", " +  to_string(inf.deficit[inf.deficit.size()-1].quant);
                                    writeClient(act_set[i].fd,neh);
                                    break;
                                }
                              }
                          }
                          else if(zapros == "make")
                          {
                              line det;
                              ss.ignore(1);
                              getline(ss,det.name,',');
                              string check;
                              ss >> check;
                              if(check.size()==0 ||  !is_number(check))
                              {
                                  int k=1;
                                  send(act_set[i].fd, &k,4,MSG_WAITALL);
                                  writeClient(act_set[i].fd,"Input error");
                                  continue;
                              }
                              det.quant =  atoi(check.c_str());
                              ss >> check;
                              if(check != "end")
                              {
                                  int k=1;
                                  send(act_set[i].fd, &k,4,MSG_WAITALL);
                                  writeClient(act_set[i].fd,"Input error");
                                  continue;
                              }
                              if(x.CanMake(det.name,det.quant).type != 0)
                              {
                                  string ans =  "Caught exception number:  "  + to_string(1) + ". Try \"canmake " +  det.name + " end\" for more info";
                                  int k=1;
                                  send(act_set[i].fd, &k,4,MSG_WAITALL);
                                  writeClient(act_set[i].fd,ans);
                                  continue;
                              }
                              x.MakeDetail(det.name,det.quant);
                              int k=1;
                              send(act_set[i].fd, &k,4,MSG_WAITALL);
                              writeClient(act_set[i].fd,"Great! You make "+ to_string(det.quant) + " " + det.name );
                          }
                          else if(zapros == "adddetail")
                          {
                              line det;
                              ss.ignore(1);
                              getline(ss,det.name,',');
                              string check;
                              ss >> check;
                              if(check.size()==0 ||  !is_number(check))
                              {
                                  int k=1;
                                  send(act_set[i].fd, &k,4,MSG_WAITALL);
                                  writeClient(act_set[i].fd,"Input error");
                                  continue;
                              }
                              det.quant =  atoi(check.c_str());
                              ss >> check;
                              if(check != "end")
                              {
                                  int k=1;
                                  send(act_set[i].fd, &k,4,MSG_WAITALL);
                                  writeClient(act_set[i].fd,"Input error");
                                  continue;
                              }
                              x.AddDetail(det.name,det.quant);
                              int k=1;
                              send(act_set[i].fd, &k,4,MSG_WAITALL);
                              writeClient(act_set[i].fd,"Great! You add "+ to_string(det.quant) + " " + det.name );
                          }
                          else if(zapros == "show")
                          {
                              string check;
                              ss >> check;
                              if(check != "end")
                              {
                                  int k=1;
                                  send(act_set[i].fd, &k,4,MSG_WAITALL);
                                  writeClient(act_set[i].fd,"Input error");
                                  continue;
                              }
                              x.send_to_client(act_set[i].fd);
                          }
                          else if(zapros == "getquant")
                          {
                              string det;
                              ss >> det;
                              string check;
                              ss >> check;
                              if(check != "end")
                              {
                                  int k=1;
                                  send(act_set[i].fd, &k,4,MSG_WAITALL);
                                  writeClient(act_set[i].fd,"Input error");
                                  continue;
                              }
                              string answer  = to_string(x.GetQuant(det)) + " " + det + " in stock";
                              cout << answer << endl;
                              int k=1;
                              send(act_set[i].fd, &k,4,MSG_WAITALL);
                              writeClient(act_set[i].fd,answer);
                          }
                          else if(zapros == "deletedetail")
                          {
                            line det;
                            ss.ignore(1);
                            getline(ss,det.name,',');
                            string check;
                            ss >> check;
                            if(check.size()==0 ||  !is_number(check))
                            {
                                int k=1;
                                send(act_set[i].fd, &k,4,MSG_WAITALL);
                                writeClient(act_set[i].fd,"Input error");
                                continue;
                            }
                            det.quant =  atoi(check.c_str());
                            ss >> check;
                            if(check != "end")
                            {
                                int k=1;
                                send(act_set[i].fd, &k,4,MSG_WAITALL);
                                writeClient(act_set[i].fd,"Input error");
                                continue;
                            }
                            if(x.deleteDetail(det.name,det.quant) == -1)
                            {
                                int k=1;
                                send(act_set[i].fd, &k,4,MSG_WAITALL);
                                writeClient(act_set[i].fd,"Device not found");
                                continue;
                            }
                            int k=1;
                            send(act_set[i].fd, &k,4,MSG_WAITALL);
                            writeClient(act_set[i].fd,"Great! You delete "+ to_string(det.quant) + " " + det.name);
                          }
                          else if(zapros == "exit")
                          {
                              string check;
                              ss >> check;
                              if(check != "end")
                              {
                                int k=1;
                                send(act_set[i].fd, &k,4,MSG_WAITALL);
                                writeClient(act_set[i].fd,"Input error");
                                continue;
                              }
                              for (i=0; i<num_set; i++)
                              {
                                  close(act_set[i].fd);
                              }
                              cout << "Good, bye\n";
                              close(sock);
                              return 0;
                          }
                          else if(zapros == "addrecipe")
                          {
                              ss.ignore(1);
                              string f;
                              getline(ss,f,',');
                              ss.ignore(1);
                              DeviceName dname = f;
                              Recipe rrecipe;
                              int flag=0;
                              while(getline(ss,f,','))
                              {
                                  if(f == "end")
                                  {
                                      flag = 1;
                                      break;
                                  }
                                  string check;
                                  int quantt;
                                  ss.ignore(1);
                                  getline(ss,check,',');
                                  if(!is_number(check))
                                  {
                                      break;
                                  }
                                  quantt =  atoi(check.c_str());;
                                  rrecipe[f] = quantt;
                                  ss.ignore(1);
                              }
                              if(flag == 1)
                              {
                                  x.AddRecipe(dname,rrecipe);
                                  int k=1;
                                  send(act_set[i].fd, &k,4,MSG_WAITALL);
                                  writeClient(act_set[i].fd,"The recipe was added successfully");
                                  continue;
                              }
                              int k=1;
                              send(act_set[i].fd, &k,4,MSG_WAITALL);
                              writeClient(act_set[i].fd,"Input error");
                              continue;
                          }
                          else if(zapros == "makefrom")
                          {
                                string f;
                                int flag = 0;
                                vector<DeviceName> vect;
                                ss.ignore(1);
                                while(getline(ss,f,','))
                                {
                                      if(f == "end")
                                      {
                                          flag = 1;
                                          break;
                                      }
                                      vect.push_back(f);
                                      ss.ignore(1);
                                }
                                if(flag == 0)
                                {
                                    int k=1;
                                    send(act_set[i].fd, &k,4,MSG_WAITALL);
                                    writeClient(act_set[i].fd,"Input error");
                                    continue;
                                }
                                MakeFromInfo inf = x.MakeFrom(vect);
                                if(inf.all_right == true)
                                {
                                    if(inf.Deficit.size() > 0)
                                    {
                                        string ans = "You can make : ";
                                        for(auto elm: inf.Deficit)
                                        {
                                            ans += elm.first + " ";
                                            if(elm.second.size() == 0)
                                            {
                                                ans+="with no deficit";
                                            }
                                            else
                                            {
                                                ans += "with deficit :\"";
                                                for(auto item: elm.second)
                                                {
                                                    ans+=item + " ";
                                                }
                                                ans += "\" ";
                                            }
                                        }
                                        int k=1;
                                        send(act_set[i].fd, &k,4,MSG_WAITALL);
                                        writeClient(act_set[i].fd,ans);
                                        continue;
                                  }
                                  else
                                  {
                                      int k=1;
                                      send(act_set[i].fd, &k,4,MSG_WAITALL);
                                      writeClient(act_set[i].fd,"You can't do anything");
                                      continue;
                                  }
                                }

                                int k=1;
                                send(act_set[i].fd, &k,4,MSG_WAITALL);
                                writeClient(act_set[i].fd,"Some components are not in stock");
                                continue;
                          }
                          else{
                          int k=1;
                          send(act_set[i].fd, &k,4,MSG_WAITALL);
                          writeClient(act_set[i].fd,"Input error");
                        }


                      }
                  }
              }
           }
        }
    }

}



int  readFromClient (int fd, string &str)
{
    int  nbytes;
    int bufsize;
    recv(fd,&bufsize,4,MSG_WAITALL);
    char buf[bufsize];
    nbytes = recv(fd,buf,bufsize,MSG_WAITALL);
    if ( nbytes<0 ) {
        // ошибка чтения
        perror ("Server: read failure");
        return -1;
    } else if ( nbytes==0 ) {
        // больше нет данных
        return -1;
    } else {
        // есть данные
        string s(buf, sizeof(buf));
        str = s;
        return 0;
    }
}

void writeClient(int fd, const string &str)
{
  char *chrstr = new char[str.length() + 1];
  int  zplen  =str.length();
  strcpy(chrstr, str.c_str());

  send(fd, &zplen,4,MSG_WAITALL);
  send(fd, chrstr, str.length(), MSG_WAITALL);
  delete [] chrstr;
}
bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}
