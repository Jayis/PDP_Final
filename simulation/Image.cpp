#include <iostream>
#include "Image.h"
#include "colorspace.h"

using namespace std;

Image::Image(){ init(); }
Image::Image(const string& iname){ init(); LoadImage(iname); }
Image::Image(IplImage* i){ init(); setImage(i); }
Image::Image(const Image &i){ init(); *this = i; }

void Image::init(){

	setImage(NULL);
}

void Image::ChangeImage(IplImage *i){

	ReleaseImage();
	setImage(i);
}

void Image::LoadImage(const string& iname){

	ReleaseImage();
	IplImage *i = cvLoadImage(iname.c_str());
	if(!i){
		cerr << "Error Loading Image: " << iname << '\n';
		exit(0);
	}

	setImage(i);
}

void Image::SaveImage(const string& iname){

	if(!img){
		cerr << "Attempt to write out NULL Image: " << iname << '\n';
		exit(0);
	}
	cvSaveImage(iname.c_str(), img);
}


void Image::ReleaseImage(){

	if(img != NULL){
		cvReleaseImage(&img);
		setImage(NULL);
	}
}

void Image::setOneChannel(Image *newI, int ch){

	if(newI->getNChannels() != 1){
		cerr << "newI must be 1-channel-image." << endl;
		exit(0);
	}

	if(newI->getHeight() != img->height || newI->getWidth() != img->width){
		cerr << "setOneChannels: size not match." << endl;
		cerr << "newI(" << newI->getHeight() << ", " << newI->getWidth() << "), ";
		cerr << "orgI(" << img->height << ", " << img->width << ")" << endl;
		exit(0);
	}

	IplImage *nipl = newI->img;

	for(int y = 0; y < img->height; y++){
		for(int x = 0; x < img->width; x++){
			int idx = y*img->widthStep + x*img->nChannels + ch;
			int idx2 = y*nipl->widthStep + x;
			img->imageData[idx] = nipl->imageData[idx2];
		}
	}
}

Image *Image::getHSVfromBGR(){

	IplImage *hsvI = cvCreateImage(cvGetSize(img), 8, 3);
	cvCvtColor(img, hsvI, CV_BGR2HSV);
	return (new Image(hsvI));
}

Image *Image::getBGRfromHSV(){

	IplImage *bgrI = cvCreateImage(cvGetSize(img), 8, 3);
	cvCvtColor(img, bgrI, CV_HSV2BGR);
	return (new Image(bgrI));
}

Image *Image::getYIQfromBGR(){

	IplImage *ipl = convertImageRGBtoYIQ(img);
	return (new Image(ipl));
}

Image *Image::getBGRfromYIQ(){

	IplImage *ipl = convertImageYIQtoRGB(img);
	return (new Image(ipl));
}

Image *Image::getOneChannel(int ch){

	if(ch < 0 || ch >= img->nChannels){
		cerr << "Index out of bounds, try to access Channel: " << ch << endl;
		exit(0);
	}

	// copy the (ch+1)-th channel's value to the new image

	IplImage *ipl = cvCreateImage(cvGetSize(img), img->depth, 1);
	for(int y = 0; y < img->height; y++){
		for(int x = 0; x < img->width; x++){
			int idx1 = y*ipl->widthStep + x*ipl->nChannels;
			int idx2 = y*img->widthStep + x*img->nChannels + ch;
			ipl->imageData[idx1] = img->imageData[idx2];
		}
	}
	return (new Image(ipl));
}

char & Image::pixel(int y, int x, int ch) {

	/*if(!validIndex(y, x, ch))
		exit(0);
	*/

	int index = y*(img->widthStep) + x*(img->nChannels) + ch;
	return img->imageData[index];
}

// private member functions

void Image::setImage(IplImage *i){

	// i might be NULL
	img = i;
}

bool Image::validIndex(float y, float x, int ch) const {

	if(y < 0 || x < 0 || y > img->height-1 || x > img->width-1){
		//cerr << "Index out of bounds, try to access Position: " << "( " <<  y << ", " << x << " ) " << endl;
		//exit(0);
		return false;
	}
	else if(ch >= img->nChannels || ch < 0){
		//cerr << "Index out of bounds, try to access Channel: " << ch << endl;
		//exit(0);
		return false;
	}
	return true;
}

// Operators

int Image::operator() (int y, int x, int ch) const {

	/*
	if(!validIndex(y, x, ch))
		exit(0);
		//return -1;
	*/

	int index = y*(img->widthStep) + x*(img->nChannels) + ch;
	return (int)(unsigned char)img->imageData[index];
}

// using bilinear interpolation to get the pixel value of (x, y)

int Image::operator() (float y, float x, int ch) const {

	//return (*this)((int)y, (int)x, ch);
	/*
	if(!validIndex(y, x, ch))
		exit(0);
		//return -1;
	*/

	int y0 = (int)y;  int y1 = y0+1;
	int x0 = (int)x;  int x1 = x0+1;

	if(y1 >= img->height)
		y1 = y0;
	if(x1 >= img->width)
		x1 = x0;

	float alphaY = y - y0;
	float alphaX = x - x0;

	int index0 = y0*(img->widthStep) + x0*(img->nChannels) + ch;
	int index1 = y0*(img->widthStep) + x1*(img->nChannels) + ch;
	float interXy0 = (1-alphaX)*((float)(uchar)img->imageData[index0]) + 
		alphaX*((float)(uchar)img->imageData[index1]);

	index0 = y1*(img->widthStep) + x0*(img->nChannels) + ch;
	index1 = y1*(img->widthStep) + x1*(img->nChannels) + ch;
	float interXy1 = (1-alphaX)*((float)(uchar)img->imageData[index0]) + 
		alphaX*((float)(uchar)img->imageData[index1]);

	return  (int)cvRound((1-alphaY)*interXy0 + alphaY*interXy1);
}

Image & Image::operator= (const Image & i) {

	if(this == &i)
		return *this;

	ReleaseImage();

	if(i.img != NULL){
		IplImage *ipl = cvCreateImage(cvGetSize(i.img), i.img->depth, i.img->nChannels);
		memcpy(ipl->imageData, i.img->imageData, i.img->imageSize);
		setImage(ipl);
	}

	return *this;
}
