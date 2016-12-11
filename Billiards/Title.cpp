#include "Ball.h"
#include "Font.h"
#include "Input.h"
#include "Title.h"
#include "Process.h"

void Title_Process(){
	if(key.GetKeyPush(DIK_Z)){
		ball.Init();
		game_mode = MODE::GAME;
	}
}

void Title_Draw(){
	font.DrawString(150, 400, "Prease Z key", 0xffffff);
}