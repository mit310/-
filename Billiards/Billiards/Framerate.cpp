#pragma comment (lib, "winmm.lib")

#include "Framerate.h"
#include "Window.h"


Fps::Fps(){
	this->first = 0;
	this->now = 0;

}

void Fps::Updata(){
	static int cnt = 0;
	if(!cnt) this->first = timeGetTime();

	if(cnt == this->FPS){	//60‰ñ(1000ms•ª‚Ìˆ—‚ªI—¹‚µ‚½‚Æ‚«)
		this->now = 1000.f/((timeGetTime()-this->first)/(float)this->FPS);	//·•ª‚ð‚Æ‚é
	}

	int t_wait = cnt*1000/this->FPS - (timeGetTime()-this->first);
	if(0 < t_wait) Sleep(t_wait);
	cnt ++;
}

float Fps::GetFPS(){
	return this->now;
}