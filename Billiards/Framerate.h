#pragma once

class Fps {
	long first;
	float now;

public:
	static const int FPS = 60;

	Fps();
	void Updata();	//更新
	float GetFPS();		//現在のフレームレートを取得
};