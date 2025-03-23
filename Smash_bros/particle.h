#ifndef PARTICLE_H
#define PARTICLE_H

#include"vector2.h"
#include"atlas.h"
#include"camera.h"
#include"until.h"

class Particle
{
public:
	Particle() = default;
	Particle(const Vector2& pos,Atlas* atlas,int lifespan)
		:lifespan(lifespan),position(pos),atlas(atlas){}
	~Particle() = default;

	void set_position(const Vector2& pos)
	{
		position = pos;
	}
	void set_atlas(Atlas* atlas)
	{
		this->atlas = atlas;
	}
	void set_lifespan(int lifespan)
	{
		this->lifespan = lifespan;
	}
	bool check_valid()const
	{
		return valid;
	}
	void on_update(int delta)
	{
		timer += delta;
		if(timer>=lifespan)
		{
			timer = 0;
			++idx_frame;
			if(idx_frame>=atlas->get_size())
			{
				valid = false;
				idx_frame = atlas->get_size() - 1;
			}
		}
	}
	void on_draw(const Camera& camera)const
	{
		putimage_alpha(camera, (int)position.x, (int)position.y, atlas->get_image(idx_frame));
	}
private:
	int timer = 0;
	int lifespan = 0;
	int idx_frame = 0;
	Vector2 position;
	Atlas* atlas = nullptr;
	bool valid = true;
};

#endif
