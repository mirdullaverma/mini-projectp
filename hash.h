#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class hashing
{
public:
	string opcodeTable[49][3];
	string opCode[49][3];
public:

	hashing()
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
		int index=0;
        int k = 1;
		for(int i=0; i<word.length(); i++)
		{
			index += k * word[i];
		    k += 1;
        }
		index = index%49;
		return index;
	}

	void insert(string word)
	{
		int index;
		index = hash(word);
		int i=0;
		for(i=0; i<3; i++)
		{
			if(!(opcodeTable[index][i].compare("0")))
			{
				opcodeTable[index][i] = word;
		        break;
			}
		}
	}

	void insertOpcode(string word, string mnemo)
	{
		int index;
		index = hash(mnemo);
		int i=0;
		for(i=0; i<3; i++)
		{
			if(!(opCode[index][i].compare("0")))
			{
				opCode[index][i] = word;
		        break;
			}
		}
	}

	string getOpCode(string mnemo)
	{
		int index;
		index = hash(mnemo);
		for(int i=0; i<3; i++)
    	{
    		if(opcodeTable[index][i].compare(mnemo) == 0)
    			return opCode[index][i];
		}
		return 0;
	}
	int found(string word)
	{
		int index,foundFlag=0;
		index = hash(word);
		for(int i=0; i<3; i++)
    	{
    		if(opcodeTable[index][i].compare(word) == 0)
    			return 1;
		}
		return 0;
	}
};
