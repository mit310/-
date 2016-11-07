#include "Sys.h"
#include "Window.h"

Window::Window(){
	this->title = "ビリヤード";
#ifdef _DEBUG
	this->window_mode = TRUE;
#else
	this->window_mode = FALSE;
#endif
}

Window::~Window(){
}

//コールバック
LRESULT CALLBACK WndProc(HWND wnd, UINT mes, WPARAM w_param, LPARAM l_param){
	switch(mes){
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(wnd, mes, w_param, l_param);
}


//ウィンドウ生成
int Window::Init(HINSTANCE h_instans){
	WNDCLASSEX wcex;

	ZeroMemory(&wcex, sizeof(wcex));

	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = h_instans;
	wcex.lpszClassName = this->title.c_str();
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpszMenuName = this->title.c_str();
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);	//色　0番にNULLがあるので必ず+1すること

	if(!RegisterClassEx(&wcex)){
		OutputDebugString("×エラー：ウィンドウの登録に失敗しました。\n");
		return -1;
	}

	//ウィンドウ作成
	this->hnd = CreateWindow(
		this->title.c_str(),
		this->title.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		__SCR_X,
		__SCR_Y,
		NULL,
		NULL,
		h_instans,
		NULL
		);

	if(!this->hnd){
		OutputDebugString("×ウィンドウ生成に失敗しました。\n");
		return -1;
	}

	OutputDebugString("○ウィンドウ生成完了\n");

	return 0;
}

BOOL Window::GetWindowMode(){
	return this->window_mode;
}

HWND Window::GetWindowHandle(){
	return this->hnd;
}

//毎フレーム呼び出すこと
int Window::Process(){
	if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) DispatchMessage(&msg);

	if(msg.message == WM_QUIT) return -1;

	return 0;
}

