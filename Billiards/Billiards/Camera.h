/*
 *　カメラ操作・管理用
 *　DirectXの機能などのものはDirectX.h参照
 */

#pragma once

class Camera {
	float h_ang;	//水平方向
	float v_ang;	//上下
	float dist;		//距離
	float clip_min;
	float clip_max;

public:
	void Init();
	void Process();
	void GetAngle(float *_h, float *_v);
};

