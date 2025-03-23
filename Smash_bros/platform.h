#ifndef	PLATFORM_H
#define	PLATFORM_H

#include<graphics.h>
#include"camera.h"`
#include"until.h"

extern bool is_debug;

class Platform
{public:
	Platform() = default;
	~Platform() = default;
	void on_draw(Camera camera)const
	{
		putimage_alpha(camera, pos.x, pos.y, img);
		if (is_debug)
		{
			setlinecolor(RGB(255, 0, 0));
			line(camera, (int)shape.left, (int)shape.y, (int)shape.right, (int)shape.y);
		}
	}
public:
	struct Collisionshape
	{
		float y;
		float left, right;
	};
public:
	Collisionshape shape;
	IMAGE* img = nullptr;
	POINT pos = { 0 };
};

#endif
