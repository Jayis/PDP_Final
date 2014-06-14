#include <iostream>
#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "simulation/simulation.h"

using namespace std;
using namespace cv;

double eva(const camera& cam, const int& t)
{
	Mat img, img_Y;
	img = take_pic ( cam, t);
	//namedWindow( "gg window", WINDOW_AUTOSIZE );
	//imshow( "gg window", img ); 

	//IplImage* temp = new IplImage(img);
	//IplImage* tempYIQ = convertImageRGBtoYIQ (temp);
	//IplImage* tempY = GetYImg (tempYIQ);

	GaussianBlur( img, img, Size(3,3), 0, 0, BORDER_DEFAULT );
	cvtColor( img, img_Y, CV_RGB2GRAY );
	
	Mat grad;
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	// Scharr( src_gray, grad, ddepth, order_grad_x, order_grad_y, window, scale, delte, )
	Sobel( img_Y, grad_x, -1, 1, 0, 3, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);

	Sobel( img_Y, grad_y, -1, 0, 1, 3, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
	
	int i, j;
	double sum = 0;
	unsigned char* data = (unsigned char*)(grad.data);

	for (i = 0; i < grad.rows; i++){
		for (j = 0; j < grad.cols; j++){
			sum += (double)data[i*grad.cols + j]/255;
		}
	}

	return sum;
}
