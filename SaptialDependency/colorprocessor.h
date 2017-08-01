#if !defined CPROCESSOR
#define CPROCESSOR

#include "cv.h"
#include "highgui.h"

class ColorProcessor {

  private:

	  // private attributes
	  int div; 

  public:

	  // empty constructor
	  ColorProcessor() : div(64) { 

		  // default parameter initialization here
	  }

	  // Getters and setters
	  void setNumberOfColorsPerChannel(int n) {

		  div= 256/n;
	  }

	  int getNumberOfColorsPerChannel() {

		  return 256/div;
	  }

	  // the processing of the image
	  void process(IplImage *image);
};


#endif
