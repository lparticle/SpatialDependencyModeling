#include "stdafx.h"
#include "gradient.h"
#include "imageIterator.h"
#include "math.h"

#define PI 3.1415926
#define EPS 0.000001

void Gradient::
process(IplImage *image) {

  IplImageIterator<unsigned char> 
      src(image,1,1,image->width-2,image->height-2);
  //IplImageIterator<unsigned char> 
  //    res_mag(magnitudes,1,1,image->width-2,image->height-2);
  //IplImageIterator<unsigned char> 
  //    res_ori(orientations,1,1,image->width-2,image->height-2);
  IplImageIterator<unsigned char> 
      res_level(levelImage,1,1,image->width-2,image->height-2);

  double x,y;
  double ort;

  unsigned char* data= &src;

  while (!src) {

	  data= &src;
	  //if(data[0])
	  {
		  x= double(data[0]-src.neighbor(-1,0));
		  y= double(data[0]-src.neighbor(0,-1)); 
		  
		  //*res_mag= int(sqrt(double(x*x+y*y))/32);// > 255 ? 255 : mag;
		  ort = atan(double(y/(x+EPS)));

		  if(x<0)
		  {
			  ort = ort + PI;
		  }

		  if(ort<0)
		  {
			  ort = 2*PI +ort;
		  }

		  //*res_ori= int(ort/PI*4);
		  //int ort = ((int(sqrt(double(x*x+y*y))/32))*8+int(ort/PI*4))*4;
		  
		  *res_level = (int(sqrt(double(x*x+y*y)/2)/32))*8+int(ort/PI*4);
	  }

	  ++src;
	  ++res_level;
  }
}