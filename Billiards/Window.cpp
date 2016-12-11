#include "Sys.h"
#include "Window.h"

Window::Window(){
	this->title = "�r�����[�h";
#ifdef _DEBUG
	this->window_mode = TRUE;
#else
	this->window_mode = FALSE;
#endif
}

Window::~Window(){
}

//�R�[���o�b�N
LRESULT CALLBACK WndProc(HWND wnd, UINT mes, WPARAM w_param, LPARAM l_param){
	switch(mes){
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(wnd, mes, w_param, l_param);
}


//�E�B���h�E����
int Window::Init(HINSTANCE h_instans){
	WNDCLASSEX wcex;

	ZeroMemory(&wcex, sizeof(wcex));

	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = h_instans;
	wcex.lpszClassName = this->title.c_str();
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpszMenuName = this->title.c_str();
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);	//�F�@0�Ԃ�NULL������̂ŕK��+1���邱��

	if(!RegisterClassEx(&wcex)){
		OutputDebugString("�~�G���[�F�E�B���h�E�̓o�^�Ɏ��s���܂����B\n");
		return -1;
	}

	//�E�B���h�E�쐬
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
		OutputDebugString("�~�E�B���h�E�����Ɏ��s���܂����B\n");
		return -1;
	}

	OutputDebugString("���E�B���h�E��������\n");

	return 0;
}

BOOL Window::GetWindowMode(){
	return this->window_mode;
}

HWND Window::GetWindowHandle(){
	return this->hnd;
}

//���t���[���Ăяo������
int Window::Process(){
	if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) DispatchMessage(&msg);

	if(msg.message == WM_QUIT) return -1;

	return 0;
}

