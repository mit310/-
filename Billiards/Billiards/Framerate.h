#pragma once

class Fps {
	long first;
	float now;

public:
	static const int FPS = 60;

	Fps();
	void Updata();	//�X�V
	float GetFPS();		//���݂̃t���[�����[�g���擾
};