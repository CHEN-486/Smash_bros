#ifndef SenceManager_H
#define SenceManager_H

#include"sence.h"
#include"camera.h"

extern sence* menu_sence;
extern sence* game_sence;
extern sence* select_sence;

class SenceManager
{
public:
	enum class SenceType
	{
		Menu,
		Game,
		Select
	};
	void set_current_sence(sence* sence)
	{
		current_sence = sence;
		current_sence->on_enter();
	}
	void switch_to(SenceType state)
	{
		current_sence->on_exit();
		switch (state)
		{
		case SenceType::Menu:
				current_sence = menu_sence;
				break;
		case SenceType::Game:
			current_sence = game_sence;
			break;
		case SenceType::Select:
			current_sence = select_sence;
			break;
		default:
			break;
		}
		current_sence->on_enter();
	}
	void on_update(int delta)
	{
		current_sence->on_update(delta);
	}
	void on_input(ExMessage msg)
	{
		current_sence->on_input(msg);
	}
	void on_draw(const Camera& camera)
	{
		current_sence->on_draw(camera);
	}
public:
	SenceManager() = default;
	~SenceManager() = default;
private:
	sence* current_sence = nullptr;
};

#endif
