#include <iostream>
#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "simulation.h"

using namespace std;
using namespace cv;

int main()
{
	const string source = "source.avi";
	const string img_seq_dir = "test/";

	//simulation_init( source , img_seq_dir);
	
	camera cam;
	cam.center_x = 3500;
	cam.center_y = 1600;
	cam.height = 360;
	cam.width = 480;
	cam.zoom =2;
	Mat img;
	img = take_pic ( cam, 0, img_seq_dir);
	namedWindow( "gg window", WINDOW_AUTOSIZE );
	imshow( "gg window", img ); 

	waitKey(0);
	return 0;
}