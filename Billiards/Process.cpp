/*
 *　ボーラードのルール
 *　0.ブレイクショット
 *　0-1.このとき何かボールが入ったらそれが1回目ショット扱いとする
 *　1.ボールと落とすポケットを指定
 *　2.１回目ショット
 *　3-1.指定を満たしていれば落としたボール番号が得点、2回目を続きから
 *　3-2.全て落ちたらストライク,次ラウンドへ
 *　4.２回ショットするか、すべて落としたらリセットし、0へ
 *
 */

#include "Ball.h"
#include "DirectX.h"
#include "Font.h"
#include "Framerate.h"
#include "Func.h"
#include "Input.h"
#include "Process.h"
#include "Title.h"
#include "Window.h"

Window window;
DirectX dx;
Key key;
Font font;
Fps fps;

Ball ball;

int table_hnd = -1;
int game_mode = MODE::TITLE;	//シーン遷移
int sys_cnt = 0;

//初期化
int Init(HINSTANCE h_instans){
	if(window.Init(h_instans) == -1) return 1;	//ウィンドウの登録・作成
	if(dx.Init(&window) == -1) return 1;		//Dx9の初期化
	ShowWindow(window.GetWindowHandle(), SW_SHOWDEFAULT);
	if(key.Init(h_instans) == -1) return 1;				//DxInput8の初期化
	if(font.Init() == -1) return 1;				//フォント初期化
	ShowCursor(FALSE);

	table_hnd = dx.LoadModel("table.x");
	return 0;
}

//メインループ
int Process(){
	if(window.Process() == -1) return -1;
	dx.ClearWindow(0x000000);
	key.Process();
	if(dx.Process() == -1) return -1;
	//処理関係
	switch(game_mode){
	case MODE::TITLE:
		Title_Process();
		break;
	case MODE::GAME:
		dx.SetPositionModel(table_hnd, VZero);
		ball.Process();
		break;
	case MODE::CLEAR:
		break;
	case MODE::OVER:
		break;
	}

	//ここから描画関連
	switch(game_mode){
	case MODE::TITLE:
		Title_Draw();
		break;
	case MODE::GAME:
		dx.SetCamera(VGet(sinf(R(sys_cnt))+1.1f, 3.f, 0.f), VZero, VGet(0.f, 1.f, 0.f));
		dx.DrawModel(table_hnd);
		ball.Draw();
#ifdef _DEBUG
		
		font.DrawString(0, 0, "y:"+_str(ball.GetDataList().at(0)->GetPos().y), 0xffffff);
		font.DrawString(0, 20, "sp_y:"+_str(ball.GetDataList().at(0)->GetSpeed().y), 0xffffff);
		font.DrawString(0, 40, "g:"+_str(ball.GetDataList().at(0)->GetG()), 0xffffff);
		font.DrawString(0, 60, "t:"+_str(-sinf(R(sys_cnt))), 0xffffff);
		
#endif
		break;
	case MODE::CLEAR:
		break;
	case MODE::OVER:
		break;
	}

	dx.FlipScreen();
	sys_cnt ++;
	return 0;
}

void Dest(){
	dx.Dest();
	key.Dest();
}