#ifndef __IMAGE_FILTER__
#define __IMAGE_FILTER__

#include <cstdlib>
#include "Image.h"

using namespace std;

class Image;

/*
 * This class is currently depending on the OpenCV image processing library.
 *
 */

class ImageFilter{

 public:
	// public member function
	ImageFilter(){};
	ImageFilter(int h, int w);
	virtual ~ImageFilter(){ filterRelease(); };

	int getHeight(){ return height; }
	int getWidth(){ return width; }

	float operator() (int, int) const; // for accessing pixels, ReadOnly
	// public member data


 protected:
	//protected member function
	void filterAlloc(int h, int w);
	void filterRelease();

	virtual void fill(); // fill up the filter by f(y,x)
	virtual float f(float x, float y){ return 0; };

	// protected member data
	float **filter;
	int height;
	int width;
};

class GaussianFilter : public ImageFilter{

 public:
	// public member function
	GaussianFilter(int h, int w, float v, float sftX=0, float sftY=0);
	virtual ~GaussianFilter(){ filterRelease(); };

 private:
	//private member function
	virtual float f(float y, float x);
	float var;
	float shiftX , shiftY;
};

#endif
