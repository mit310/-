#include "Window.h"
#include "DirectX.h"
#include "Process.h"


#define _CRTDBG_MAP_ALLOCK
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
	#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif


int WINAPI WinMain(
	HINSTANCE h_instans,
	HINSTANCE h_prev_instance,
	LPSTR cmd_list,
	int n_show_cmd
){
		Init(h_instans);

		while(1){
			if(Process() == -1) break;
		}


		Dest();
		return 0;
}
