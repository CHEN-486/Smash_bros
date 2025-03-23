#ifndef SUN_BULLET_EX_H
#define SUN_BULLET_EX_H

#include"bullet.h"
#include"animation.h"
#include"vector2.h"
#include"resource.h"
#include"camera.h"

extern Atlas atlas_sun_ex;
extern Atlas atlas_sun_ex_explode;

extern Camera main_camera;

class SunBulletEx :public Bullet
{
public:
	SunBulletEx() 
	{
		size.x = 288;
		size.y = 288;
		damage = 20;
		velocity.x = 0.0f;
		velocity.y = speed_sun_ex;

		animation_sun_ex.set_atlas(&atlas_sun_ex);
		animation_sun_ex.set_interval(50);

		animation_sun_ex_explode.set_atlas(&atlas_sun_explode);
		animation_sun_ex_explode.set_interval(50);
		animation_sun_ex_explode.set_loop(false);
		animation_sun_ex_explode.set_callback([&]() {set_status(BulletStatus::Remove); });

		IMAGE* frame_idle = animation_sun_ex.get_frame();
		IMAGE* frame_explode = animation_sun_ex_explode.get_frame();
		explode_render_offset.x = (frame_explode->getwidth() - frame_idle->getwidth()) / 2;
		explode_render_offset.y = (frame_explode->getheight() - frame_idle->getheight()) / 2;
	}
	~SunBulletEx() = default;

	void on_collied()
	{
		Bullet::on_collied();
		main_camera.shak(20, 350);
		mciSendString(_T("play sun_explode_ex from 0"), NULL, 0, NULL);
	}
	bool check_collied(const Vector2& position, const Vector2& size)
	{
		bool collied_x = (max(this->position.x + this->size.x, position.x + size.x) - min(this->position.x, position.x))
			<= (this->size.x + size.x);
		bool collied_y = (max(this->position.y + this->size.y, position.y + size.y) - min(this->position.y, position.y))
			<= (this->size.y + size.y);

		return collied_x && collied_y;
	}
	void on_update(int delta)
	{
		switch (status)
		{
		case BulletStatus::Normal:
			position += velocity * delta;
			animation_sun_ex.on_update(delta);
			break;
		case BulletStatus::Novalue:
			animation_sun_ex_explode.on_update(delta);
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
			animation_sun_ex.on_draw(camera, (int)position.x, (int)position.y);
			break;
		case BulletStatus::Novalue:
			animation_sun_ex_explode.on_draw(camera,
				(int)position.x - explode_render_offset.x, (int)position.y - explode_render_offset.y);
			break;
		default:
			break;
		}
	}
private:
	const float speed_sun_ex = 0.15f;
private:
	Animation animation_sun_ex;
	Animation animation_sun_ex_explode;
	Vector2 explode_render_offset;
};

#endif
