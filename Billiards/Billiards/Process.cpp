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
#include "Camera.h"
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
Camera camera;

int table_hnd = -1;
int game_mode = MODE::TITLE;	//シーン遷移
int round = 1;					//ラウンド
int game_flg = 0;				//0:ブレイクショット/2:１ショット目/4:2ショット目/奇数:静止するまで待つ
int sys_cnt = 0;
int score = 0;

static int GameStep();

//初期化
int Init(HINSTANCE h_instans){
	if(window.Init(h_instans) == -1) return 1;	//ウィンドウの登録・作成
	if(dx.Init(&window) == -1) return 1;		//Dx9の初期化
	ShowWindow(window.GetWindowHandle(), SW_SHOWDEFAULT);
	if(key.Init(h_instans) == -1) return 1;				//DxInput8の初期化
	if(font.Init() == -1) return 1;				//フォント初期化
	ShowCursor(FALSE);

	table_hnd = dx.LoadModel("table.x");
	camera.Init();
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
		camera.Process();
		ball.Process();
		GameStep();
		break;
	case MODE::CLEAR:
		if(key.GetKeyPush(DIK_Z) == 1) game_mode = MODE::TITLE;
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
		dx.DrawModel(table_hnd);
		ball.Draw();
#ifdef _DEBUG
		
		font.DrawString(0, 0, "ROUND:"+_str(round), 0xffffff);
		font.DrawString(0, 20, "SCORE:"+_str(score), 0xffffff);
//		font.DrawString(0, 40, "g:"+_str(ball.GetDataList().at(0)->GetG()), 0xffffff);
		font.DrawString(0, __SCR_Y-20, _str(fps.GetFPS()), 0xffffff);
		
#endif
		break;
	case MODE::CLEAR:
		font.DrawString(200, __SCR_Y/2-10, "FINISH!"+_str(score), 0xffffff);
		font.DrawString(200, 20, "SCORE:"+_str(score), 0xffffff);
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

//操作とゲーム進行フラグ
static int GameStep(){
	switch(game_flg){
	case 0:
		if(key.GetKeyPush(DIK_Z) == 1){
			float ang, power = 0.1f;
			camera.GetAngle(&ang, NULL);
			ang *= -1;
			ang += R(180);
			vec t = VGet(power*cosf(ang), 0.f, power*sinf(ang));
			ball.AddSpeed(0, t);
			game_flg ++;
		}
		break;
	case 1:	//全ての玉がストップするまで待つ
		{
			int t_cnt = 0;
			for(unsigned int i=0;i<ball.GetDataList().size();++i){
				if(ball.GetDataList().at(i)->GetSpeed() == VZero) t_cnt ++;
			}
			if(t_cnt == ball.GetDataList().size()) game_flg ++;

			//手玉の落下判定
			if(ball.GetDataList().at(0)->GetPos().y < -1.f){
				game_flg = 1;
			}
		}
		break;
	case 2:
		if(key.GetKeyPush(DIK_Z) == 1){
			float ang, power = 0.1f;
			camera.GetAngle(&ang, NULL);
			ang *= -1;
			ang += R(180);
			vec t = VGet(power*cosf(ang), 0.f, power*sinf(ang));
			ball.AddSpeed(0, t);
			game_flg ++;
		}
		break;
	case 3:	//全ての玉がストップするまで待つ
		{
			int t_cnt = 0;
			for(unsigned int i=0;i<ball.GetDataList().size();++i){
				if(ball.GetDataList().at(i)->GetSpeed() == VZero) t_cnt ++;
			}
			if(t_cnt == ball.GetDataList().size()){
				if(round == 10) game_flg ++;
				else {
					game_flg = 0;
					round ++;
					//配置リセット
					ball.DeleteAll();
					ball.Init();	//初期化

				}
			}
		}
		break;
	case 4:	//最後のみ
		if(key.GetKeyPush(DIK_Z) == 1){
			float ang, power = 0.1f;
			camera.GetAngle(&ang, NULL);
			ang *= -1;
			ang += R(180);
			vec t = VGet(power*cosf(ang), 0.f, power*sinf(ang));
			ball.AddSpeed(0, t);
			game_flg = 5;
		}
		break;
	case 5:	//全ての玉がストップするまで待つ,玉の配置を戻す
		{
			int t_cnt = 0;
			for(unsigned int i=0;i<ball.GetDataList().size();++i){
				if(ball.GetDataList().at(i)->GetSpeed() == VZero) t_cnt ++;
			}
			if(t_cnt == ball.GetDataList().size()){
				game_flg = 0;
				//配置リセット
				ball.DeleteAll();
				ball.Init();	//初期化
				game_mode = MODE::CLEAR;
				round = 0;
			}
		}
		break;
	}
	return 0;
}