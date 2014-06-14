#include <iostream>
#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "simulation/simulation.h"

using namespace std;
using namespace cv;

double eva2(const camera& cam, const int& t, Mat* img, Mat* img_Y, const camera& last_cam, const Mat* last_imgY)
{
	*img = take_pic ( cam, t);
	//namedWindow( "gg window", WINDOW_AUTOSIZE );
	//imshow( "gg window", img ); 

	//IplImage* temp = new IplImage(img);
	//IplImage* tempYIQ = convertImageRGBtoYIQ (temp);
	//IplImage* tempY = GetYImg (tempYIQ);

	////////////////////////////// static
	GaussianBlur( *img, *img, Size(3,3), 0, 0, BORDER_DEFAULT );
	cvtColor( *img, *img_Y, CV_RGB2GRAY );
	
	Mat grad;
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	// Scharr( src_gray, grad, ddepth, order_grad_x, order_grad_y, window, scale, delte, )
	Sobel( *img_Y, grad_x, -1, 1, 0, 3, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);

	Sobel( *img_Y, grad_y, -1, 0, 1, 3, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
	
	int i, j;
	double static_sum = 0;
	unsigned char* data = (unsigned char*)(grad.data);
	for (i = 0; i < grad.rows; i++){
		for (j = 0; j < grad.cols; j++){
			static_sum += (double)data[i*grad.cols + j]/255;
		}
	}
	
	///////////////////////////// motion
	double motion_sum = 0;
	unsigned char* last_data = (unsigned char*)((*last_imgY).data);
	unsigned char* cur_data = (unsigned char*)((*imgY).data);
	int shift_x = (int)last_cam.center_x - (int)cam.center_x;
	int shift_y = (int)last_cam.center_y - (int)cam.center_y;
	for (i = 0; i < (*img).rows; i++){
		if ((i + shift_y) < 0 || (i + shift_y) >= (*img.rows)) continue;
		for (j = 0; j < (*img).cols; j++){
			if ((j + shift_x) < 0 || (j + shift_x) >= (*img.cols)) continue;
			change = (double)(cur_data[i*(*img).cols + j] - last_data[(i + shift_y)*(*img).cols + (j + shift_x)]);
			if (change < 0) change = -change;
			motion_sum += change*100;
		}
	}
	
	

	return static_sum + motion_sum;
}

double eva(const camera& cam, const int& t, Mat* img)
{
	*img = take_pic ( cam, t);
	//namedWindow( "gg window", WINDOW_AUTOSIZE );
	//imshow( "gg window", img ); 

	//IplImage* temp = new IplImage(img);
	//IplImage* tempYIQ = convertImageRGBtoYIQ (temp);
	//IplImage* tempY = GetYImg (tempYIQ);

	////////////////////////////// static
	GaussianBlur( *img, *img, Size(3,3), 0, 0, BORDER_DEFAULT );
	cvtColor( *img, *img_Y, CV_RGB2GRAY );
	
	Mat grad;
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	// Scharr( src_gray, grad, ddepth, order_grad_x, order_grad_y, window, scale, delte, )
	Sobel( *img_Y, grad_x, -1, 1, 0, 3, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);

	Sobel( *img_Y, grad_y, -1, 0, 1, 3, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
	
	int i, j;
	double static_sum = 0;
	unsigned char* data = (unsigned char*)(grad.data);
	for (i = 0; i < grad.rows; i++){
		for (j = 0; j < grad.cols; j++){
			static_sum += (double)data[i*grad.cols + j]/255;
		}
	}
	
	return static_sum;
}