#pragma once
#include <Windows.h>

extern class Font font;
extern class Fps fps;
extern class Key key;
extern class Window window;
extern class DirectX dx;
extern int sys_cnt;
extern int game_mode;
extern int score;

extern class Ball ball;
extern class Camera camera;
extern int table_hnd;	//ステージのモデルハンドル

struct MODE {
	enum {
		TITLE,
		GAME,
		CLEAR,
		OVER
	};
};

int Init(HINSTANCE h_instans);
int Process();
void Dest();