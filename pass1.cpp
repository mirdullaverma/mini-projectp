#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#include "symtable1.h"
#include "Hash.h"

using namespace std;

hashing begin()
{
	ifstream fp("input");
	ofstream fw;
	fw.open("OPCODE_TABLE");
    hashing obj;
	string line;
	string word;
	string mnemo;
	if (fp.is_open())
	{
    	while (getline (fp,line) )
    	{
      		istringstream iss(line);
      		if(iss >> word)
            {   obj.insert(word);
            	mnemo = word;
            	iss >> word;
            	obj.insertOpcode(word, mnemo);
            }
        }
    }
    for(int i=0; i<49; i++)
    	for(int j=0; j<3; j++)
    	{
    		fw << obj.opcodeTable[i][j] << "\t"<< obj.opCode[i][j]<<'\n';
    	}
    	return obj;
}


void DisplaySym()
{
    int i;
    struct stab *p;
    p = first;
    ofstream f("SYMTAB.txt");
    f << "\n\t\t\tLABEL\t\t\tADDRESS\n";
    for(i=0; i<size; i++)
    {
      f << p->label <<"    "<< hex << p->addr << endl;
  cout<<"\n\t\t\t"<<p->label<<"\t\t\t"<<p->addr<<endl;
      p=p->next;
    }
    f.close();
}


int main()
{
	hashing obj;
	obj = begin();
	ofstream rec("IntermediateRecord.txt");
	ifstream fp("inst.txt");
	ofstream fo("Intermediate.txt");
	int locCntr = 0;                         .
  	int codeLength = 0;
	string line;
	string word;
	int startAddr;
  	string progName;
  	int error=2;
  	int startFlag = 0;
  	int wordCount = 1;
  	int prevLoc;
  	int flag = 0;
	if (fp.is_open())
	{
 		while (getline (fp,line, '\n') )
		{
			istringstream iss(line);
			if(line.find("START") && startFlag == 0)
			{
				startFlag = 1;
				int loopCounter = 0;
				while(iss >> word)
				{
					if(!word.compare("START"))
					{
						error = 0;
					}
					if (loopCounter == 0)
					{
						progName = word;
						loopCounter++;
					}
					else if (loopCounter == 1)
					{
						loopCounter++;
					}
					else if (loopCounter == 2)
					{
						loopCounter++;
						stringstream ss(word);
						ss >> hex >> startAddr;
					}
				}
				if (loopCounter == 2)
					startAddr = 0;
				locCntr = startAddr;
				fo << hex << locCntr << "   " << line << "\n";
				continue;
			}
			else
			{
				wordCount = 1;
				prevLoc = locCntr;

				while(iss >> word)
				{

					if(word == "END")
					{
						flag = 1;
					}
					if(wordCount == 2)
					{
						 if(word == "RESW")
						{
				        	iss >> word;
				        	wordCount++;
				        	const char* val = word.c_str();
				        	locCntr += 3 * atoi(val);
							error = 0;
			        	}

						else if(word == "RESB")
						{
			    	    	iss >> word;
			    	    	wordCount++;
							locCntr +=  strtoul(word.c_str(), NULL, 10);
							error = 0;
			        	}

						else if(word == "BYTE")
						{
					  		iss >> word;
					  		wordCount++;
							if(word.at(0) == 'C')
								locCntr += word.length() - 3;
							else
								locCntr++;
							error = 0;
			        	}
						else if(word == "WORD")
						{
							iss >> word;
							wordCount++;
							locCntr += 3;
							error = 0;
			        	}
			        	else
			        	{	locCntr += 3;
			        	}
					}
					if(wordCount == 1 && word != "END")
					{

						if(!SearchSym(word))
						{
							if(obj.found(word) == false)
							{
								InsertSym(word, locCntr);
								error = 0;
							}
						}
						else
							error = 1;
					}
					wordCount++;
				}
				if(word == "END" || flag == 1)
				{
//					error = 0;
					fo << "         " << line << "\n";
					break;
				}
				else
					fo << hex << prevLoc << "   " << line << "\n";
			}
        }
    }
    DisplaySym();
    switch(error)
    {
    	case 0 :
			     cout<< "Starting address:"<< hex << startAddr << endl;
			     codeLength = locCntr - startAddr;
				 cout << "Program length:\t  "<< hex << codeLength << endl << endl;
				 rec << progName << "\t" << hex << startAddr << "\t" << hex << codeLength << endl;
				 cout<<"Pass-1 complete"<<endl;
				 break;
	}
}
