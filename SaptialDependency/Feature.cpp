// Feature.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <string>
#include <windows.h>
#include <stdio.h>
#include <direct.h>
#include <fstream>


#include "cv.h"
#include "highgui.h"

#include "colorprocessor.h"
#include "gradient.h"
#include "neighborprocessor.h"
#include "imageIterator.h"

using namespace std;

int BINNUM;

void usage(const char* exec_name) {
   cout << "Usage: " << exec_name << " MSFtype imgfolder outfolder" << endl;
   cout << "<MSFtype>: Color - MSF-Color"
        << "           Edge	- MSF-Edge" << endl;
   cout << "<imgfolder>: the folder of the images to extract features" << endl;
   cout << "<aimfolder>:the folder of output files " << endl;
}

void ImgProcess(string featureType, string imgName, string outFolder)
{
	IplImage *image;                      // This is image pointer
	
	//cvShowImage("Original Image", image); // display it

	Neighbor processor;		// The image processor as an automatic variable

	if (featureType == "Color") 
	{
		image= cvLoadImage(imgName.c_str());             // load the image
		IplImage* hsv = cvCreateImage( cvGetSize( image ), image->depth, 3 );
		IplImage* h_plane = cvCreateImage( cvGetSize( image ), image->depth, 1 );
		IplImage* planes[] = { h_plane };
		

		cvCvtColor( image, hsv, CV_BGR2HSV );
		cvCvtPixToPlane( hsv, h_plane, 0, 0, 0 );

		
		Neighbor processor;
		processor.setBinNum(180);
		processor.processImage(h_plane);


		float maxValue = 0;

		int h_bins = 152;
		//int hist_size[] = {h_bins};
		float h_ranges[] = {0, 500}; /* hue varies from 0 (~0¡ãred) to 180 (~360¡ãred again) */
		float* ranges[] = { h_ranges };
		CvHistogram* hist;
		hist = cvCreateHist( 1,&h_bins, CV_HIST_ARRAY, ranges, 1 );
		cvCalcHist( planes, hist, 0, 0 );
		//cvSave("tmp.xml", hist);

		cvReleaseImage( &image );	

	}
	else if(featureType =="Edge")
	{
		//command: Edge D:\Dataset\scene_categories D:\Experiment\Edge_msf
		image= cvLoadImage(imgName.c_str());             // load the image
		IplImage* grayImage = cvCreateImage( cvGetSize( image ), image->depth, 1 );
		cvCvtColor( image, grayImage, CV_BGR2GRAY );
		
		IplImage* cannyImage = cvCreateImage( cvGetSize( image ), image->depth, 1 );
		cvCanny( grayImage, cannyImage, 50, 200, 3);

		cvAnd( grayImage, cannyImage, grayImage);
		
		IplImage* gradientImage;
		Gradient gradientProcessor;
		gradientProcessor.processImage(grayImage);
		gradientImage = gradientProcessor.getOutput();

		IplImageIterator<unsigned char> 
			  src(gradientImage,0,0,gradientImage->width,gradientImage->height);

		IplImage* s_image = cvCreateImage( cvSize(gradientImage->width,gradientImage->height), IPL_DEPTH_16U, 1 );
		IplImageIterator<unsigned short int> 
			  aim(s_image,0,0,s_image->width,s_image->height);

		 while (!src)
		 {
			  *aim = *src; // 
			  //cout << *src << endl; //
			  src++;
			  aim++;
		 } 

		//cvSave("tmp.xml", gradientProcessor.getOutput());
		//cvShowImage("Original Image", gradientProcessor.getOutput()); // display it
		//cvWaitKey(2);
		processor.setBinNum(64);
		processor.processImage(s_image);

		size_t lashidx = imgName.find_last_of("\\");
		string rawname = imgName.substr(lashidx+1,imgName.length());
		size_t dotidx = rawname.find_last_of(".");
		rawname = rawname.substr(0,dotidx);
		string outputfile = outFolder + "\\" + rawname +".xml";

		cvSave(outputfile.c_str(),processor.getMsfFeature());
 //   	cvSave(outputHistfile.c_str(),processor.getMsfFeature());


		cvReleaseImage( &grayImage );
		cvReleaseImage( &cannyImage );
		cvReleaseImage( &image );	
		cvReleaseImage( &s_image);	

	}
	else if(featureType =="SIFT")
	{

		// command: SIFT D:\Experiment\rawFormat_16_200 D:\Experiment\SIFT_16_200_msf\
	    //char * filename = "D:\\tmp.txt";

		ifstream i_file;
		string out_text;
		double tmpPixel;
		int width, length;

		IplImage* s_image;// cvCreateImage( CvSize size, int depth, int channels );

		 //
		i_file.open(imgName.c_str());
		if (i_file.is_open())
		{
			i_file >>length>> width;
			//cout << width<<" "<<length<<endl;

			s_image = cvCreateImage( cvSize(width, length), IPL_DEPTH_16U, 1 );
			IplImageIterator<unsigned short int> 
			  src(s_image,0,0,s_image->width,s_image->height);

			 while (!src)
			 {
				  i_file >> *src; // 
				  //cout << *src << endl; //
				  src++;
			 } 
			 
		}
		else
			cout << "Read£º" << imgName.c_str() << " Error£¡"; 
		i_file.close();

		//cvSave("D:\\tmp.xml", s_image);

		processor.setBinNum(BINNUM);
		processor.processImage(s_image);

		size_t lashidx = imgName.find_last_of("\\");
		string rawname = imgName.substr(lashidx+1,imgName.length());
		size_t dotidx = rawname.find_last_of(".");
		rawname = rawname.substr(0,dotidx);
		string outputfile = outFolder + "\\" + rawname +".xml";

		cvSave(outputfile.c_str(),processor.getMsfFeature());

		processor.release();
		cvReleaseImage( &s_image );
	}
	
	
}

int FileSearch(string featureType,string imgfolder, string aimfolder)
{
	int imgcount = 0;
	WIN32_FIND_DATAA fileData;
	string folderfiles = imgfolder + "\\*";

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
			subaimfolder = aimfolder +"\\"+(string)fileData.cFileName;

			attrib = GetFileAttributes(subaimfolder.c_str());
			if(attrib != FILE_ATTRIBUTE_DIRECTORY)
			{
				_mkdir(subaimfolder.c_str());				
			}
			cout << subaimfolder.c_str()<< endl;

			imgcount +=FileSearch(featureType, subimgfolder, subaimfolder);
			
		}
		else
		{
			// Obtain the file name
			// string filename fileData.cFileName;
			string cfilename = (string)fileData.cFileName;
			size_t dotidx = cfilename.find_last_of(".");
			string rawname = cfilename.substr(0,dotidx);
			string ext = cfilename.substr(dotidx+1,cfilename.length());
			if (ext != "jpg"&&ext != "vq")
				bProcess = false;

			if (bProcess) 
			{
				imgcount++;
				imgpath = imgfolder+"\\"+cfilename;
				ImgProcess(featureType, imgpath, aimfolder);

			}
		}
		bRepeat = FindNextFile(hFind, &fileData);
	}
	
    FindClose(hFind);

	return imgcount;
}


int _tmain(int argc, _TCHAR* argv[])
{
	if (argc != 5) 
	{
		usage(argv[0]);
		exit(0);
	}

	string featureType = (string)(argv[1]);
	string imgfolder = (string)(argv[2]);
	string aimfolder = (string)(argv[3]);
	BINNUM = atoi((argv[4]));

	//string imgfolder = "D:\\Dataset\\SceneClass13";
	//string aimfolder = "D:\\Experiment";

	//cvNamedWindow( "Original Image");   // create the window on which
    
	int imgcount;

	DWORD attrib = GetFileAttributes(aimfolder.c_str());

	if(attrib != FILE_ATTRIBUTE_DIRECTORY)
	{
		_mkdir(aimfolder.c_str());
	}

	imgcount = FileSearch(featureType,imgfolder,aimfolder);

	return 0;
}



