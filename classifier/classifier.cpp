// classifier.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include<fstream>

#include "cv.h"
#include "highgui.h"

#include "genTrainTest.h"

using namespace std;


int main(int argc, _TCHAR* argv[])
{

	string projectPath = (string)(argv[1]);

	string ftype;
	ftype= (string)(argv[2]);
	string binNum = (string)(argv[3]);

	//GenTrainTest();
	//string projectPath = "D:\\Experiment\\SIFT_16_400_msf\\";
	string categories[] = {"bedroom", "CALsuburb", "industrial", "kitchen", "livingroom", "MITcoast", "MITforest",
		"MIThighway", "MITinsidecity", "MITmountain", "MITopencountry", "MITstreet", "MITtallbuilding", "PARoffice", "store"};

	int pos_class, MARK;

	int groupNum;
	int maxGroupNum = 10;

	string train_name, test_name;

	for(groupNum = 0; groupNum< maxGroupNum; groupNum++)
	{
		train_name = ".\\data\\tr_"+ binNum +"_g"+convertInt(groupNum) +ftype;
		test_name = ".\\data\\te_"+ binNum +"_g"+convertInt(groupNum)+ftype;

		string tr_idx = ".\\dataIdx\\tr_g"+convertInt(groupNum);
		string te_idx = ".\\dataIdx\\te_g"+convertInt(groupNum);

		for(pos_class=0; pos_class<1; pos_class++)
		{
			
			string fileName;
			int count = 1;

			FILE *tr_file;

			tr_file = fopen(train_name.c_str(), "w+");
			//test_file = fopen("test_file", "r+");

			ifstream train_file(tr_idx.c_str());
			int i, j;
			string imgName;

			while(!train_file.eof())
			{
				train_file>>i>>imgName;
				//cout<<i<<" "<<imgName.c_str()<<endl;
				if(i == pos_class)
					MARK = 1;
				else
					MARK = -1;
				fileName = projectPath +categories[i] +"\\"+imgName+".xml";

				//cout<<fileName.c_str()<<endl;
				CvMat * msfFeature;
				msfFeature = (CvMat *)cvLoad(fileName.c_str());

				//fprintf(tr_file, "%d ", MARK);
				fprintf(tr_file, "%d ", i+1);
				
				for(j = 0; j<msfFeature->width; j++)
				{
					/*printf("%d %d %d %s\n", i+1, j+1, count, fileName.c_str());*/
					fprintf(tr_file, "%d:%f", j+1, CV_MAT_ELEM(*msfFeature, float, 0, j));
					if(j==msfFeature->width-1)
						fprintf(tr_file, "\n");
					else
						fprintf(tr_file, " ");
				}

				count++;			

				//cvSave("tmp.xml", msfFeature);
			}

			train_file.close();

			fclose(tr_file);


			
			count = 1;

			FILE *te_file;

			te_file = fopen(test_name.c_str(), "w+");
			//test_file = fopen("test_file", "r+");

			ifstream test_file(te_idx.c_str());

			while(!test_file.eof())
			{
				test_file>>i>>imgName;
				//cout<<i<<" "<<imgName.c_str()<<endl;
				if(i == pos_class)
					MARK = 1;
				else
					MARK = -1;
				fileName = projectPath + categories[i] +"\\"+imgName+".xml";

				//cout<<fileName.c_str()<<endl;
				CvMat * msfFeature;
				msfFeature = (CvMat *)cvLoad(fileName.c_str());

				//fprintf(te_file, "%d ", MARK);
				fprintf(tr_file, "%d ", i+1);
				
				for(j = 0; j< msfFeature->width; j++)
				{
					fprintf(te_file, "%d:%f", j+1,CV_MAT_ELEM(*msfFeature, float, 0, j));
					if(j==msfFeature->width-1)
						fprintf(te_file, "\n");
					else
						fprintf(te_file, " ");
				}

				count++;			

				//cvSave("tmp.xml", msfFeature);
			}
			test_file.close();
			fclose(te_file);
		}
	}
	return 0;
}

