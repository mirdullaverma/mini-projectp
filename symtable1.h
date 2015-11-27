#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>


 using namespace std;


int size=0;
void DisplaySym();
void InsertSym();

struct stab
{
    string label;
    int addr;
    struct stab *next;
};

struct stab *last, *first;


void InsertSym(string l,  int LOCCTR)
{
    struct stab *p;
    p = new stab;
    p->label = l;
    p->addr = LOCCTR;
    p->next=NULL;
    if(size==0)
    {
      first=p;
      last=p;
    }
    else
    {
      last->next=p;
      last=p;
    }
    size++;
}



bool SearchSym(string lab)
{
 int i,flag=0;
 struct stab *p;
 p=first;
  for(i=0;i<size;i++)
   {
    if(p->label == lab)
     flag=1;
    p=p->next;
   }
 return flag;
}

int getTargetAddr(string lab)
{
  int i,flag=0;
  struct stab *p;
  p=first;
  for(i=0;i<size;i++)
  {
    if(p->label == lab)
    {
      flag=1;
      return p->addr;
    }
    p=p->next;
  }
  return flag;
}

