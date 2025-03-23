#ifndef UNTIL_H
#define UNTIL_H

#include<graphics.h>
#include"atlas.h"
#include"camera.h"
#include"resource.h"

#pragma comment(lib,"Winmm.lib")
#pragma comment(lib,"MSIMG32.LIB")

//äÖÈ¾Í¸Ã÷Í¼Æ¬
void putimage_alpha(const Camera& camera,int x,int y,IMAGE* img)
{
	int w = img->getwidth();
	int h = img->getheight();
	Vector2 pos = camera.get_position();
	AlphaBlend(GetImageHDC(GetWorkingImage()), (int)(x-pos.x), (int)(y-pos.y), w, h, 
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}
//äÖÈ¾²Ã¼ôÍ¼Æ¬¡¾ÖØÔØ¡¿
void putimage_alpha(const Camera& camera, int dst_x, int dst_y, int w, int h, IMAGE* img, int src_x, int src_y)
{
	int width = w > 0 ? w : img->getwidth();
	int height = h > 0 ? h : img->getheight();
	Vector2 pos = camera.get_position();
	AlphaBlend(GetImageHDC(GetWorkingImage()), (int)(dst_x - pos.x), (int)(dst_y - pos.y), width, height,
		GetImageHDC(img), (int)(src_x - pos.x), (int)(src_y - pos.y), width, height, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}


//·­×ªÍ¼Æ¬
void flip_image(IMAGE* src,IMAGE* dst )
{
	int width = src->getwidth();
	int height = src->getheight();
	Resize(dst, width, height);
	DWORD* src_buffer = GetImageBuffer(src);
	DWORD* dst_buffer = GetImageBuffer(dst);
	for(int y=0;y<height;++y)
	{
		for (int x = 0;x<width;++x)
		{
			int src_buffer_index = y * width + x;
			int dst_buffer_index = y * width + (width - x - 1);
			dst_buffer[dst_buffer_index] = src_buffer[src_buffer_index];
		}
	}
}
//·­×ªatlasÍ¼¼¯
void flip_atlas(Atlas* src,Atlas* dst)
{
	dst->clear();
	for(int i=0;i<src->get_size();++i)
	{
		IMAGE img_template;
		flip_image(src->get_image(i), &img_template);
		dst->add_image(img_template);
	}
}

//äÖÈ¾ÏßÌõ¡¾ÖØÔØ¡¿
void line(const Camera& camera,int x1,int y1,int x2,int y2)
{
	Vector2 pos = camera.get_position();
	line((int)(x1 - pos.x), (int)(y1 - pos.y), (int)(x2 - pos.x), (int)(y2 - pos.y));
}
//°×É«¼ôÓ°
void sketch_image(IMAGE* src,IMAGE* dst)
{
	int wid = src->getwidth();
	int hei = src->getheight();
	Resize(dst, wid, hei);
	DWORD* src_buffer = GetImageBuffer(src);
	DWORD* dst_buffer = GetImageBuffer(dst);
	for(int y=0;y<hei;++y)
	{
		for (int x = 0;x<wid;++x)
		{
			int idx = y * wid + x;
			dst_buffer[idx] = BGR(RGB(255, 255, 255)) | (src_buffer[idx] & 0xFF000000);
		}
	}
}

//¼ÓÔØ×ÊÔ´
void load_game_resources()
{
	AddFontResourceEx(_T("resources/IPix.ttf"), FR_PRIVATE, NULL);
	
	loadimage(&img_menu_background, _T("resources/menu_background.png"));
	loadimage(&img_VS, _T("resources/VS.png"));
	loadimage(&img_1P, _T("resources/1P.png"));
	loadimage(&img_2P, _T("resources/2P.png"));
	loadimage(&img_1P_desc, _T("resources/1P_desc.png"));
	loadimage(&img_2P_desc, _T("resources/2P_desc.png"));
	loadimage(&img_gravestone_right, _T("resources/gravestone.png"));
	flip_image(&img_gravestone_right, &img_gravestone_left);
	loadimage(&img_selector_tip, _T("resources/selector_tip.png"));
	loadimage(&img_selector_background, _T("resources/selector_background.png"));
	loadimage(&img_1P_selector_btn_idle_right, _T("resources/1P_selector_btn_idle.png"));
	flip_image(&img_1P_selector_btn_idle_right, &img_1P_selector_btn_idle_left);
	loadimage(&img_1P_selector_btn_down_right, _T("resources/1P_selector_btn_down.png"));
	flip_image(&img_1P_selector_btn_down_right, &img_1P_selector_btn_down_left);
	loadimage(&img_2P_selector_btn_idle_right, _T("resources/2P_selector_btn_idle.png"));
	flip_image(&img_2P_selector_btn_idle_right, &img_2P_selector_btn_idle_left);
	loadimage(&img_2P_selector_btn_down_right, _T("resources/2P_selector_btn_down.png")); 
	flip_image(&img_2P_selector_btn_down_right, &img_2P_selector_btn_down_left);
	loadimage(&img_peashooter_selector_background_right, _T("resources/peashooter_selector_background.png"));
	flip_image(&img_peashooter_selector_background_right, &img_peashooter_selector_background_left);
	loadimage(&img_sunflower_selector_background_right, _T("resources/sunflower_selector_background.png")); 
	flip_image(&img_sunflower_selector_background_right, &img_sunflower_selector_background_left);

	loadimage(&img_sky, _T("resources/sky.png"));
	loadimage(&img_hills, _T("resources/hills.png"));
	loadimage(&img_platform_large, _T("resources/platform_large.png"));
	loadimage(&img_platform_small, _T("resources/platform_small.png"));

	loadimage(&img_1P_cursor, _T("resources/1P_cursor.png"));
	loadimage(&img_2P_cursor, _T("resources/2P_cursor.png"));

	atlas_peashooter_idle_right.load_from_file(_T("resources/peashooter_idle_%d.png"), 9);
	flip_atlas(&atlas_peashooter_idle_right, &atlas_peashooter_idle_left);
	atlas_peashooter_run_right.load_from_file(_T("resources/peashooter_run_%d.png"), 5);
	flip_atlas(&atlas_peashooter_run_right, &atlas_peashooter_run_left); 
	atlas_peashooter_attack_ex_right.load_from_file(_T("resources/peashooter_attack_ex_%d.png"), 3);
	flip_atlas(&atlas_peashooter_attack_ex_right, &atlas_peashooter_attack_ex_left); 
	atlas_peashooter_die_right.load_from_file(_T("resources/peashooter_die_%d.png"), 4); 
	flip_atlas(&atlas_peashooter_die_right, &atlas_peashooter_die_left);

	atlas_sunflower_idle_right.load_from_file(_T("resources/sunflower_idle_%d.png"), 8);
	flip_atlas(&atlas_sunflower_idle_right, &atlas_sunflower_idle_left); 
	atlas_sunflower_run_right.load_from_file(_T("resources/sunflower_run_%d.png"), 5);
	flip_atlas(&atlas_sunflower_run_right, &atlas_sunflower_run_left);
	atlas_sunflower_attack_ex_right.load_from_file(_T("resources/sunflower_attack_ex_%d.png"), 9); 
	flip_atlas(&atlas_sunflower_attack_ex_right, &atlas_sunflower_attack_ex_left); 
	atlas_sunflower_die_right.load_from_file(_T("resources/sunflower_die_%d.png"), 2); 
	flip_atlas(&atlas_sunflower_die_right, &atlas_sunflower_die_left);

	loadimage(&img_pea, _T("resources/pea.png"));
	atlas_pea_break.load_from_file(_T("resources/pea_break_%d.png"), 3); 
	atlas_sun.load_from_file(_T("resources/sun_%d.png"), 5); 
	atlas_sun_explode.load_from_file(_T("resources/sun_explode_%d.png"), 5); 
	atlas_sun_ex.load_from_file(_T("resources/sun_ex_%d.png"), 5); 
	atlas_sun_ex_explode.load_from_file(_T("resources/sun_ex_explode_%d.png"), 5); 
	atlas_sun_text.load_from_file(_T("resources/sun_text_%d.png"), 6); 

	atlas_run_effect.load_from_file(_T("resources/run_effect_%d.png"), 4); 
	atlas_jump_effect.load_from_file(_T("resources/jump_effect_%d.png"), 5); 
	atlas_land_effect.load_from_file(_T("resources/land_effect_%d.png"), 2); 

	loadimage(&img_1P_winner, _T("resources/1P_winner.png"));
	loadimage(&img_2P_winner, _T("resources/2P_winner.png")); 
	loadimage(&img_winner_bar, _T("resources/winner_bar.png"));
 
	loadimage(&img_avatar_peashooter, _T("resources/avatar_peashooter.png"));  
	loadimage(&img_avatar_sunflower, _T("resources/avatar_sunflower.png"));

	mciSendString(_T("open resources/bgm_game.mp3 alias bgm_game"), NULL, 0, NULL);
	mciSendString(_T("open resources/bgm_menu.mp3 alias bgm_menu"), NULL, 0, NULL);
	mciSendString(_T("open resources/pea_break_1.mp3 alias pea_break_1"), NULL, 0, NULL);
	mciSendString(_T("open resources/pea_break_2.mp3 alias pea_break_2"), NULL, 0, NULL);
	mciSendString(_T("open resources/pea_break_3.mp3 alias pea_break_3"), NULL, 0, NULL);
	mciSendString(_T("open resources/pea_shoot_1.mp3 alias pea_shoot_1"), NULL, 0, NULL);
	mciSendString(_T("open resources/pea_shoot_2.mp3 alias pea_shoot_2"), NULL, 0, NULL);
	mciSendString(_T("open resources/pea_shoot_ex.mp3 alias pea_shoot_ex"), NULL, 0, NULL);
	mciSendString(_T("open resources/sun_explode.mp3 alias sun_explode"), NULL, 0, NULL);
	mciSendString(_T("open resources/sun_explode_ex.mp3 alias sun_explode_ex"), NULL, 0, NULL);
	mciSendString(_T("open resources/sun_text.mp3 alias sun_text"), NULL, 0, NULL);
	mciSendString(_T("open resources/ui_confirm.wav alias ui_confirm"), NULL, 0, NULL);
	mciSendString(_T("open resources/ui_switch.wav alias ui_switch"), NULL, 0, NULL);
	mciSendString(_T("open resources/ui_win.wav alias ui_win"), NULL, 0, NULL);

}

#endif
