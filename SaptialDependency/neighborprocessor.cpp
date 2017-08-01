#include "stdafx.h"
#include "neighborprocessor.h"
#include "imageIterator.h"

void Neighbor::process(IplImage *image) 
{
	calHist(image);
	calMSF(image);
    
	int i;
	for(i= 0; i<div; i++)
	{
	  CV_MAT_ELEM(*msfFeature, float, 0, i) = CV_MAT_ELEM( *hist, float, 0, i);
	  CV_MAT_ELEM(*msfFeature, float, 0, div+i) = CV_MAT_ELEM( *Pai, float, 0, i);
	}

  /*cvSave("formatTest.xml", finalMat);
  cvSave("histTest.xml", hist);*/

}


void Neighbor::calMSF(IplImage *image) 
{

  IplImageIterator<unsigned short int> 
      src(image,1,1,image->width-2,image->height-2);

  CvMat *cooccurrence = cvCreateMat( div, div, CV_32SC1 ); 
  CvMat *transMat = cvCreateMat( div, div, CV_32FC1 );
  CvMat *Pn;//= cvCreateMat( div, div, CV_32FC1 );
  CvMat *An= cvCreateMat( div, div, CV_32FC1 );
  cvZero(cooccurrence);
  cvZero(transMat);
  cvZero(An);
 // cvZero(Pn);
 
// c_ij = #(p_1=c_i, p_2=c_j| |p_1-p_2|=d )/2  Note, in practice /2 can be ignore. See the function below.
  while (!src) 
  {
	  int cpixel = src.neighbor(0,0);
	  CV_MAT_ELEM( *cooccurrence, int, cpixel, src.neighbor(-1,-1) ) ++;
	  //int element = CV_MAT_ELEM(*cooccurrence,int,cpixel,src.neighbor(-1,-1));
	  //printf("%d\n",element);
	  CV_MAT_ELEM( *cooccurrence, int, cpixel, src.neighbor( 0,-1) ) ++;
	  CV_MAT_ELEM( *cooccurrence, int, cpixel, src.neighbor( 1,-1) ) ++;
	  CV_MAT_ELEM( *cooccurrence, int, cpixel, src.neighbor(-1, 0) ) ++;
	  CV_MAT_ELEM( *cooccurrence, int, cpixel, src.neighbor( 0, 1) ) ++;
	  CV_MAT_ELEM( *cooccurrence, int, cpixel, src.neighbor(-1, 1) ) ++;
	  CV_MAT_ELEM( *cooccurrence, int, cpixel, src.neighbor( 0, 1) ) ++;
	  CV_MAT_ELEM( *cooccurrence, int, cpixel, src.neighbor( 1, 1) ) ++;

	  ++src;
  }

  
  int i, j;
  int sumJ;
//p_ij = c_ij /sum_j(c_ij)
  for(i=0; i<div; i++) 
  {
	  sumJ = 0;
	  for(j=0;j<div; j++)
	  {
		  sumJ += CV_MAT_ELEM( *cooccurrence, int, i, j);
		  //printf("%d\n", sumJ);
	  }

	  for(j=0;j<div; j++)
	  {
		 CV_MAT_ELEM(*transMat, float, i, j) = CV_MAT_ELEM( *cooccurrence, int, i, j)/float(sumJ+0.00001);
	  }
  }

  int N = 50;

  for(i=0; i<div; i++)
  {
      CV_MAT_ELEM( *An, float, i, i) = 1; 
  }

  Pn= cvCloneMat(transMat);

  for(i=1;i<N; i++)
  {
	  cvMul( Pn, transMat, Pn, 1 );
	  cvAdd( An, Pn, An, NULL );
  }

  cvConvertScale( An, An, 1.0/N, 0 );

  float tmpSum = 0;

  for(j=0; j<div; j++)
  {
	  for(i=0; i<div; i++)
	  {
		  CV_MAT_ELEM( *Pai, float, 0, j) +=  CV_MAT_ELEM( *An, float, i, j);
	  }
	  tmpSum += CV_MAT_ELEM( *Pai, float, 0, j);
  }

  for(j=0; j<div; j++)
  {
	  CV_MAT_ELEM( *Pai, float, 0, j) = CV_MAT_ELEM( *Pai, float, 0, j)/tmpSum;
  }

  cvReleaseMat(&cooccurrence);
  cvReleaseMat(&transMat);
  cvReleaseMat(&An);
  cvReleaseMat(&Pn);
}

void Neighbor::calHist(IplImage *image)
{

	IplImageIterator<unsigned short int> 
      src(image,0,0,image->width,image->height);

	int count = 0;
	 while (!src) 
	 {
		 CV_MAT_ELEM( *hist, float, 0, *src) ++;
		 count++;
		 src++;
	 }

	 int i;

	 for( i =0; i< div; i++)
	 {
		 CV_MAT_ELEM( *hist, float, 0, i) =  CV_MAT_ELEM( *hist, float, 0, i) /count;
	 }

}