#include <iostream>
#include "ImageFilter.h"
#define PI 3.1415926

using namespace std;

// public member functions
ImageFilter::ImageFilter(int h, int w):height(h), width(w) {

	if(h <= 0 || w <= 0){
		cout << "Attempt to allocate Filter with (h: "
			<< h << ", w: " << w << ")" << endl;
		exit(0);
	}

	filter = NULL;
	filterRelease();
	filterAlloc(h, w);
	fill();
}

void ImageFilter::filterAlloc(int h, int w){

	filter = new float*[h];
	filter[0] = new float[h*w];
	for(int i = 1; i < h; i++)
		filter[i] = filter[0] + i*w;
}

void ImageFilter::filterRelease(){

	if(filter != NULL){
		delete filter[0];
		delete filter;
		filter = NULL;
	}
}

void ImageFilter::fill(){

	int h = height; int w = width;

	float sum = 0.;
	for(int y = 0; y < h; y++){
		for(int x = 0; x < w; x++){
			filter[y][x] = f(x, y);
			sum += filter[y][x];
		}
	}
	if(sum != 0){
        for(int y = 0; y < h; y++){
            for(int x = 0; x < w; x++){
                filter[y][x] /= sum;
            }
        }
	}
}

float ImageFilter::operator() (int y, int x) const{
	return filter[y][x];
}

// GaussianFilter member functions

GaussianFilter::GaussianFilter(int h, int w, float v, float sftX, float sftY){

	if(h <= 0 || w <= 0){
		cout << "Attempt to allocate GaussianFilter with (h: "
			<< h << ", w: " << w << ")" << endl;
		exit(0);
	}
	height = h;
	width = w;
	var = v;
	if (var < 10e-7) var = 10e-7;
	shiftX = sftX;
	shiftY = sftY;

	filter = NULL;
	filterRelease();
	filterAlloc(h, w);
	fill();
}

float GaussianFilter::f(float x, float y){

	x -= (float)(width-1)/2. + shiftX;
	y -= (float)(height-1)/2. + shiftY;
	return (1./(2*PI*var) * exp(-((float)(x*x + y*y))/(2*var)));
}
