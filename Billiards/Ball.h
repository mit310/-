#pragma once
#include <vector>

#include "DirectX.h"

const int BALL_NUM = 11;
const float G = 0.098f;

class Ball_data {
	vec pos;
	vec speed;	//���x
	vec a;	//�����x

	float r;	//���a
	float g;	//�d��

	bool my;	//�����{�[�����ǂ���

	int hnd;
	int id;	//�{�[���ԍ�

public:
	Ball_data(int _id, vec _pos, bool _my);
	~Ball_data();
	void Init();	//���f���̓ǂݍ��݂Ȃ�
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

class Ball {	//�Ǘ��N���X
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