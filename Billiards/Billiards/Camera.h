/*
 *�@�J��������E�Ǘ��p
 *�@DirectX�̋@�\�Ȃǂ̂��̂�DirectX.h�Q��
 */

#pragma once

class Camera {
	float h_ang;	//��������
	float v_ang;	//�㉺
	float dist;		//����
	float clip_min;
	float clip_max;

public:
	void Init();
	void Process();
	void GetAngle(float *_h, float *_v);
};

