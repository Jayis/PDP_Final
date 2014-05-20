#ifndef __IMAGE_WARPPER__
#define __IMAGE_WARPPER__

#include <cstdlib>
#include <cv.h>
#include <highgui.h>
#include "Image.h"
#include "ImageFilter.h"

using namespace std;

class Image;
class ImageFilter;

/*
 * This class is currently depending on the OpenCV image processing library.
 * 
 */

class ImageWarpper{

 public:
	// public member function
	ImageWarpper(){};
	~ImageWarpper(){};

	Image *resize(Image *im, float s, int method);
	Image *resize(Image *im, float sh, float sw, int method);
	Image *downSamplePSF(Image *im, float s, ImageFilter *psf, float shiftY=0, float shiftX=0);
	Image *downSamplePSF(Image *im, float sh, float sw, ImageFilter *psf, float shiftY=0, float shiftX=0);

	// public member data
};

#endif	
