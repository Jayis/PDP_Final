#ifndef _obj_h
#define _obj_h

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