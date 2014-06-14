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

const string source = "v.mp4";
const string destination = "simulation/vtest/";

void simulation_init ();

Mat take_pic ( const camera& cam, const float& global_t );

#endif
