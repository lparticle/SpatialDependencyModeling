#include "stdafx.h"
#include "colorprocessor.h"
#include "imageIterator.h"

	void ColorProcessor::
process(IplImage *image) {

  IplImageIterator<unsigned char> it(image);
  unsigned char* data;

  while (!it) {

     data= &it; // get pointer to current pixel

	 data[0]= data[0]/div * div + div/2;
	 data[1]= data[1]/div * div + div/2;
	 data[2]= data[2]/div * div + div/2;

     it+= 3; // next pixel
  }
}

