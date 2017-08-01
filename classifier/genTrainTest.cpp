#include "stdafx.h"
#include "genTrainTest.h"
#include <iostream>
#include <string>

using namespace std;


string convertInt(int number, int slength)
{
   stringstream ss;//create a stringstream
 
   if(slength)
   {
	    ss   <<   setw(slength)   <<   setfill('0')   <<   number;  
   }
   else
   {
	   ss << number;//add number to the stream;
   }
   
   return ss.str();//return a string with the contents of the stream
}

void GenTrainTest(void)
{
	FILE *train_file, *test_file;

	train_file = fopen("train_file", "w+");
	test_file = fopen("test_file", "w+");
	string imgName;
	UnrepeatedIntRandom randperm;

	int numClass[] = {216,241,311,210,289,360,328,260,308,374,410,292,356,215,315};
	int i, j;
	int tmpNum;

	for(i=0; i<15; i++)
	{
		InitUnrepeatedIntRandom(&randperm, 1, numClass[i]+1, numClass[i]);

		for(j= 0; j<100; j++)
		{
			tmpNum = Random(&randperm);	
			imgName = "image_"+ convertInt(tmpNum, 4);
			fprintf(train_file, "%d %s\n", i, imgName.c_str()); 
		}

		for(j= 100; j<numClass[i]; j++)
		{
			tmpNum = Random(&randperm);	
			imgName = "image_"+ convertInt(tmpNum, 4);
			fprintf(test_file, "%d %s\n", i, imgName.c_str()); 
		}
	}

	fclose(train_file);
	fclose(test_file);

}