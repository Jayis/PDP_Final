#include "obj.h"

float default_h = 360;
float default_w = 480;
float default_zoom = 1;

int default_world_h = 1080;
int default_world_w = 1920;


world::world ()
{
	width = default_world_w;
	height = default_world_h;
}

camera::camera ()
{
	zoom = default_zoom;

	center_x = 250;
	center_y = 200;

	width = default_w;
	height = default_h;
}
