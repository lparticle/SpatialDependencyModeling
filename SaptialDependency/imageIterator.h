#if !defined IMGITERATOR
#define IMGITERATOR

template <class PEL>
class IplImageIterator {
 
  int i, j, i0;
  PEL* data;
  int step;
  int nl, nc;
  int nch;
 
 public:
 
  /* constructor */
  IplImageIterator(IplImage* image, 
     int x=0, int y=0, int dx= 0, int dy=0) : 
       i(x), j(y), i0(0) {
     
    data= reinterpret_cast<PEL*>(image->imageData); 
    step= image->widthStep / sizeof(PEL);
	CvRect rect= cvGetImageROI(image);
 
	nl= rect.height;
	nc= rect.width;
	x+= rect.x;
	y+= rect.y;

    if ((y+dy)>0 && (y+dy)<nl) nl= y+dy;
    if (y<0 || y>=nl) j=0;
    data+= step*j;
 
    if ((x+dx)>0 && (x+dx)<nc) nc= x+dx;
    nc*= image->nChannels;
    if (x>0 && x<nc) i0= x*image->nChannels;
    i= i0;
 
    nch= image->nChannels;
  }

  /* has next ? */
  bool operator!() const { return j < nl; }
 
  /* next pixel or next color component */
  IplImageIterator& operator++() {
	i++; 
    if (i >= nc) { 
		i=i0; 
		j++; 
		data+= step; 
	}
    return *this;
  }

  const IplImageIterator operator++(int) {
	IplImageIterator<PEL> copy(*this);
	++(*this);
	return copy;
  }

  IplImageIterator& operator+=(int s) {
	i+= s; 
    if (i >= nc) { 
		i=i0; 
		j++; 
		data+= step; 
	}
    return *this;
  }

  /* pixel access */
  PEL& operator*() { 
	  return data[i]; 
  }

  const PEL operator*() const { 
	  return data[i]; 
  }

  const PEL neighbor(int dx, int dy) const { 
	  return *(data+dy*step+i+dx*nch); 
  }

  PEL* operator&() const { 
	  return data+i; 
  }
 
  /* current pixel coordinates */
  int column() const { 
	  return i/nch; 
  }

  int line() const { 
	  return j; 
  }
}; 

template <class PEL>
class IplMultiImageIterator {
 
  int i, j, i0;
  PEL** data;
  int step;
  int nl, nc;
  int nch;
  int nimages;
 
 public:
 
  /* constructor */
  IplMultiImageIterator(IplImage** images, int n,
     int x=0, int y=0, int dx= 0, int dy=0) : 
       i(x), j(y), i0(0) {
     
	nimages= n;

	data= new PEL*[nimages];
	for (int i=0; i<nimages; i++) {

		data[i]= reinterpret_cast<PEL*>(images[i]->imageData); 
	}

    step= images[0]->widthStep / sizeof(PEL);
	CvRect rect= cvGetImageROI(images[0]);
 
	nl= rect.height;
	nc= rect.width;
	x+= rect.x;
	y+= rect.y;

    if ((y+dy)>0 && (y+dy)<nl) nl= y+dy;
    if (y<0 || y>=nl) j=0;

	for (int i=0; i<nimages; i++) {

		data[i]+= step*j;
	}
 
    if ((x+dx)>0 && (x+dx)<nc) nc= x+dx;
    nc*= images[0]->nChannels;
    if (x>0 && x<nc) i0= x*images[0]->nChannels;
    i= i0;
 
    nch= images[0]->nChannels;
  }

  ~IplMultiImageIterator() {

	  delete[] data;
  }

  /* has next ? */
  bool operator!() const { return j < nl; }
 
  /* next pixel or next color component */
  IplMultiImageIterator& operator++() {
	i++; 
    if (i >= nc) { 
		i=i0; 
		j++; 

		for (int i=0; i<nimages; i++) {

			data[i]+= step;
		}
	}
    return *this;
  }

  const IplMultiImageIterator operator++(int) {
	IplImageIterator<PEL> copy(*this);
	++(*this);
	return copy;
  }

  IplMultiImageIterator& operator+=(int s) {
	i+= s; 
    if (i >= nc) { 
		i=i0; 
		j++; 

		for (int i=0; i<nimages; i++) {
			data[i]+= step; 
		}
	}
    return *this;
  }

  /* pixel access */
  PEL& operator[](int n) { 
	  return data[n][i]; 
  }

  const PEL neighbor(int n, int dx, int dy) const { 
	  return *(data[n]+dy*step+i+dx*nch); 
  }
 
  /* current pixel coordinates */
  int column() const { 
	  return i/nch; 
  }

  int line() const { 
	  return j; 
  }
}; 

#endif
