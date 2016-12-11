/*
 *�@�{�[���[�h�̃��[��
 *�@0.�u���C�N�V���b�g
 *�@0-1.���̂Ƃ������{�[�����������炻�ꂪ1��ڃV���b�g�����Ƃ���
 *�@1.�{�[���Ɨ��Ƃ��|�P�b�g���w��
 *�@2.�P��ڃV���b�g
 *�@3-1.�w��𖞂����Ă���Η��Ƃ����{�[���ԍ������_�A2��ڂ𑱂�����
 *�@3-2.�S�ė�������X�g���C�N,�����E���h��
 *�@4.�Q��V���b�g���邩�A���ׂė��Ƃ����烊�Z�b�g���A0��
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
int game_mode = MODE::TITLE;	//�V�[���J��
int sys_cnt = 0;

//������
int Init(HINSTANCE h_instans){
	if(window.Init(h_instans) == -1) return 1;	//�E�B���h�E�̓o�^�E�쐬
	if(dx.Init(&window) == -1) return 1;		//Dx9�̏�����
	ShowWindow(window.GetWindowHandle(), SW_SHOWDEFAULT);
	if(key.Init(h_instans) == -1) return 1;				//DxInput8�̏�����
	if(font.Init() == -1) return 1;				//�t�H���g������
	ShowCursor(FALSE);

	table_hnd = dx.LoadModel("table.x");
	return 0;
}

//���C�����[�v
int Process(){
	if(window.Process() == -1) return -1;
	dx.ClearWindow(0x000000);
	key.Process();
	if(dx.Process() == -1) return -1;
	//�����֌W
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

	//��������`��֘A
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