#include <iostream>
#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "../eva.h"

#include "simulation.h"

using namespace std;
using namespace cv;

int main()
{
	//simulation_init( source , img_seq_dir);
	
	camera cam;
	cam.center_x = 778;
	cam.center_y = 1400;
	cam.height = 10;
	cam.width = 10;

	//float sum = eva(cam, 0);
	
	/*
	Mat img, img_Y;
	img = take_pic ( cam, 0);
	namedWindow( "gg window", WINDOW_AUTOSIZE );
	imshow( "gg window", img ); 

	printf("a\n");

	GaussianBlur( img, img, Size(3,3), 0, 0, BORDER_DEFAULT );
        cvtColor( img, img_Y, CV_RGB2GRAY );

	printf("b\n");

        Mat grad;
        Mat grad_x, grad_y;
        Mat abs_grad_x, abs_grad_y;

        // Scharr( src_gray, grad, ddepth, order_grad_x, order_grad_y, window, scale, delte, )
        Sobel( img_Y, grad_x, -1, 1, 0, 3, 1, 0, BORDER_DEFAULT);
        convertScaleAbs(grad_x, abs_grad_x);

	printf("c\n");

        Sobel( img_Y, grad_y, -1, 0, 1, 3, 1, 0, BORDER_DEFAULT);
        convertScaleAbs(grad_y, abs_grad_y);

	printf("d\n");

        addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

	printf("e\n");
        int i, j;
        double sum = 0;
        unsigned char* data = (unsigned char*)(grad.data);

        for (i = 0; i < grad.rows; i++){
                for (j = 0; j < grad.cols; j++){
                        sum += (double)data[i*grad.cols + j] / 255;
                }
        }

	printf("f\n");
	*/
	printf("sum:%f\n", sum);
	

	waitKey(0);
	return 0;
}
