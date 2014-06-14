#include <iostream>
#include "Patch.h"

using namespace std;

Patch::Patch(Image *_im, int pw, float cy, float cx, int ch)
	    :im(_im), pwidth(pw)
{
	setCenter(cy, cx, ch);
}

int Patch::Convolve(ImageFilter *filter){

	if(pwidth != filter->getHeight() || pwidth != filter->getWidth()){
		cout << "Attempting to convolve a Patch with a Size-Unmatched Filter." << endl;
		exit(0);
	}
	float sum = 0;
	for(int y = 0; y < pwidth; y++){
		for(int x = 0; x < pwidth; x++){

			float newY=startY+y, newX=startX+x;
			if(newY < 0)
				newY = -newY;
			else if(newY > im->getHeight()-1)
				newY = 2*(im->getHeight()-1) - newY;
			if(newX < 0)
				newX = -newX;
			else if(newX > im->getWidth()-1)
				newX = 2*(im->getWidth()-1) - newX;

			if(im->validIndex(newY, newX))
				sum += ((*im)(newY, newX, channel)*(*filter)(y, x));
		}
	}
	if(sum > 255)
		sum = 255;
	else if(sum < 0)
		sum = 0;

	return (int)sum;
}

void Patch::setCenter(float y, float x){ 
	setCenter(y, x, channel);
}

void Patch::setCenter(float y, float x, int ch){ 
	if(im->validIndex(y, x, ch)){
		centerY = y; centerX = x; 
		startY = centerY - ((float)(pwidth-1))/2.;
		startX = centerX - ((float)(pwidth-1))/2.;
		channel = ch;
	}
}

int Patch::operator() (int y, int x) const{

	if(im->validIndex(float(startY+y), float(startX+x), channel))
		return (*im)(float(startY+y), float(startX+x), channel);
	else
		return -1;
}

void Patch::SavePatch(string fName){

	Image oimg(*im);

	for(int y = startY-3; y < startY + pwidth + 3; y++){
		for(int x = startX-3; x < startX + pwidth + 3; x++){
			if(oimg.validIndex(y, x))
				oimg.pixel(y, x) = char(0);
		}
	}

	for(int y = startY; y < startY + pwidth; y++){
		for(int x = startX; x < startX + pwidth; x++){
			if(oimg.validIndex(y, x))
				oimg.pixel(y, x) = (*im)(y, x);
		}
	}

	oimg.SaveImage(fName);
}

void Patch::SavePatch(string fName, int pR, int pG, int pB){

	if(im->getNChannels() != 3)
		return;

	Image oimg(*im);

	for(int y = startY-2; y < startY + pwidth + 2; y++){
		for(int x = startX-2; x < startX + pwidth + 2; x++){
			if(oimg.validIndex(y, x)){
				oimg.pixel(y, x, 0) = pB;
				oimg.pixel(y, x, 1) = pG;
				oimg.pixel(y, x, 2) = pR;
			}
		}
	}

	for(int y = startY; y < startY + pwidth; y++){
		for(int x = startX; x < startX + pwidth; x++){
			if(oimg.validIndex(y, x)){
				oimg.pixel(y, x, 0) = (*im)(y, x, 0);
				oimg.pixel(y, x, 1) = (*im)(y, x, 1);
				oimg.pixel(y, x, 2) = (*im)(y, x, 2);
			}
		}
	}

	*im = oimg;
	oimg.SaveImage(fName);
}

bool Patch::validPatch(){
	if( startY < 0 || startY+pwidth > im->getHeight() ||
	    startX < 0 || startX+pwidth > im->getWidth() )
		return false;
	else
		return true;
}
