#pragma once
#include <Windows.h>
#include <string>

//�E�B���h�E�N���X�i����̓V���O���g���j
class Window {
	HWND hnd;
	MSG msg;	//�n�r�ւ̃��b�Z�[�W
	std::string title;
	BOOL window_mode;	//false:�t���X�N���[��/true:�E�B���h�E���[�h
public:
	Window();
	~Window();

	int Init(HINSTANCE h_instans);
	int Process();

	HWND GetWindowHandle();
	BOOL GetWindowMode();
};