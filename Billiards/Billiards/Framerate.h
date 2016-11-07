#pragma once

class Fps {
	static const int FPS = 60;
	long first;
	float now;

public:
	Fps();
	void Updata();	//更新
	float GetFPS();		//現在のフレームレートを取得
};