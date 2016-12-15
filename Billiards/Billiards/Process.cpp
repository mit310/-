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
int game_mode = MODE::TITLE;	//�V�[���J��
int round = 1;					//���E���h
int game_flg = 0;				//0:�u���C�N�V���b�g/2:�P�V���b�g��/4:2�V���b�g��/�:�Î~����܂ő҂�
int sys_cnt = 0;
int score = 0;

static int GameStep();

//������
int Init(HINSTANCE h_instans){
	if(window.Init(h_instans) == -1) return 1;	//�E�B���h�E�̓o�^�E�쐬
	if(dx.Init(&window) == -1) return 1;		//Dx9�̏�����
	ShowWindow(window.GetWindowHandle(), SW_SHOWDEFAULT);
	if(key.Init(h_instans) == -1) return 1;				//DxInput8�̏�����
	if(font.Init() == -1) return 1;				//�t�H���g������
	ShowCursor(FALSE);

	table_hnd = dx.LoadModel("table.x");
	camera.Init();
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

	//��������`��֘A
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

//����ƃQ�[���i�s�t���O
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
	case 1:	//�S�Ă̋ʂ��X�g�b�v����܂ő҂�
		{
			int t_cnt = 0;
			for(unsigned int i=0;i<ball.GetDataList().size();++i){
				if(ball.GetDataList().at(i)->GetSpeed() == VZero) t_cnt ++;
			}
			if(t_cnt == ball.GetDataList().size()) game_flg ++;

			//��ʂ̗�������
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
	case 3:	//�S�Ă̋ʂ��X�g�b�v����܂ő҂�
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
					//�z�u���Z�b�g
					ball.DeleteAll();
					ball.Init();	//������

				}
			}
		}
		break;
	case 4:	//�Ō�̂�
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
	case 5:	//�S�Ă̋ʂ��X�g�b�v����܂ő҂�,�ʂ̔z�u��߂�
		{
			int t_cnt = 0;
			for(unsigned int i=0;i<ball.GetDataList().size();++i){
				if(ball.GetDataList().at(i)->GetSpeed() == VZero) t_cnt ++;
			}
			if(t_cnt == ball.GetDataList().size()){
				game_flg = 0;
				//�z�u���Z�b�g
				ball.DeleteAll();
				ball.Init();	//������
				game_mode = MODE::CLEAR;
				round = 0;
			}
		}
		break;
	}
	return 0;
}