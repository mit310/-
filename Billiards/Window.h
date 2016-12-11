#pragma once
#include <Windows.h>
#include <string>

//ウィンドウクラス（今回はシングルトン）
class Window {
	HWND hnd;
	MSG msg;	//ＯＳへのメッセージ
	std::string title;
	BOOL window_mode;	//false:フルスクリーン/true:ウィンドウモード
public:
	Window();
	~Window();

	int Init(HINSTANCE h_instans);
	int Process();

	HWND GetWindowHandle();
	BOOL GetWindowMode();
};