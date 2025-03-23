#ifndef GameSence_H
#define GameSence_H

#include"sence.h"
#include"sence_manager.h"
#include"until.h"
#include"platform.h"
#include<vector>
#include"player.h"
#include"statusbar.h"

std::vector<Platform> platform_list;

extern IMAGE img_sky;
extern IMAGE img_hills;
extern IMAGE img_platform_large;
extern IMAGE img_platform_small;

extern SenceManager sence_manager;
extern bool is_debug;

extern Player* player_1;
extern Player* player_2;

extern IMAGE* img_avatar_p1;
extern IMAGE* img_avatar_p2;

extern IMAGE img_1P_winner;
extern IMAGE img_2P_winner;
extern IMAGE img_winner_bar;

extern IMAGE img_1P_cursor;
extern IMAGE img_2P_cursor;

extern Camera main_camera;

class GameSence :public sence
{
public:
	GameSence() = default;
	~GameSence() = default;

	void on_enter()
	{
		is_game_over = false;
		is_slide_out_started = false;

		pos_img_sky.x = (getwidth() - img_sky.getwidth()) / 2;
		pos_img_sky.y = (getheight() - img_sky.getheight()) / 2;

		pos_img_hills.x = (getwidth() - img_hills.getwidth()) / 2;
		pos_img_hills.y = (getheight() - img_hills.getheight()) / 2;

		platform_list.resize(4);

		Platform& large_platform = platform_list[0];
		large_platform.img = &img_platform_large;
		large_platform.pos.x = (int)(getwidth() - img_platform_large.getwidth()) / 2;
		large_platform.pos.y = 455;
		large_platform.shape.y = large_platform.pos.y + 60;
		large_platform.shape.left = large_platform.pos.x + 30;
		large_platform.shape.right = large_platform.pos.x + img_platform_large.getwidth() - 30;

		Platform& small_platform_1 = platform_list[1];
		small_platform_1.img = &img_platform_small;
		small_platform_1.pos.x = 175;
		small_platform_1.pos.y = 360;
		small_platform_1.shape.y = (float)small_platform_1.pos.y + img_platform_small.getheight() / 2;
		small_platform_1.shape.left = (float)small_platform_1.pos.x + 40;
		small_platform_1.shape.right = (float)small_platform_1.pos.x + img_platform_small.getwidth() - 40;

		Platform& small_platform_2 = platform_list[2];
		small_platform_2.img = &img_platform_small;
		small_platform_2.pos.x = 855;
		small_platform_2.pos.y = 360;
		small_platform_2.shape.y = (float)small_platform_2.pos.y + img_platform_small.getheight() / 2;
		small_platform_2.shape.left = (float)small_platform_2.pos.x + 40;
		small_platform_2.shape.right = (float)small_platform_2.pos.x + img_platform_small.getwidth() - 40;

		Platform& small_platform_3 = platform_list[3];
		small_platform_3.img = &img_platform_small;
		small_platform_3.pos.x = 515;
		small_platform_3.pos.y = 225;
		small_platform_3.shape.y = (float)small_platform_3.pos.y + img_platform_small.getheight() / 2;
		small_platform_3.shape.left = (float)small_platform_3.pos.x + 40;
		small_platform_3.shape.right = (float)small_platform_3.pos.x + img_platform_small.getwidth() - 40;

		player_1->set_position(200, 50);
		player_2->set_position(975, 50);

		status_bar_p1.set_avatar(img_avatar_p1);
		status_bar_p2.set_avatar(img_avatar_p2);
		status_bar_p1.set_position(235, 625);
		status_bar_p2.set_position(675, 625);

		pos_img_winner_bar.x = -img_winner_bar.getwidth();
		pos_img_winner_bar.y = (getheight() - img_winner_bar.getheight()) / 2;
		pos_x_img_winner_bar_dst = (getwidth() - img_winner_bar.getwidth()) / 2;
		pos_img_winner_text.x = -img_winner_bar.getwidth();
		pos_img_winner_text.y = (getheight() - img_1P_winner.getheight()) / 2;
		pos_x_img_winner_text_dst = (getwidth() - img_1P_winner.getwidth()) / 2;

		timer_winner_slide_in.restart();
		timer_winner_slide_in.set_wait_time(2500);
		timer_winner_slide_in.set_one_shot(true);
		timer_winner_slide_in.set_callback([&]() {is_slide_out_started = true; });

		timer_winner_slide_out.restart();
		timer_winner_slide_out.set_wait_time(1000);
		timer_winner_slide_out.set_one_shot(true);
		timer_winner_slide_out.set_callback([&]() 
			{
				sence_manager.switch_to(SenceManager::SenceType::Menu); 
			});

		timer_cursor_visibility.set_wait_time(2500);
		timer_cursor_visibility.set_one_shot(true);
		timer_cursor_visibility.set_callback([&]() {is_cursor_visible = false; });

		mciSendString(_T("play bgm_game repeat from 0"), NULL, 0, NULL);
	}
	void on_update(int delta)
	{

		player_1->on_update(delta);
		player_2->on_update(delta);
		bullet_collied(player_1);
		bullet_collied(player_2);

		status_bar_p1.set_hp(player_1->get_hp());
		status_bar_p1.set_mp(player_1->get_mp());
		status_bar_p2.set_hp(player_2->get_hp());
		status_bar_p2.set_mp(player_2->get_mp());

		if (is_cursor_visible)
			timer_cursor_visibility.on_update(delta);

		check_game_over();
		if(is_game_over)
		{
			pos_img_winner_bar.x += (int)(speed_winner_bar * delta);
			pos_img_winner_text.x += (int)(speed_winner_text * delta);
			if (!is_slide_out_started)
			{
				timer_winner_slide_in.on_update(delta);
				if (pos_img_winner_bar.x > pos_x_img_winner_bar_dst)
					pos_img_winner_bar.x = pos_x_img_winner_bar_dst;
				if (pos_img_winner_text.x > pos_x_img_winner_text_dst)
					pos_img_winner_text.x = pos_x_img_winner_text_dst;
			}
			else
				timer_winner_slide_out.on_update(delta);
		}

		main_camera.on_update(delta);
	}
	void on_draw(const Camera& camera)
	{
		putimage_alpha(camera, pos_img_sky.x, pos_img_sky.y, &img_sky);
		putimage_alpha(camera, pos_img_sky.x, pos_img_sky.y, &img_hills);

		for (const Platform& platform : platform_list)
			platform.on_draw(camera);

		if(is_debug)
		{
			settextcolor(RGB(255, 0, 0));
			outtextxy(15, 15, _T("已开启调试模式，按下Q键关闭"));
		}

		player_1->on_draw(camera);
		player_2->on_draw(camera);
		if(is_cursor_visible)
		{
			 Vector2 pos_player_1;
			 Vector2 pos_player_2;
			 pos_player_1.x = player_1->get_position().x + 
				 (player_1->get_size().x - img_1P_cursor.getwidth()) / 2;
			 pos_player_1.y = player_1->get_position().y - img_1P_cursor.getheight();
			 pos_player_2.x = player_2->get_position().x +
				 (player_2->get_size().x - img_2P_cursor.getwidth()) / 2;
			 pos_player_2.y = player_2->get_position().y - img_2P_cursor.getheight();
			putimage_alpha(camera, pos_player_1.x, pos_player_1.y, &img_1P_cursor);
			putimage_alpha(camera, pos_player_2.x, pos_player_2.y, &img_2P_cursor);
		}
		if(is_game_over)
		{
			putimage_alpha(camera, pos_img_winner_bar.x, pos_img_winner_bar.y, &img_winner_bar);
			putimage_alpha(camera, pos_img_winner_text.x, pos_img_winner_text.y,
				player_1->get_hp() > 0 ? &img_1P_winner : &img_2P_winner);
		}
		else
		{
			status_bar_p1.on_draw();
			status_bar_p2.on_draw();
		}
	}
	void on_input(const ExMessage& msg)
	{
		player_1->on_input(msg);
		player_2->on_input(msg);

		switch (msg.message)
		{
		case WM_KEYUP:
			switch (msg.vkcode)
			{
			case 0x51:
				is_debug = !is_debug;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
	void on_exit()
	{
		delete player_1;
		delete player_2;
		player_1 = nullptr;
		player_2 = nullptr;
		is_debug = false;
		main_camera.reset();
	}
private:
	void check_game_over()
	{
		const Vector2& pos_player_1 = player_1->get_position();
		const Vector2& pos_player_2 = player_2->get_position();
		if (pos_player_1.y >= getheight())
			player_1->set_hp(0);
		if (pos_player_2.y >= getheight())
			player_2->set_hp(0);

		if(player_1->get_hp()<=0||player_2->get_hp()<=0)
		{
			if(!is_game_over)
			{
				mciSendString(_T("stop bgm_game"), NULL, 0, NULL);
				mciSendString(_T("play ui_win from 0"), NULL, 0, NULL);
			}
			is_game_over = true;
		}
	}
private:
	void bullet_collied(Player* player)
	{
		
		Player* target_player = nullptr;
		switch (player->get_targetID())
		{
		case PlayerID::P1:
			target_player = player_1;
			break;
		case PlayerID::P2:
			target_player = player_2;
			break;
		default:
			break;
		}

		if (target_player->get_hp() == 0)
			return;

		std::vector<Bullet*> bullet_list = player->get_bullet_list();
		if (!target_player->get_is_involnerable())
		{
			for (Bullet* bullet : bullet_list)
			{
				if (bullet->get_status() != BulletStatus::Normal)
					continue;
				if (bullet->check_collied(target_player->get_position(), target_player->get_size()))
				{
					bullet->on_collied();
					bullet->set_status(BulletStatus::Novalue);//回调函数内使用类方法
					target_player->be_hurt(bullet->get_damage());
					target_player->make_involnerable();
					if (target_player->get_hp() == 0)
						target_player->set_velocity_y(-1.0f);
				}
			}
		}
	}
	private:
		const float speed_winner_bar = 3.0f;
		const float speed_winner_text = 1.5f;
private:
	POINT pos_img_sky = { 0 };
	POINT pos_img_hills = { 0 };

	StatusBar status_bar_p1;
	StatusBar status_bar_p2;

	bool is_game_over = false;

	POINT pos_img_winner_bar = { 0 };
	POINT pos_img_winner_text = { 0 };
	int pos_x_img_winner_bar_dst = 0;
	int pos_x_img_winner_text_dst = 0;
	Timer timer_winner_slide_in;
	Timer timer_winner_slide_out;
	bool is_slide_out_started = false;

	bool is_cursor_visible = true;
	Timer timer_cursor_visibility;
};

#endif
