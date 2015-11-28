#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
 using namespace std;
int size=0;
void disp();
void insrt();

struct stab
{
    string lbl;
    int adr;
    struct stab *next;
};

struct stab *l, *f;

void insrt(string l,  int LOCCTR)
{
    struct stab *p;
    p = new stab;
    p->lbl = l;
    p->adr = LOCCTR;
    p->next=NULL;
    if(size==0)
    {
      f=p;
      l=p;
    }
    else
    {
      l->next=p;
      l=p;
    }
    size++;
}
bool findsym(string lab)
{
 int i,flag=0;
 struct stab *p;
 p=f;
  for(i=0;i<size;i++)
   {
    if(p->lbl == lab)
     flag=1;
    p=p->next;
   }
 return flag;
}

int gettargetadrs(string lab)
{
  int i,flag=0;
  struct stab *p;
  p=f;
  for(i=0;i<size;i++)
  {
    if(p->lbl == lab)
    {
      flag=1;
      return p->adr;
    }
    p=p->next;
  }
  return flag;
}
