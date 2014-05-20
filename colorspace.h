#ifndef __COLORSPACE__
#define __COLORSPACE__

#include <cv.h>
#include <iostream>
#include <stdio.h>

using namespace std;
// Create a YIQ image from the RGB image using an approximation of NTSC conversion(ref: "YIQ" Wikipedia page).
// Remember to free the generated YIQ image.
// Actually deal with BGC Images.
IplImage* convertImageRGBtoYIQ(const IplImage *imageRGB)
{
	float fR, fG, fB;
	float fY, fI, fQ;
	const float FLOAT_TO_BYTE = 255.0f;
	const float BYTE_TO_FLOAT = 1.0f / FLOAT_TO_BYTE;
	const float MIN_I = -0.5957f;
	const float MIN_Q = -0.5226f;
	const float Y_TO_BYTE = 255.0f;
	const float I_TO_BYTE = 255.0f / (MIN_I * -2.0f);
	const float Q_TO_BYTE = 255.0f / (MIN_Q * -2.0f);

	// Create a blank YIQ image
	IplImage *imageYIQ = cvCreateImage(cvGetSize(imageRGB), 8, 3);
	if (!imageYIQ || imageRGB->depth != 8 || imageRGB->nChannels != 3) {
		printf("ERROR in convertImageRGBtoYIQ()! Bad input image.\n");
		exit(1);
	}

	int h = imageRGB->height;			// Pixel height
	int w = imageRGB->width;			// Pixel width
	int rowSizeRGB = imageRGB->widthStep;		// Size of row in bytes, including extra padding.
	char *imRGB = imageRGB->imageData;		// Pointer to the start of the image pixels.
	int rowSizeYIQ = imageYIQ->widthStep;		// Size of row in bytes, including extra padding.
	char *imYIQ = imageYIQ->imageData;		// Pointer to the start of the image pixels.
	for (int y=0; y<h; y++) {
		for (int x=0; x<w; x++) {
			// Get the RGB pixel components. NOTE that OpenCV stores RGB pixels in B,G,R order.
			uchar *pRGB = (uchar*)(imRGB + y*rowSizeRGB + x*3);
			int bB = *(uchar*)(pRGB+0);	// Blue component
			int bG = *(uchar*)(pRGB+1);	// Green component
			int bR = *(uchar*)(pRGB+2);	// Red component

			// Convert from 8-bit integers to floats
			fR = bR * BYTE_TO_FLOAT;
			fG = bG * BYTE_TO_FLOAT;
			fB = bB * BYTE_TO_FLOAT;
			// Convert from RGB to YIQ,
			// where R,G,B are 0-1, Y is 0-1, I is -0.5957 to +0.5957, Q is -0.5226 to +0.5226.
			fY =    0.299 * fR +    0.587 * fG +    0.114 * fB;
			fI = 0.595716 * fR - 0.274453 * fG - 0.321263 * fB;
			fQ = 0.211456 * fR - 0.522591 * fG + 0.311135 * fB;
			// Convert from floats to 8-bit integers
			int bY = (int)(0.5f + fY * Y_TO_BYTE);
			int bI = (int)(0.5f + (fI - MIN_I) * I_TO_BYTE);
			int bQ = (int)(0.5f + (fQ - MIN_Q) * Q_TO_BYTE);

			// Clip the values to make sure it fits within the 8bits.
			if (bY > 255)
				bY = 255;
			if (bY < 0)
				bY = 0;
			if (bI > 255)
				bI = 255;
			if (bI < 0)
				bI = 0;
			if (bQ > 255)
				bQ = 255;
			if (bQ < 0)
				bQ = 0;

			// Set the YIQ pixel components
			uchar *pYIQ = (uchar*)(imYIQ + y*rowSizeYIQ + x*3);
			*(pYIQ+0) = bY;		// Y component
			*(pYIQ+1) = bI;		// I component
			*(pYIQ+2) = bQ;		// Q component
		}
	}
	return imageYIQ;
}

// Create an RGB image from the YIQ image using an approximation of NTSC conversion(ref: "YIQ" Wikipedia page).
// Remember to free the generated RGB image.
// Actually deal with BGC Images.
IplImage* convertImageYIQtoRGB(const IplImage *imageYIQ)
{
	float fY, fI, fQ;
	float fR, fG, fB;
	const float FLOAT_TO_BYTE = 255.0f;
	const float BYTE_TO_FLOAT = 1.0f / FLOAT_TO_BYTE;
	const float MIN_I = -0.5957f;
	const float MIN_Q = -0.5226f;
	const float Y_TO_FLOAT = 1.0f / 255.0f;
	const float I_TO_FLOAT = -2.0f * MIN_I / 255.0f;
	const float Q_TO_FLOAT = -2.0f * MIN_Q / 255.0f;

	// Create a blank RGB image
	IplImage *imageRGB = cvCreateImage(cvGetSize(imageYIQ), 8, 3);
	if (!imageRGB || imageYIQ->depth != 8 || imageYIQ->nChannels != 3) {
		printf("ERROR in convertImageYIQtoRGB()! Bad input image.\n");
		exit(1);
	}

	int h = imageYIQ->height;			// Pixel height
	int w = imageYIQ->width;			// Pixel width
	int rowSizeYIQ = imageYIQ->widthStep;		// Size of row in bytes, including extra padding.
	char *imYIQ = imageYIQ->imageData;		// Pointer to the start of the image pixels.
	int rowSizeRGB = imageRGB->widthStep;		// Size of row in bytes, including extra padding.
	char *imRGB = imageRGB->imageData;		// Pointer to the start of the image pixels.
	for (int y=0; y<h; y++) {
		for (int x=0; x<w; x++) {
			// Get the YIQ pixel components
			uchar *pYIQ = (uchar*)(imYIQ + y*rowSizeYIQ + x*3);
			int bY = *(uchar*)(pYIQ+0);	// Y component
			int bI = *(uchar*)(pYIQ+1);	// I component
			int bQ = *(uchar*)(pYIQ+2);	// Q component

			// Convert from 8-bit integers to floats
			fY = (float)bY * Y_TO_FLOAT;
			fI = (float)bI * I_TO_FLOAT + MIN_I;
			fQ = (float)bQ * Q_TO_FLOAT + MIN_Q;
			// Convert from YIQ to RGB
			// where R,G,B are 0-1, Y is 0-1, I is -0.5957 to +0.5957, Q is -0.5226 to +0.5226.
			fR =  fY  + 0.9563 * fI + 0.6210 * fQ;
			fG =  fY  - 0.2721 * fI - 0.6474 * fQ;
			fB =  fY  - 1.1070 * fI + 1.7046 * fQ;
			// Convert from floats to 8-bit integers
			int bR = (int)(fR * FLOAT_TO_BYTE);
			int bG = (int)(fG * FLOAT_TO_BYTE);
			int bB = (int)(fB * FLOAT_TO_BYTE);

			// Clip the values to make sure it fits within the 8bits.
			if (bR > 255)
				bR = 255;
			if (bR < 0)
				bR = 0;
			if (bG > 255)
				bG = 255;
			if (bG < 0)
				bG = 0;
			if (bB > 255)
				bB = 255;
			if (bB < 0)
				bB = 0;

			// Set the RGB pixel components. NOTE that OpenCV stores RGB pixels in B,G,R order.
			uchar *pRGB = (uchar*)(imRGB + y*rowSizeRGB + x*3);
			*(pRGB+0) = bB;		// B component
			*(pRGB+1) = bG;		// G component
			*(pRGB+2) = bR;		// R component
		}
	}
	return imageRGB;
}

IplImage* GetYImg(const IplImage *imageYIQ){

    IplImage *imageY = cvCreateImage(cvGetSize(imageYIQ), 8, 1);

    int h = imageYIQ->height;
	int w = imageYIQ->width;
	int YIQstep = imageYIQ->widthStep;
	int Ystep = imageY->widthStep;

    for(int i = 0; i < h; i++){
        for(int j = 0; j < w; j++){
            imageY->imageData[i*Ystep + j*imageY->nChannels] = imageYIQ->imageData[i*YIQstep + j*imageYIQ->nChannels];
        }
    }

    return imageY;
}
#endif
