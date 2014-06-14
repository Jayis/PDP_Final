#include "simulation.h"

void simulation_init ( const string& srcvideo, const string& destination )
{
	VideoCapture inputVideo( destination + srcvideo );
	if (! inputVideo.isOpened() ) {
		cout << "Can't open Video : " << srcvideo << endl;
		return ;
	}

	Mat frame;
	int num_frame = 0;
	string temp_save_dir;
	
	for (num_frame=0; num_frame>-1; num_frame++) {
		inputVideo >> frame;
		if ( frame.empty() ) break;

		temp_save_dir = destination + num2string(num_frame) + ".jpg";

		imwrite ( temp_save_dir, frame);
	}

}

Mat take_pic ( const camera& cam, const float& global_t, const string& destination )
{
	int frame_idx = (global_t * 24);
	float center_x = int (cam.center_x) + 0.5;
	float center_y = int (cam.center_y) + 0.5;
	int L = (center_x + 0.5) - cam.width * cam.zoom / 2;
	int U = (center_y + 0.5) - cam.height * cam.zoom / 2;


	Mat frame = imread ( destination + num2string ( frame_idx ) + ".jpg", CV_LOAD_IMAGE_COLOR);

	Mat sub_img_temp1 = frame( Rect (L, U, cam.width * cam.zoom, cam.height * cam.zoom)).clone();

	IplImage* sub_img_temp2 = new IplImage(sub_img_temp1);
	
	Image* sub_img = new Image(sub_img_temp2);

	float v = log(cam.zoom) / 1.33886;
	GaussianFilter gss(9, 9, v);
	ImageWarpper wp;

	Image *down = wp.downSamplePSF( sub_img, 1/cam.zoom, &gss, 0, 0);
	//down->SaveImage ("zoom_2_diff4.jpg");
	
	Mat down_mat( down->getIplImg(), 1);
	
	return down_mat;
}