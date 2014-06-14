#ifndef _obj_h
#define _obj_h

extern float default_h;
extern float default_w;
extern float default_zoom;

extern int default_world_h;
extern int default_world_w;

class world
{
public:
	world ();

	int height;
	int width;
};

class camera
{
public:
	camera ();

	float center_x;
	float center_y;

	int height;
	int width;

	float zoom;
};

#endif
