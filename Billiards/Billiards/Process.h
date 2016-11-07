#pragma once
#include <Windows.h>

extern class Fps fps;
extern class Window window;
extern int sys_cnt;

int Init(HINSTANCE h_instans);
int Process();
void Dest();