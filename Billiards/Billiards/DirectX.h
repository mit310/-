#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>
#include <Windows.h>

#include "Collision.h"
#include "DX_Model.h"
#include "Window.h"

class DirectX : public DX_Model {
protected:
	static LPDIRECT3D9 d3d;	//インターフェイス
	static LPDIRECT3DDEVICE9 device;	//デバイス
	vec cam_pos;
	vec cam_trg;
	vec cam_up;
	float cam_clip_min;
	float cam_clip_max;

public:
	DirectX();


	int Init(Window *win_data);
	void Dest();
	int Process();

	//↓ここからゲーム用
	int LoadModel(std::string _file);
	void DrawModel(int _hnd);
	void SetCamera(vec _pos, vec _trg, vec _up);
	void SetRotateModel(int _hnd, vec _ang);	//軸回転
	void SetPositionModel(int _hnd, vec _pos);	//座標
	void SetScaleModel(int _hnd, vec _scale);

	vec GetCameraPos();
	vec GetCameraTrg();

	Model_HitResult_Dim GetHitColl_Model_Lay(int _hnd, vec _pos1, vec _pos2);	//モデルと線の衝突判定
	void DeleteHitColl(Model_HitResult_Dim *_hit);	//衝突判定結果を破棄

	void ClearWindow(int col = 0x000000);
	void FlipScreen();	//描画終了＆反映(EndSceneも実行されるのでこれ以降は描画しない)

	//デバッグなんかに
	void DrawLine(vec _pos1, vec _pos2, int _col);
};