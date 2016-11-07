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
	//�f�o�C�X����
	if(FAILED(DirectInput8Create(h_instans, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&this->di, NULL))){
		OutputDebugString("�~DirectInput8�̐����Ɏ��s���܂����B\n");
		return -1;
	}

	if(FAILED(this->di->CreateDevice(GUID_SysKeyboard, &this->device, NULL))){
		OutputDebugString("�~DirectInput8�̐����Ɏ��s���܂����B\n���f�o�C�X�̐����Ɏ��s���܂����B\n");
		return -1;
	}

	this->device->SetDataFormat(&c_dfDIKeyboard);

	//�E�B���h�E���O�ʂ̎��̂ݎ󂯕t��&�L�[���͂�Ɛ肵�Ȃ�
	this->device->SetCooperativeLevel(window.GetWindowHandle(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	//�󂯕t���J�n
	if(this->device) this->device->Acquire();

	OutputDebugString("�ZDirectInput8�����������܂����B\n");


	return 0;
}

void Key::Dest(){
	if(this->device != NULL){
		this->device->Unacquire();
		this->device->Release();
		this->device = NULL;
	}
	SAFE_RELEASE(this->di);
	OutputDebugString("�ZDirectInput8�F�f�o�C�X��������܂����B");
}

int Key::Process(){

	if(FAILED(this->device->GetDeviceState(sizeof(this->buf), (void *)&this->buf))){
		//�f�o�C�X���X�g
		while(this->device->Acquire() == DIERR_INPUTLOST);	//�擾�ł���܂Ń��[�v
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