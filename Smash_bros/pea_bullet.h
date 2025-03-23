#ifndef PEA_BULLET_H
#define PEA_BULLET_H

#include"bullet.h"
#include"animation.h"
#include"resource.h"
#include"until.h"

extern IMAGE img_pea;
extern Atlas atlas_pea_break;

class PeaBullet:public Bullet
{
public:
	PeaBullet() 
	{
		size.x = 64, size.y = 64;
		damage = 10;

		animation_break.set_atlas(&atlas_pea_break);
		animation_break.set_interval(100);
		animation_break.set_loop(false);
		animation_break.set_callback([&]() {set_status(BulletStatus::Remove); });

	}
	~PeaBullet() = default;

	void on_collied()
	{
		Bullet::on_collied();
		switch (rand() % 3)
		{
		case 0:
			mciSendString(_T("play pea_break_1 from 0"), NULL, 0, NULL);
			break;
		case 1:
			mciSendString(_T("play pea_break_2 from 0"), NULL, 0, NULL);
			break;
		case 2:
			mciSendString(_T("play pea_break_3 from 0"), NULL, 0, NULL);
			break;
		}
	}
	void on_update(int delta)
	{
		switch (status)
		{
		case BulletStatus::Normal:
			position += velocity * (int)delta;
			break;
		case BulletStatus::Novalue:
			animation_break.on_update(delta);
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
			putimage_alpha(camera, (int)position.x, (int)position.y, &img_pea);
			break;
		case BulletStatus::Novalue:
			animation_break.on_draw(camera, (int)position.x, (int)position.y);
		default:
			break;
		}
	}
private:
	Animation animation_break;
	
};

#endif
