#ifndef PLAYER_H
#define PLAYER_H

#include"camera.h"
#include<graphics.h>
#include"vector2.h"
#include"animation.h"
#include"platform.h"
#include"playerid.h"
#include"bullet.h"
#include"vector"
#include"timer.h"
#include"bulletstatus.h"
#include"particle.h"

extern std::vector<Platform> platform_list;
extern Atlas atlas_run_effect;
extern Atlas atlas_jump_effect;
extern Atlas atlas_land_effect;

class Player
{
public:
	Player()
	{
		timer_attack_cd.set_wait_time(attack_cd);
		timer_attack_cd.set_one_shot(true);
		timer_attack_cd.set_callback([&]() {can_attack = true; });

		timer_involnerable.set_wait_time(750);
		timer_involnerable.set_one_shot(true);
		timer_involnerable.set_callback([&]() {is_involnerable = false; });

		timer_involnerable_blink.set_wait_time(75);
		timer_involnerable_blink.set_callback([&]() {is_show_sketch_frame = !is_show_sketch_frame; });

		timer_run_effect_generation.set_wait_time(75);
		timer_run_effect_generation.set_callback([&]()
			{
				Vector2 pos_particle;
				IMAGE* image = atlas_run_effect.get_image(0);
				pos_particle.x = position.x + (size.x - image->getwidth()) / 2;
				pos_particle.y = position.y + size.y - image->getheight();
				particle_list.emplace_back(pos_particle, &atlas_run_effect, 45);

			});

		timer_die_effect_generation.set_wait_time(35);
		timer_die_effect_generation.set_callback([&]()
			{
				Vector2 pos_particle;
				IMAGE* image = atlas_run_effect.get_image(0);
				pos_particle.x = position.x + (size.x - image->getwidth()) / 2;
				pos_particle.y = position.y + size.y - image->getheight();
				particle_list.emplace_back(pos_particle, &atlas_run_effect, 150);

			});

		animation_jump_effect.set_atlas(&atlas_jump_effect);
		animation_jump_effect.set_loop(false);
		animation_jump_effect.set_interval(25);
		animation_jump_effect.set_callback([&]() {is_jump_effect_visible = false; });
		animation_land_effect.set_atlas(&atlas_land_effect);
		animation_land_effect.set_loop(false);
		animation_land_effect.set_interval(25);
		animation_land_effect.set_callback([&]() {is_land_effect_visible = false; });

	}
	~Player() = default;

	virtual void on_update(int delta)
	{
		int direction = is_right_down - is_left_down;
		if (direction != 0)
		{
			if (!is_attacking_ex)
				is_facing_right = direction > 0;
			current_animation = is_facing_right ? &animation_run_right : &animation_run_left;
			float distance = direction * run_velocity * delta;
			on_run(distance);
		}
		else
		{
			current_animation = is_facing_right ? &animation_idle_right : &animation_idle_left;
			timer_run_effect_generation.pause();
		}
		if (is_attacking_ex)
			current_animation = is_facing_right ? &animation_attack_ex_right : &animation_attack_ex_left;

		if(!can_attack)
		{
			timer_attack_cd.on_update(delta);
		}
		if (is_involnerable)
		{
			timer_involnerable.on_update(delta);
			timer_involnerable_blink.on_update(delta);
		}
		timer_run_effect_generation.on_update(delta);
		if (hp <= 0)
			timer_die_effect_generation.on_update(delta);

		particle_list.erase(std::remove_if(
			particle_list.begin(), particle_list.end(),
			[](const Particle& particle)
			{return !particle.check_valid(); }
		), particle_list.end()); 
		for (Particle& particle : particle_list)
			particle.on_update(delta);

		bullet_list.erase(std::remove_if(
			bullet_list.begin(), bullet_list.end(),
			[](const Bullet* bullet)
			{bool deletable = bullet->check_can_remove();
		if (deletable) delete bullet;
		return deletable; }
		),
			bullet_list.end());

		for (Bullet* bullet : bullet_list)
			bullet->on_update(delta);

		if (hp <= 0)
		{
			current_animation = is_facing_right ? &animation_die_right : &animation_die_left;
		}
		
		current_animation->on_update(delta);
		if (is_jump_effect_visible)
			animation_jump_effect.on_update(delta);
		if (is_land_effect_visible)
			animation_land_effect.on_update(delta);

		if (is_show_sketch_frame)
			sketch_image(current_animation->get_frame(), &img_sketch);

		fall_and_collied(delta);
	}
	virtual void on_draw(const Camera& camera)
	{
		if (is_jump_effect_visible)
			animation_jump_effect.on_draw(camera,(int)pos_jump_effect.x,(int)pos_jump_effect.y);
		if (is_land_effect_visible)
			animation_land_effect.on_draw(camera, (int)pos_land_effect.x, (int)pos_land_effect.y);
		for (const Particle& particle : particle_list)
			particle.on_draw(camera);

		if (hp > 0 && is_involnerable && is_show_sketch_frame)
			putimage_alpha(camera, (int)position.x, (int)position.y, &img_sketch);
		else
			current_animation->on_draw(camera, (int)(position.x), (int)(position.y));

		for (Bullet* bullet : bullet_list)
			bullet->on_draw(camera);
	}
	virtual void on_input(const ExMessage& msg)
	{
		switch (msg.message)
		{case WM_KEYDOWN:
			switch (ID)
			{
			case PlayerID::P1:
				switch (msg.vkcode)
				{
				case  0x41:
					is_left_down = true;
					break;
				case 0x44:
					is_right_down = true;
					break;
				case 0x57:
					on_jump();
					break;
				case 0x46:
					if (can_attack)
					{
						on_attack();
						can_attack = false;
						timer_attack_cd.restart();
					}
					break;
				case 0x47:
					if (mp >= 100)
					{
						on_attack_ex();
						mp = 0;
					}
					break;
				default:
					break;
				}
				break;
			case PlayerID::P2:
				switch (msg.vkcode)
				{
				case  VK_LEFT:
					is_left_down = true;
					break;
				case  VK_RIGHT:
					is_right_down = true;
					break;
				case VK_UP:
					on_jump();
					break;
				case VK_OEM_PERIOD:
					if (can_attack)
					{
						on_attack();
						can_attack = false;
						timer_attack_cd.restart();
					}
					break;
				case VK_OEM_2:
					if (mp >= 100)
					{
						on_attack_ex();
						mp = 0;
					}
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
			break;
		case WM_KEYUP:
			switch (ID)
			{
			case PlayerID::P1:
				switch (msg.vkcode)
				{
				case  0x41:
					is_left_down = false;
					break;
				case 0x44:
					is_right_down = false;
					break;
				default:
					break;
				}
				break;
			case PlayerID::P2:
				switch (msg.vkcode)
				{
				case  VK_LEFT:
					is_left_down = false;
					break;
				case  VK_RIGHT:
					is_right_down = false;
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
	void set_playerID(PlayerID playerID)
	{
		this->ID = playerID;
		is_facing_right = ID == PlayerID::P1 ? true : false;
	}
	void set_position(float x,float y)
	{
		position.x = x;
		position.y = y;
	}
	const Vector2& get_position()const
	{
		return position;
	}
	const Vector2& get_size()const
	{
		return size;
	}
	std::vector<Bullet*>& get_bullet_list()
	{
		return bullet_list;
	}
	PlayerID get_targetID()
	{
		return ID == PlayerID::P1 ? PlayerID::P2 : PlayerID::P1;
	}
	bool get_is_involnerable()
	{
		return is_involnerable; 
	}
	void set_hp(int value)
	{
		hp = value;
	}
	void set_velocity_y(float value)
	{
		velocity_y = value;
	}
	int get_hp()const
	{
		return hp;
	}
	int get_mp()const
	{
		return mp;
	}
	void be_hurt(int damage)
	{
		hp -= damage;
	}
	void make_involnerable()
	{
		is_involnerable = true;
		timer_involnerable.restart();
	}
protected:
	void on_run(float distance)
	{
		if (is_attacking_ex)
			return;

		position.x += distance;
		timer_run_effect_generation.resume();
	}
	void fall_and_collied(int delta)
	{
		float last_velocity_y = velocity_y;
		velocity_y += gravity * delta;
		position.y += velocity_y * delta;

		if (hp <= 0)
			return;

		if (velocity_y>0)
		{
			for (const Platform& platform : platform_list)
			{
				const Platform::Collisionshape& shape = platform.shape;
				bool is_collied_x = ((max(position.x + size.x, shape.right) - min(position.x, shape.left))
					< size.x + (shape.right - shape.left));
				bool  is_collied_y = (shape.y<=(position.y + size.y) && shape.y>position.y);

				if (is_collied_x&&is_collied_y)
				{
					float delta_pos_y = velocity_y * delta;
					float last_tick_foot_pos_y = position.y + size.y - delta_pos_y;
					if(last_tick_foot_pos_y<=shape.y)
					{
						position.y = shape.y - size.y;
						velocity_y = 0;
						if (last_velocity_y != 0)
							on_land();
						break;
					}
				}
			}
		}
	}
	void on_jump()
	{
		if (velocity_y != 0 || is_attacking_ex)
			return;

		velocity_y += jump_velocity;
		is_jump_effect_visible = true;
		animation_jump_effect.reset();

		IMAGE* image = animation_jump_effect.get_frame();
		pos_jump_effect.x = position.x + (size.x - image->getwidth()) / 2;
		pos_jump_effect.y = position.y + size.y - image->getheight();
	}
	void on_land()
	{
		is_land_effect_visible = true;
		animation_land_effect.reset();
		IMAGE* image = animation_land_effect.get_frame();
		pos_land_effect.x = position.x + (size.x - image->getwidth()) / 2;
		pos_land_effect.y = position.y + size.y - image->getheight();
	}
	virtual void on_attack()
	{}
	virtual void on_attack_ex()
	{}
protected:
	const float run_velocity = 0.55f;
	const float gravity = 1.6e-3f;
	const float jump_velocity = -0.85f;
protected:
	Vector2 position;
	Vector2 size;
	float velocity_y;

	Animation animation_idle_left;
	Animation animation_idle_right;
	Animation animation_run_left;
	Animation animation_run_right;
	Animation animation_die_left;
	Animation animation_die_right;

	Animation* current_animation = nullptr;

	PlayerID ID;

	bool is_left_down = false;
	bool is_right_down = false;

	bool is_facing_right;

	int hp = 100;
	int mp = 0;

	int attack_cd = 100;
	Timer timer_attack_cd;
	bool can_attack = true;

	bool is_attacking_ex = false;
	Animation animation_attack_ex_right;
	Animation animation_attack_ex_left;

	std::vector<Bullet*> bullet_list;

	bool is_involnerable = false;
	bool is_show_sketch_frame = false;
	Timer timer_involnerable;
	Timer timer_involnerable_blink;
	IMAGE img_sketch;

	std::vector<Particle> particle_list;

	Timer timer_run_effect_generation;
	Timer timer_die_effect_generation;

	bool is_jump_effect_visible = false;
	bool is_land_effect_visible = false;
	Vector2 pos_jump_effect;
	Vector2 pos_land_effect;
	Animation animation_jump_effect;
	Animation animation_land_effect;
};
#endif
