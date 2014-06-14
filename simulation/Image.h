#ifndef __IMAGE__
#define __IMAGE__

#include <cstdlib>
#include <cv.h>
#include <highgui.h>
#include "ImageWarpper.h"

using namespace std;
using namespace cv;

class ImageWarpper;

/*
 * This class is currently depending on the OpenCV image processing library.
 * 
 */

class Image{

	friend class ImageWarpper;

 public:
	// public member function
	Image();
	Image(const Image &i);
	Image(const string&);
	Image(IplImage *);
	~Image(){ ReleaseImage(); }

	void ChangeImage(IplImage *);
	void LoadImage(const string&);
	void SaveImage(const string&);
	void ReleaseImage();

	// newI must be one channel image
	void setOneChannel(Image* newI, int);

	Image *getYIQfromBGR();
	Image *getBGRfromYIQ();
	Image *getOneChannel(int);
	Image *getBGRfromHSV();
	Image *getHSVfromBGR();

	int getHeight() const { return img->height; }
	int getWidth() const { return img->width; }
	int getNChannels() const { return img->nChannels; }
	int getDepth() const { return img->depth; }
	IplImage *getIplImg() const { return img; }

	bool validIndex(float, float, int=0) const;
	char & pixel(int y, int x, int ch=0); // fake operator, write pixel

	int operator() (int, int, int=0) const; // for accessing pixels, ReadOnly
	int operator() (float, float, int=0) const; // for accessing sub-pixel pixels

	Image & operator= (const Image &);
	// public member data

 private:
	void setImage(IplImage *); // Make sure setImage is the only one that will change "img" directly
	void init();

	IplImage *img;
};

#endif	
