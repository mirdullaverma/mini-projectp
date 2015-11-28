#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class c1
{
public:
	string opcodeTable[49][3];	//Array of strings storing MNEMONICS
	string opCode[49][3];		//Array of strings storing corresponding OPCODES
public:

	c1()
    {
		for(int i=0; i<49; i++)
            for(int j=0; j<3; j++)
            {
                opcodeTable[i][j] = '0';
                opCode[i][j] = '0';
            }
	}
	int hash(string word)
	{
		int in=0;
        int k = 1;
		for(int i=0; i<word.length(); i++)
		{
			in += k * word[i];
		    k += 1;
        }
		in = in%49;
		return in;
	}

	void insrt(string word)
	{
		int in;
		in = hash(word);
		int i=0;
		for(i=0; i<3; i++)
		{
			if(!(opcodeTable[in][i].compare("0")))
			{
				opcodeTable[in][i] = word;
		        break;
			}
		}
	}

	void insrtop(string word, string mnemo)
	{
		int in;
		in = hash(mnemo);
		int i=0;
		for(i=0; i<3; i++)
		{
			if(!(opCode[in][i].compare("0")))
			{
				opCode[in][i] = word;
		        break;
			}
		}
	}

	string getop(string mnemo)
	{
		int in;
		in = hash(mnemo);
		for(int i=0; i<3; i++)
    	{
    		if(opcodeTable[in][i].compare(mnemo) == 0)
    			return opCode[in][i];
		}
		return 0;
	}
	int found(string word)
	{
		int in,foundFlag=0;
		in = hash(word);
		for(int i=0; i<3; i++)
    	{
    		if(opcodeTable[in][i].compare(word) == 0)
    			return 1;
		}
		return 0;
	}
};
