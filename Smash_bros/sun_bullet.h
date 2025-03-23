#ifndef SUN_BULLET_H
#define SUN_BULLET_H

#include"bullet.h"
#include"resource.h"
#include"animation.h"
#include"vector2.h"

extern Atlas atlas_sun;
extern Atlas atlas_sun_explode;

extern Camera main_camera;

class SunBullet :public Bullet
{
public:
	SunBullet() 
	{
		size.x = 96;
		size.y = 96;
		damage = 20;
		velocity = { 0.25f,-0.5f };

		animation_sun.set_atlas(&atlas_sun);
		animation_sun.set_interval(50);

		animation_sun_explode.set_atlas(&atlas_sun_explode);
		animation_sun_explode.set_interval(50);
		animation_sun_explode.set_loop(false);
		animation_sun_explode.set_callback([&]() {set_status(BulletStatus::Remove); });

		IMAGE* frame_idle = animation_sun.get_frame();
		IMAGE* frame_explode = animation_sun_explode.get_frame();
		explode_render_offset.x = (frame_explode->getwidth() - frame_idle->getwidth()) / 2;
		explode_render_offset.y = (frame_explode->getheight() - frame_idle->getheight()) / 2;
	}
	~SunBullet() = default;

	void on_collied()
	{
		Bullet::on_collied();
		main_camera.shak(5, 250);
		mciSendString(_T("play sun_explode from 0"), NULL, 0, NULL);
	}
	void on_update(int delta)
	{
		switch (status)
		{case BulletStatus::Normal:
			velocity.y += gravity * delta;
			position += velocity * (float)delta;
			animation_sun.on_update(delta);
			break;
		case BulletStatus::Novalue:
			animation_sun_explode.on_update(delta);
			break;
		default:
			break;	
		}
		if (check_if_exceed_screen())
			set_status(BulletStatus::Remove);
	}
	void on_draw(const Camera& camera)const
	{
		switch (status)
		{
		case BulletStatus::Normal:
			animation_sun.on_draw(camera, (int)position.x, (int)position.y);
			break;
		case BulletStatus::Novalue:
			animation_sun_explode.on_draw(camera, 
				(int)(position.x - explode_render_offset.x), 
				(int)position.y - explode_render_offset.y);
			break;
		default:
			break;  
		}
	}

private:
	const float gravity = 1e-3f;
private:
	Animation animation_sun;
	Animation animation_sun_explode;
	Vector2 explode_render_offset;
};
#endif
