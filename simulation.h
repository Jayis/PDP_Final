#ifndef _simu_h
#define _simu_h

#include <iostream>
#include <string>
#include <cmath>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "obj.h"
#include "misc.h"
#include "ImageFilter.h"
#include "ImageWarpper.h"
#include "Image.h"

using namespace std;
using namespace cv;

void simulation_init ( const string& srcvideo, const string& destination );

Mat take_pic ( const camera& cam, const float& global_t, const string& destination );

#endif