#if !defined GRADIENT
#define GRADIENT

#include "cv.h"

class Gradient {

  private:

	  // private attributes
	  IplImage *levelImage;

  public:

	  // empty constructor
	  Gradient() : levelImage(0){ } 

	  IplImage* getOutput() {
		  return levelImage;
	  }

	  // to check if an initialization is required
	  bool isInitialized(IplImage *image) 
	  {
		  return levelImage && (levelImage->width == image->width) 
			            && (levelImage->height == image->height);
	  }

	  // for all memory allocation
	  void initialize(IplImage *image) 
	  {
		  cvReleaseImage(&levelImage);
		  levelImage= cvCreateImage(cvSize(image->width,image->height),
										image->depth, 1);
		  cvZero(levelImage);
	  }

	  // the processing of the image
	  void process(IplImage *image);

	  // the method that checks for initilization
	  // and then process the image
	  inline void processImage(IplImage *image) {
		  if (!isInitialized(image)) {
			  initialize(image);
		  }
		  process(image);
	  }

	  // memory de-allocation
	  void release() 
	  {
		  cvReleaseImage(&levelImage);
	  }

	  ~Gradient() 
	  {
		  release();
	  }
};

#endif
