#include "DirectX.h"
#include "Font.h"
#include "Framerate.h"
#include "Func.h"
#include "Input.h"
#include "Process.h"
#include "Window.h"

Window window;
DirectX dx;
Key key;
Font font;
Fps fps;

int hnd = -1;
static int sys_cnt = 0;

//������
int Init(HINSTANCE h_instans){
	if(window.Init(h_instans) == -1) return 1;	//�E�B���h�E�̓o�^�E�쐬
	if(dx.Init(&window) == -1) return 1;		//Dx9�̏�����
	ShowWindow(window.GetWindowHandle(), SW_SHOWDEFAULT);
	if(key.Init(h_instans) == -1) return 1;				//DxInput8�̏�����
	if(font.Init() == -1) return 1;				//�t�H���g������
	ShowCursor(FALSE);

	hnd = dx.LoadModel("table.x");
	return 0;
}

//���C�����[�v
int Process(){
	if(window.Process() == -1) return -1;
	dx.ClearWindow(0x000000);
	key.Process();
	if(dx.Process() == -1) return -1;

	//��������`��֘A
	dx.SetCamera(VGet(cosf(R(sys_cnt))*5.f, 5.f, sinf(R(sys_cnt))*5.f), VZero, VGet(0.f, 1.f, 0.f));

	dx.DrawModel(hnd);
//	font.CreateChar(0, 0, L"��");
//	font.CreateChar(50, 100, L"��");

	font.DrawString(150, 400, _str(key.GetKeyPush(DIK_Z)), 0x0000ff);
	font.DrawString(300, 200, _str(fps.GetFPS()), 0x0000ff);
	font.DrawString((int)(100+sinf(R(sys_cnt))*100), (int)(200+sinf(R(sys_cnt))*100), _str(sys_cnt), 0xffffff);

	dx.FlipScreen();
	sys_cnt ++;
	return 0;
}

void Dest(){
	dx.Dest();
	key.Dest();
}