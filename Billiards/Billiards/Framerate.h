#pragma once

class Fps {
	static const int FPS = 60;
	long first;
	float now;

public:
	Fps();
	void Updata();	//�X�V
	float GetFPS();		//���݂̃t���[�����[�g���擾
};