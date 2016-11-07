#pragma once
#pragma comment(lib, "dinput8.lib")

#define INITGUID
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>


class Key {
	LPDIRECTINPUT8 di;
	LPDIRECTINPUTDEVICE8 device;

	int cnt[256];
	char buf[256];

public:
	Key();
	~Key();
	int Init(HINSTANCE h_instans);	//DIK_ZÅADIK_UPÇ»Ç«
	int Process();
	int GetKeyPush(char _code);


	void Dest();
};