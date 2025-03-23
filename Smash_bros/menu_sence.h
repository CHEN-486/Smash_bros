#ifndef MenuSence_H
#define MenuSence_H

#include"sence.h"
#include"atlas.h"
#include"camera.h"
#include"animation.h"
#include"timer.h"
#include"sence_manager.h"


extern IMAGE img_menu_background;

extern SenceManager sence_manager;


class MenuSence:public sence
{
public:
	MenuSence() = default;
	~MenuSence() = default;

	void on_enter()
	{
		mciSendString(_T("play bgm_menu repeat from 0"), NULL, 0, NULL);

	}
	void on_update(int delta)
	{
		
	}
	void on_draw(const Camera& camera)
	{
		putimage(0, 0,&img_menu_background);
		
	}
	void on_input(const ExMessage& msg)
	{
		if (msg.message == WM_KEYUP)
		{
			mciSendString("play ui_confirm from 0", NULL, 0, NULL);
			sence_manager.switch_to(SenceManager::SenceType::Select);
		}
	}
	void on_exit()
	{
		
	}

private:
	
};

#endif
