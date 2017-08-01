#if !defined NEIGHBOR
#define NEIGHBOR

#include "cv.h"

class Neighbor {

  private:

	  // private attributes
	  int div;	  
	  CvMat *Pai;
	  CvMat * hist;
	  CvMat * msfFeature;

	  void calHist(IplImage *image);
	  void calMSF(IplImage *image);


	    	  // to check if an initialization is required
	  bool isInitialized() 
	  {
		  return Pai&&(Pai->height==div)
			  &&hist&&msfFeature;
	  }

	  // for all memory allocation
	  void initialize() 
	  {
			cvReleaseMat(&Pai);
			Pai= cvCreateMat( 1, div, CV_32FC1 );
			cvZero(Pai);

			cvReleaseMat(&hist);
			hist = cvCreateMat( 1, div, CV_32FC1 );//cvCreateHist( 1,&h_bins, CV_HIST_ARRAY, ranges, 1 );
			cvZero(hist);

			cvReleaseMat(&msfFeature);
			msfFeature= cvCreateMat( 1, 2*div, CV_32FC1 );
			cvZero(msfFeature);
	  }


	  	  //return MSF
  	  CvMat * getMsfMatrix() 
	  {
		  return Pai;
	  }

	  //return MSF
  	  CvMat* getHist() 
	  {
		  return hist;
	  }



  public:

	  // empty constructor
	  Neighbor( ) : div(256),Pai(0),hist(0),msfFeature(0) { } 

  	  // Getters and setters
	  void setBinNum(int n) 
	  {
		  div= n;
	  }


	   // Getters and setters
	  void setNumberOfColorsPerChannel(int n) 
	  {
		  div= 256/n;
	  }

	  CvMat* getMsfFeature() 
	  {
		  return msfFeature;
	  }


	  // the processing of the image
	  void process(IplImage *image);

	  // the method that checks for initilization
	  // and then process the image
	  inline void processImage(IplImage *image) 
	  {
		  if (!isInitialized()) 
		  {
			  initialize();
		  }
		  process(image);
	  }

	  // memory de-allocation
	  void release() 
	  {
		  cvReleaseMat(&hist);
		  cvReleaseMat(&Pai);
		  cvReleaseMat(&msfFeature);
	  }

	  ~Neighbor() 
	  {
		  release();
	  }
};

#endif
