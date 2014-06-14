#include <stdio.h>
#include <iostream>
#include "ImageWarpper.h"
#include "Patch.h"

using namespace std;

class Patch;

Image *ImageWarpper::resize(Image *im, float s, int method){
	return resize(im, s, s, method);
}

Image *ImageWarpper::resize(Image *im, float sh, float sw, int method){
	
	int newH = im->getHeight()*sh;
	int newW = im->getWidth()*sw;

	if(newH <= 0 || newW <= 0){
		cout << "Warning: Attemp to Resize a Image with scaling factor too small (h: " 
			<< sh << ", w: " << sw << ")." << endl;
		return NULL;
	}

	IplImage *ipl = cvCreateImage(cvSize(newW, newH), im->getDepth(), im->getNChannels());
	cvResize(im->img, ipl, method);

	return new Image(ipl);
}


Image *ImageWarpper::downSamplePSF(Image *im, float s, ImageFilter *psf, float shiftY, float shiftX){
	return downSamplePSF(im, s, s, psf, shiftY, shiftX);
}

Image *ImageWarpper::downSamplePSF(Image *im, float sh, float sw, ImageFilter *psf, float shiftY, float shiftX){

	if(sh > 1 || sw > 1)
		cout << "Warning: Attempt to Down-Sample a Image with factor (h: "
			<< sh << ", w: " << sw << ") greater than 1." << endl;

	int newH = im->getHeight()*sh;
	int newW = im->getWidth()*sw;
	int newCH = im->getNChannels();

	if(newH <= 0 || newW <= 0){
		cout << "Warning: Attempt to Down-Sample a Image with scaling factor too small (h: " 
			<< sh << ", w: " << sw << ")." << endl;
		return NULL;
	}

	IplImage *ipl = cvCreateImage(cvSize(newW, newH), im->getDepth(), newCH);
	Image *imL = new Image(ipl);

	float _sh = 1./sh; 
	float _sw = 1./sw;

	Patch *p = new Patch(im, psf->getWidth());
	for(int c = 0; c < newCH; c++){
		for(int y = 0; y < newH; y++){
			for(int x = 0; x < newW; x++){
				p->setCenter(float(y+0.5)*_sh+shiftY-0.5, float(x+0.5)*_sw+shiftX-0.5, c);
				/*
				char tmpChar[50];
				sprintf(tmpChar, "Data/Patches/Patch_%d_%d.png", y, x);
				p->SavePatch(tmpChar);
				*/
				imL->pixel(y, x, c) = (char)p->Convolve(psf);
			}
		}
	}
	delete p;

	return imL;
}
