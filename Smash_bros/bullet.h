#ifndef BULLET_H
#define BULLET_H

#include"vector2.h"
#include<functional>
#include"player.h"
#include"bulletstatus.h"


class Bullet
{
public:
	Bullet() = default;
	~Bullet() = default;

	const Vector2& get_size()const
	{
		return size;
	}
	void set_position(Vector2& vec)
	{
		position = vec;
	}
	const Vector2& get_position()const
	{
		return position;
	}
	void set_velocity(float x,float y)
	{
		velocity.x = x;
		velocity.y = y;
	}
	void set_velocity(bool is_facing_right)
	{
		velocity.x = is_facing_right ? velocity.x : -velocity.x;
	}
	const Vector2& get_velocity()const
	{
		return velocity;
	}
	void set_demage(int val)
	{
		damage = val;
	}
	const float& get_damage()const
	{
		return damage;
	}
	void set_status(BulletStatus status)
	{
		this->status = status;
	}
	BulletStatus get_status()const
	{
		return status;
	}
	void  set_callback(std::function<void()>func)
	{
		callback = func;
	}
	void set_collied_target(PlayerID ID)
	{
		target_id = ID;
	}
	PlayerID  get_collied_target()const
	{
		return target_id; 
	}
	bool check_can_remove()const
	{
		return status == BulletStatus::Remove;
	}
	virtual void on_collied()
	{
		if (callback)
			callback();
	}
	virtual bool check_collied(const Vector2& position, const Vector2& size)
	{
		return (this->position.x + this->size.x / 2 >= position.x &&
			this->position.x + this->size.x / 2 <= position.x + size.x &&
			this->position.y + this->size.y / 2 >= position.y &&
			this->position.y + this->size.y / 2 <= position.y + size.y);
	}
	virtual void on_update(int delta)
	{}
	virtual void on_draw(const Camera& camera)const
	{}
protected:
	Vector2 size;
	Vector2 position;
	Vector2 velocity;
	float damage = 10;
	BulletStatus status = BulletStatus::Normal;
	std::function<void()> callback;
	PlayerID target_id;
protected:
	bool check_if_exceed_screen()
	{
		return (position.x + size.x <= 0 || position.x >= getwidth()
			|| position.y + size.y <= 0 || position.y >= getheight());
	}
};

#endif
