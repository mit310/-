#pragma once
#include <vector>

#include "DirectX.h"

const int BALL_NUM = 11;
const float G = 0.098f;

class Ball_data {
	vec pos;
	vec speed;	//速度
	vec a;	//加速度

	float r;	//半径
	float g;	//重力

	bool my;	//白いボールかどうか

	int hnd;
	int id;	//ボール番号

public:
	Ball_data(int _id, vec _pos, bool _my);
	~Ball_data();
	void Init();	//モデルの読み込みなど
	void Process();
	void Draw();

	int GetId();
	vec GetPos();
	vec GetSpeed();
	float GetBallSize();

	void SetSpeed(vec _sp);

#ifdef _DEBUG
	float GetG();
#endif
};

class Ball {	//管理クラス
	std::vector<Ball_data *> data;
public:
	Ball();
	~Ball();
	void Init();
	void Process();
	void Add(int _id, vec _pos, bool _my = false);
	void Delete(int _id);
	void DeleteAll();
	void Draw();
	void AddSpeed(int _id, vec _sp);

	std::vector<Ball_data *> GetDataList();
};