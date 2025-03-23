#include<iostream>
#include<graphics.h>

#include"sence.h"
#include"menu_sence.h"
#include"game_sence.h"
#include"select_sence.h"
#include"sence_manager.h"
#include"until.h"
#include"resource.h"
#include"player.h"
#include"bullet.h"

const int FPS = 60;
bool running = true;
sence* menu_sence = nullptr;
sence* game_sence = nullptr;
sence* select_sence = nullptr;
SenceManager sence_manager;

Camera main_camera;

Player* player_1 = nullptr;
Player* player_2 = nullptr;

IMAGE* img_avatar_p1 = nullptr;
IMAGE* img_avatar_p2 = nullptr;

bool is_debug = false;

int main()
{
	ExMessage msg;
	load_game_resources();
	initgraph(1280, 720, EX_SHOWCONSOLE);

	//设置字体样式
	settextstyle(28, 0, _T("IPix"));
	setbkmode(TRANSPARENT);

	//开启批量绘图
	BeginBatchDraw();

	menu_sence = new MenuSence();
	game_sence = new GameSence();
	select_sence = new SelectSence();
	sence_manager.set_current_sence(menu_sence);
	
	while(running)
	{
		DWORD beginTime = GetTickCount();

		while (peekmessage(&msg))
		{		   
			sence_manager.on_input(msg);
		}

		static DWORD last_tick_time = GetTickCount();
		DWORD current_tick_time = GetTickCount();
		DWORD delta_tick_time = current_tick_time - last_tick_time;
		sence_manager.on_update(delta_tick_time);
		last_tick_time = current_tick_time;

		//交换桢缓d
		cleardevice();
		sence_manager.on_draw(main_camera); 
		FlushBatchDraw();
				   
		//桢间隔固定
		DWORD endTime = GetTickCount();
		DWORD deltaTime = endTime - beginTime;
		if (deltaTime < 1000 / FPS)
			Sleep(1000 / FPS - deltaTime);
	}

	//结束批量绘图
	EndBatchDraw();
	return 0;
}
