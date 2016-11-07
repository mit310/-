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

//初期化
int Init(HINSTANCE h_instans){
	if(window.Init(h_instans) == -1) return 1;	//ウィンドウの登録・作成
	if(dx.Init(&window) == -1) return 1;		//Dx9の初期化
	ShowWindow(window.GetWindowHandle(), SW_SHOWDEFAULT);
	if(key.Init(h_instans) == -1) return 1;				//DxInput8の初期化
	if(font.Init() == -1) return 1;				//フォント初期化
	ShowCursor(FALSE);

	hnd = dx.LoadModel("table.x");
	return 0;
}

//メインループ
int Process(){
	if(window.Process() == -1) return -1;
	dx.ClearWindow(0x000000);
	key.Process();
	if(dx.Process() == -1) return -1;

	//ここから描画関連
	dx.SetCamera(VGet(cosf(R(sys_cnt))*5.f, 5.f, sinf(R(sys_cnt))*5.f), VZero, VGet(0.f, 1.f, 0.f));

	dx.DrawModel(hnd);
//	font.CreateChar(0, 0, L"あ");
//	font.CreateChar(50, 100, L"う");

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