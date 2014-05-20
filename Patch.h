#ifndef __PATCH__
#define __PATCH__

#include "Image.h"
#include "ImageFilter.h"

using namespace std;

class Image;
class ImageFilter;

/*
 * This class is currently depending on the OpenCV image processing library.
 * 
 */

class Patch{

 public:
	// public member function
	Patch(Image *_im, int pw, float cy=0, float cx=0, int ch=0);
	~Patch(){}

	void ChangeImage(Image *_im){ im = _im; }; // no need to release
	int Convolve(ImageFilter *filter);
	
	void setCenter(float y, float x);
	void setCenter(float y, float x, int ch);

	int getWidth(){ return pwidth; }
	int getChannel(){ return channel; }
	float getCenterX(){ return centerX; }
	float getCenterY(){ return centerY; }
	float getIdxX(int x){ return startX + x; }
	float getIdxY(int y){ return startY + y; }

	void SavePatch(string fName);
	void SavePatch(string fName, int pR, int pG, int pB);
	bool validPatch();

	int operator() (int, int) const; // for accessing patch cells, ReadOnly
	//Image & operator= (const Image &);
	// public member data

 private:

	Image *im;
	float centerX, centerY;
	int channel;
	float startX, startY;
	int pwidth;
};

#endif	
