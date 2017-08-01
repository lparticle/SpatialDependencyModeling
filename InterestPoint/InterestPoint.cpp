// InterestPoint.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <windows.h>
#include <stdio.h>
#include <direct.h>


//#include "Img.h"
//#include "Definition.h"
#include "cv.h"
#include "highgui.h"
//#include "imshow.h"

#define MSER_EXE_FILE "mser.exe"
#define EXTRACT_FEATURES_EXE_FILE "extract_features.exe"

using namespace std;

void usage(const char* exec_name) 
{
	cout << "Usage: " << exec_name << " detector imgfolder aimfolder aimfolderMark parameter" << endl;
	cout << "<detector>:"
		<<"\n\t-harlap - harris-laplace detector"
        <<"\n\t-heslap - hessian-laplace detector"
		<<"\n\t-haraff - harris-affine detector"
		<<"\n\t-hesaff - hessian-affine detector"
		<<"\n\t-harhes - harris-hessian-laplace detector"
		<<"\n\t-sedgelap - edge-laplace detector"
	    <<"\n\t-mser	- MSER detector" << endl;
	cout << "<imgfolder>: the folder for the images to extract features" << endl;
	cout << "<aimfolder>: the folder for the output file " << endl;
	cout << "<aimfolderMark>: " 
		<<"\n0	save the output files in the same folder(aimfolder)"
		<<"\n1	keep subfolders as the imgfolder" 
		<< endl;
	cout << "<parameter>: inputs all the other parameters you want" <<endl;
	cout <<"\nfor MSER detectors"
		<<"\n\t-es (1.000) [1.000] ellipse scale, (output types 2 and 4)"
		<<"\n\t-per (0.010) [0.010] maximum relative area"
		<<"\n\t-ms (30) [30] minimum size of output region"
		<<"\n\t-mm (10) [10] minimum margin"
		<<"\n\t-rel (0) [0] use relative margins"
		<<"\n\t-v (0) [0] verbose output"
		<<"\n\t-t (0) [0] output file type 0 - RLE, 1 - ExtBound., 2 - Ellipse, 3 - GF, 4 -Aff."<< endl;
	cout<<"\nfor other detectors"
		<<"\n\t-jla  - steerable filters,  similarity="
		 <<"\n\t-sift - sift [D. Lowe],  similarity="
		 <<"\n\t-gloh - extended sift,  similarity="
		 <<"\n\t-mom  - moments,  similarity="
		 <<"\n\t-koen - differential invariants,  similarity="
		 <<"\n\t-cf   - complex filters [F. Schaffalitzky],  similarity="
		 <<"\n\t-sc   - shape context,  similarity=45000"
		 <<"\n\t-spin - spin,  similarity="
		 <<"\n\t-gpca - gradient pca [Y. Ke],  similarity="
		 <<"\n\t-cc - cross correlation,  similarity="
		 <<"\n\t-pca input.basis - projects the descriptors with pca basis"
		 <<"\n\t-p1 image.pgm.points - input regions format 1"
		 <<"\n\t-p2 image.pgm.points - input regions format 2"
		 <<"\n\t-o1 out.desc - saves descriptors in out.desc output format1"
		 <<"\n\t-o2 out.desc - saves descriptors in out.desc output format2"
		 <<"\n\t-noangle -computes rotation variant descriptors (no rotation esimation)"
		 <<"\n\t-DC - draws regions as circles in out.desc.png"
		 <<"\n\t-DR - draws regions as ellipses in out.desc.png"
		 <<"\n\t-c 255 - draws points in grayvalue [0,...,255]"<< endl;
}


int batchExtract(string detector, string imgfolder, string aimfolder, string subfoldermark="1", string parameter=NULL) 
{
	int imgcount = 0;
	string textOutput;
	/*int kpcount = 0;*/
	int iDIM = 0;
	WIN32_FIND_DATAA fileData;
	string folderfiles = imgfolder + "\\*.*";


	HANDLE hFind = FindFirstFile(folderfiles.c_str(), &fileData);
	bool bFinished = (hFind == INVALID_HANDLE_VALUE);
	int bRepeat = 1;

	string subimgfolder, subaimfolder;
	string imgpath;
	DWORD attrib;

	while (bRepeat && !bFinished) {
		bool bProcess = true;

		// If the current file is "." or "..", ignore
		if(strcmp(fileData.cFileName, ".") == 0 || 
			strcmp(fileData.cFileName, "..") == 0 ) 
		{
			bProcess = false;	
		}
		else if( (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) 
		// If the current file is a directory, ignore
		{
			subimgfolder = imgfolder +"\\"+(string)fileData.cFileName;

			if(subfoldermark =="1")
			{
				subaimfolder = aimfolder +"\\"+(string)fileData.cFileName;
				attrib = GetFileAttributes(subaimfolder.c_str());
				if(attrib != FILE_ATTRIBUTE_DIRECTORY)
				{
					_mkdir(subaimfolder.c_str());				
				}
				cout << subaimfolder.c_str()<< endl;
			}
			else
			{
				subaimfolder = aimfolder;
			}

			imgcount +=batchExtract(detector, subimgfolder, subaimfolder, subfoldermark, parameter);
			
		}
		else
		{
			// Obtain the file name
			// string filename fileData.cFileName;
			string cfilename = (string)fileData.cFileName;
			size_t dotidx = cfilename.find_last_of(".");
			string rawname = cfilename.substr(0,dotidx);
			string ext = cfilename.substr(dotidx+1,cfilename.length());
			string imgName, orgName;

			if(ext == "png"||ext == "pgm")
			{
				orgName = imgfolder+"\\"+cfilename;
				imgName = orgName ;
				bProcess = true;
			}else if (ext == "jpg"||ext == "bmp" ||ext =="gif")
			{
				orgName = imgfolder+"\\"+cfilename;
				imgName = orgName ;
				IplImage *image= cvLoadImage(imgName.c_str());
				cvSaveImage( "tmp.pgm", image );
				imgName = "tmp.pgm";
				bProcess = true;

			}else
			{
				bProcess = false;
			}

			if (bProcess) 
			{
				imgcount++;
				textOutput = aimfolder +"\\" + rawname + ".desc";
				if (detector == "-MSER"||detector == "-mser") {
					//! MSER feature
			
					string mser_cmd = (string)MSER_EXE_FILE + " -t 2 -es 2 -i " + 
						orgName + " -o " + " tmp.mser";

					string sift_cmd = (string)EXTRACT_FEATURES_EXE_FILE + " -i " + 
										imgName + " -p1 " + 
										"tmp.mser" + " -o1 " + textOutput + parameter;
					system(mser_cmd.c_str());
					system(sift_cmd.c_str());		
				}
				else if(detector == "-harlap" ||detector == "-heslap"||detector == "-haraff"
					||detector == "-hesaff"||detector == "-harhes"||detector == "-sedgelap")
				{
					//! Co-variant SIFT feature descriptor
					string cmd = (string)EXTRACT_FEATURES_EXE_FILE + " " +detector+" -i " + 
									imgName + " -o1 " + textOutput + parameter;
					system(cmd.c_str());					
				}

				else if(detector == "-dense" )
				{
					//! Co-variant SIFT feature descriptor
					string cmd = (string)EXTRACT_FEATURES_EXE_FILE + " " +detector+" -i " + 
									imgName + " -o1 " + textOutput + parameter;
					system(cmd.c_str());					
				}

				else
				{
					cout<<"\nWrong Command!!!\n"<<endl; 
					usage("InterestPoint");
					exit(0);
				}

			}
		}
		bRepeat = FindNextFile(hFind, &fileData);
	}
	
    FindClose(hFind);

	return imgcount;
}



int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 4) 
	{
		usage(argv[0]);
		exit(0);
	}

	string detector = (string)(argv[1]);
	string imgfolder = (string)(argv[2]);
	string ftxfolder = (string)(argv[3]);
	string folderMark = (string)(argv[4]);
	string parameter(" ");

	int count;

	for(count=5; count<argc; count++)
	{
		parameter = parameter + " " + (string)(argv[count]);
	}

	batchExtract(detector,imgfolder,ftxfolder,folderMark,parameter);
	//batchCombine(ftxfolder, output_file);
	return 0;
}

