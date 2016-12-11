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
	static LPDIRECT3D9 d3d;	//�C���^�[�t�F�C�X
	static LPDIRECT3DDEVICE9 device;	//�f�o�C�X
	vec cam_pos;
	vec cam_trg;
	vec cam_up;


public:
	DirectX();


	int Init(Window *win_data);
	void Dest();
	int Process();

	//����������Q�[���p
	int LoadModel(std::string _file);
	void DrawModel(int _hnd);
	void SetCamera(vec _pos, vec _trg, vec _up);
	void SetRotateModel(int _hnd, vec _ang);	//����]
	void SetPositionModel(int _hnd, vec _pos);	//���W
	void SetScaleModel(int _hnd, vec _scale);
	Model_HitResult_Dim GetHitColl_Model_Lay(int _hnd, vec _pos1, vec _pos2);	//���f���Ɛ��̏Փ˔���
	void DeleteHitColl(Model_HitResult_Dim _hit);	//�Փ˔��茋�ʂ�j��

	void ClearWindow(int col = 0x000000);
	void FlipScreen();	//�`��I�������f(EndScene�����s�����̂ł���ȍ~�͕`�悵�Ȃ�)

	//�f�o�b�O�Ȃ񂩂�
	void DrawLine(vec _pos1, vec _pos2, int _col);
};