#include <iostream>
#include <cstring>
#include <string>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include "findObjCode.h"
#include "Hash.h"
#include "symtable1.h"
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

string createHeaderRec()
{
	int pgn, stn, start;
	string progName;
	string startAddr;
	string codeLength;
	string headerRecord;
	string l1;
	string word;
	ifstream fpRec("IntermediateRecord.txt");
	while(getline(fpRec, l1, '\n' ))
	{
		istringstream iss(l1);
		iss >> word;
		progName = word;
		iss >> word;
		startAddr = word;
		iss >> hex >> word;
		codeLength = word;
	}
	stringstream ss;
	ss.str(string());
	if(progName.length() < 6)
		pgn = 6-progName.length();
	else
		pgn = 0;
	for(int i=0; i<pgn; i++)
	{
		progName += " ";
	}
	if(startAddr.length() < 6)
		stn = 6 - startAddr.length();
	else
		stn = 0;
	for(int i=0; i<stn; i++)
		startAddr = "0" + startAddr;
	headerRecord = "H" + progName + startAddr + codeLength + '\n';
	cout<<headerRecord<<endl;
	return headerRecord;
}

int main()
{
	int wordCount = 1;
	int stn, pgn;
	int target;
	string line;
	string word;
	int i=0, len, flag=1;
	const char* name;
	const char* leng;
	hashing OPTAB;
	OPTAB = begin();
	string targetStr;
	string opcode;
	string objCode;
	string codeLength;
	string headerRecord;
	string textRecord;
	string endRecord;
	string startAddr;
	int count = 0;
	ifstream fp1("Intermediate.txt");
	ifstream fp2("inst.sic");
	ifstream fp5("SYMTAB.txt");
	ofstream fp3("ObjectCode.txt");
	ofstream fp4("AssListing.txt");

	ifstream fpRec("IntermediateRecord.txt");
	string l1;
	while(getline(fpRec, l1, '\n' ))
	{
		istringstream iss(l1);
		iss >> word;
		iss >> word;
		startAddr = word;
		iss >> hex >> word;
		codeLength = word;
	}
	headerRecord = createHeaderRec();
	fp3 << headerRecord ;
	len = strtoul(codeLength.c_str(), NULL, 16);
	stringstream ss;
	ss.str(string());
	ss << hex << len;
	codeLength = ss.str();
	string symbol, tAddr, symLine;
	if(fp5.is_open())
	{
		while(getline(fp5, symLine, '\n'))
		{
			istringstream iss(symLine);
			iss >> symbol;
			iss >> dec >> tAddr;
			int x = strtoul(tAddr.c_str(), NULL, 16);
			InsertSym(symbol, x);
		}
	}
	fp5.close();
	if (fp2.is_open())
	{
 		while (getline (fp1, line, '\n') )
		{
			wordCount = 1;
			istringstream iss(line);
			if(line.find("START"))
			{
				if(flag)
				{
					textRecord = "T";
					iss >> word;
					if(word.length() < 6)
						stn = 6 - word.length();
					else
						stn = 0;
					for(int i=0; i<stn; i++)
					{
						word = "0" + word;
					}
					textRecord += word;
					if(len <= 60)
					{
						textRecord += "00";
						flag = 0;
					}
					else
					{
						flag = 1;
						textRecord += "1E";
						len = len - 60;
					}
				}

				while(iss >> word)
				{
						if( OPTAB.found(word))
						{
							opcode = OPTAB.getOpCode(word);
							iss >> word;
							wordCount++;
							target = getTargetAddr(word);
							ss.str(string());
							ss << hex << target;
							targetStr = ss.str();

							if(target != 0)
							{
								objCode = findObjCode(opcode, targetStr, line);
								textRecord += objCode;
								count++;

							}
							else
							{
								cout<<"Error: Symbol not defined\n";
							}
							continue;
						}

					if(wordCount == 3 && word  != "END")
					{
						if(word == "RESW" || word == "RESB")
							continue;
						else if(word == "WORD")
						{
							iss >> word;
							wordCount++;
							if(word.length() < 6)
								stn = 6 - word.length();
							else
								stn = 0;
							for(int i=0; i<stn; i++)
							{
								word = "0" + word;
							}
							textRecord += word;
							count++;
							continue;
						}
						else if(word == "BYTE")
						{
							iss >> word;
							wordCount++;
							if(word.at(0) == 'C')
							{
								string bytAddr;
								//find the ASCII value
								int temp;
								while(i < word.length()-1)
								{
									temp = word[i];
									stringstream bytWord;
									bytWord << hex << temp;
									string result( bytWord.str() );
									bytAddr += result;
									i++;

								}
								textRecord += bytAddr;
								count++;
								continue;
							}
							else if(word.at(0) == 'X')
							{
								textRecord += word;
								count++;
								continue;
							}
						}
					}
					wordCount++;
				}
			}
		}
		stringstream Wcount;
		Wcount << hex << count*3;
		string countWord( Wcount.str() );
		for(int i=0; i<2-countWord.length(); i++)
		{
			countWord = "0" + countWord;
		}

		textRecord.replace(7, 2, countWord);
		fp3 << textRecord << '\n';
		endRecord = "E" + startAddr + '\n';
		fp3 << endRecord << '\n';
	}
	cout<<"Pass-2 complete. Check file 'ObjectCode.txt'\n";
}
