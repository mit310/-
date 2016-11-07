#include <d3dx9.h>

#include "Func.h"
#include "Input.h"
#include "Process.h"
#include "Window.h"

Key::Key(){
	this->di = NULL;
	this->device = NULL;
	ZeroMemory(this->buf, sizeof(this->buf));
	ZeroMemory(this->cnt, sizeof(this->cnt));

}

Key::~Key(){
}

int Key::Init(HINSTANCE h_instans){
	//デバイス生成
	if(FAILED(DirectInput8Create(h_instans, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&this->di, NULL))){
		OutputDebugString("×DirectInput8の生成に失敗しました。\n");
		return -1;
	}

	if(FAILED(this->di->CreateDevice(GUID_SysKeyboard, &this->device, NULL))){
		OutputDebugString("×DirectInput8の生成に失敗しました。\n└デバイスの生成に失敗しました。\n");
		return -1;
	}

	this->device->SetDataFormat(&c_dfDIKeyboard);

	//ウィンドウが前面の時のみ受け付け&キー入力を独占しない
	this->device->SetCooperativeLevel(window.GetWindowHandle(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	//受け付け開始
	if(this->device) this->device->Acquire();

	OutputDebugString("〇DirectInput8を初期化しました。\n");


	return 0;
}

void Key::Dest(){
	if(this->device != NULL){
		this->device->Unacquire();
		this->device->Release();
		this->device = NULL;
	}
	SAFE_RELEASE(this->di);
	OutputDebugString("〇DirectInput8：デバイスを解放しました。");
}

int Key::Process(){

	if(FAILED(this->device->GetDeviceState(sizeof(this->buf), (void *)&this->buf))){
		//デバイスロスト
		while(this->device->Acquire() == DIERR_INPUTLOST);	//取得できるまでループ
	}
	for(int i=0;i<256;++i){
		if(this->buf[i] & 0x80) this->cnt[i] ++;
		else this->cnt[i] = 0;
	}
	return 0;
}

int Key::GetKeyPush(char _code){
	return this->cnt[_code];
}