#ifndef SUNFLOWER_H
#define SUNFLOWER_H

#include"camera.h"
#include<graphics.h>
#include"player.h"
#include<iostream>
#include"resource.h"
#include"sun_bullet.h"
#include"sun_bullet_ex.h"

extern Atlas atlas_sunflower_idle_left;
extern Atlas atlas_sunflower_idle_right;
extern Atlas atlas_sunflower_run_left;
extern Atlas atlas_sunflower_run_right;
extern Atlas atlas_sunflower_attack_ex_left;
extern Atlas atlas_sunflower_attack_ex_right;
extern Atlas atlas_sunflower_die_left;
extern Atlas atlas_sunflower_die_right;
extern Atlas atlas_sun_text;

extern Player* player_1;
extern Player* player_2;

class Sunflower:public Player
{
public:
	Sunflower()
	{
		animation_idle_left.set_atlas(&atlas_sunflower_idle_left);
		animation_idle_right.set_atlas(&atlas_sunflower_idle_right);
		animation_run_left.set_atlas(&atlas_sunflower_run_left);
		animation_run_right.set_atlas(&atlas_sunflower_run_right);
		animation_attack_ex_left.set_atlas(&atlas_sunflower_attack_ex_left);
		animation_attack_ex_right.set_atlas(&atlas_sunflower_attack_ex_right);
		animation_sun_text.set_atlas(&atlas_sun_text);

		animation_idle_left.set_interval(75);
		animation_idle_right.set_interval(75);
		animation_run_left.set_interval(75);
		animation_run_right.set_interval(75);
		animation_attack_ex_left.set_interval(100);
		animation_attack_ex_right.set_interval(100);
		animation_sun_text.set_interval(100);

		animation_attack_ex_left.set_loop(false);
		animation_attack_ex_right.set_loop(false);
		animation_sun_text.set_loop(false);

		animation_attack_ex_left.set_callback([&]() {is_attacking_ex = false; });
		animation_attack_ex_right.set_callback([&]() {is_attacking_ex = false; });

		animation_die_left.set_atlas(&atlas_sunflower_die_left);
		animation_die_left.set_loop(false);
		animation_die_left.set_interval(150);
		animation_die_right.set_atlas(&atlas_sunflower_die_right);
		animation_die_right.set_loop(false);
		animation_die_right.set_interval(150);

		timer_attack_cd.set_wait_time(250);

		size.x = 96;
		size.y = 96;
	}
	~Sunflower() = default;

	void on_update(int delta)
	{
		Player::on_update(delta);
		if(is_attacking_ex)
		{
			animation_sun_text.on_update(delta);
		}
	}
	void on_draw(const Camera& camera)
	{
		Player::on_draw(camera);
		if(is_attacking_ex)
		{
			Vector2 pos_text;
			IMAGE* image = animation_sun_text.get_frame();
			pos_text.x = position.x + (size.x - image->getwidth()) / 2;
			pos_text.y = position.y - image->getheight();
			const Vector2& vec = camera.get_position();
			animation_sun_text.on_draw(camera, (int)(pos_text.x - vec.x), (int)(pos_text.y - vec.y));
		}
	}
	void on_attack()
	{
		Bullet* bullet = new SunBullet;

		Vector2 pos;
		pos.x = position.x + (size.x - bullet->get_size().x) / 2;
		pos.y = position.y;

		bullet->set_position(pos);
		bullet->set_velocity(is_facing_right);
		bullet->set_collied_target(ID == PlayerID::P1 ? PlayerID::P2 : PlayerID::P1);
		bullet->set_callback([&]() {mp += 35;
			bullet->set_status(BulletStatus::Novalue); });
		bullet_list.push_back(bullet);

		mciSendString(_T("play pea_shoot_2 from 0"), NULL, 0, NULL);
	}
	void on_attack_ex()
	{
		is_attacking_ex = true;
		animation_sun_text.reset();
		is_facing_right ? animation_attack_ex_right.reset() : animation_attack_ex_left.reset();

		Bullet* bullet = new SunBulletEx;
		Player* target_player = ID == PlayerID::P1 ? player_2 : player_1;
		const Vector2& target_position = target_player->get_position();
		const Vector2& target_size = target_player->get_size();
		const Vector2& bullet_size = bullet->get_size();
		Vector2 pos;
		pos.x = target_position.x + (target_size.x - bullet_size.x) / 2;
		pos.y = -size.y;

		bullet->set_position(pos);
		bullet->set_collied_target(ID == PlayerID::P1 ? PlayerID::P2 : PlayerID::P1);
		bullet->set_callback([&]() {mp += 50; });
		bullet_list.push_back(bullet);

		mciSendString(_T("play sun_text from 0"), NULL, 0, NULL);
	}
private:
	Animation animation_sun_text;
};

#endif
