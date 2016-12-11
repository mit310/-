#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")

#include "DirectX.h"
#include "Framerate.h"
#include "Func.h"
#include "Process.h"
#include "Sys.h"

LPDIRECT3D9 DirectX::d3d;
IDirect3DDevice9 *DirectX::device;

DirectX::DirectX(){
	this->d3d = NULL;
	this->device = NULL;
	this->cam_pos = VGet(0.f, 10.f, -15.f);
	this->cam_trg = VZero;
	this->cam_up = VGet(0.f, 1.f, 0.f);

}

int DirectX::Init(Window *win_data){
	HRESULT flg = S_OK;

	this->d3d = Direct3DCreate9(D3D_SDK_VERSION);

	if(!this->d3d){
		OutputDebugString("�~DirectX9�F�C���^�[�t�F�C�X�����Ɏ��s���܂����B\n");
		return -1;
	}
	OutputDebugString("��DirectX9�F�C���^�[�t�F�C�X�𐶐����܂����B\n");


	D3DPRESENT_PARAMETERS param;
	ZeroMemory(&param, sizeof(param));

	//�o�b�O�o�b�t�@
	param.BackBufferWidth = __SCR_X;
	param.BackBufferHeight = __SCR_Y;

	param.BackBufferFormat = D3DFMT_X8R8G8B8; //D3DFMT_UNKNOWN���ƃt���X�N���[�����ɂ��܂������Ȃ�
	param.Windowed = win_data->GetWindowMode();

	param.BackBufferCount = 1;	//�o�b�t�@�͂P��
	param.MultiSampleType = D3DMULTISAMPLE_NONE;	//�}���`�T���v�����O�͂��Ȃ�
	param.MultiSampleQuality = 0;					//�����s���Ȃ��̂�0

	param.SwapEffect = D3DSWAPEFFECT_DISCARD;		//�X���b�v�G�t�F�N�g��DirectX�ɔC����
	param.EnableAutoDepthStencil = TRUE;			//�y�o�b�t�@��DirectX��
	param.AutoDepthStencilFormat = D3DFMT_D16;		//16�r�b�g�y�o�b�t�@

	param.hDeviceWindow = win_data->GetWindowHandle();	//�E�B���h�E�n���h���̎w��
	param.Flags = 0;
	param.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//���t���b�V�����[�g�͂��̂܂܎g��
	param.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; //��ʂ̐�����A�͎����Ōv�Z(Framerate.cpp)
	flg = this->d3d->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			win_data->GetWindowHandle(),
			D3DCREATE_SOFTWARE_VERTEXPROCESSING /*| D3DCREATE_FPU_PRESERVE*/,
			&param,
			&this->device
	);

	if(FAILED(flg)){
		SAFE_RELEASE(this->d3d);
		OutputDebugString("�~DirectX9�F�f�o�C�X�̐����Ɏ��s���܂����B\n");
		OutputDebugString("���C���^�[�t�F�C�X���J�����܂����B\n");
		return -1;
	}
	OutputDebugString("��DirectX9�F�f�o�C�X�𐶐����܂����B\n");

	SetSeed();
	ShowWindow(win_data->GetWindowHandle(), SW_SHOWDEFAULT);	//���C���֐���n_show_cmd�ɏ]��

	this->device->SetRenderState(D3DRS_LIGHTING,TRUE);
	D3DLIGHT9	light;
	::ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r  = 0.3f;   light.Diffuse.g = 0.3f;   light.Diffuse.b = 0.3f;
	light.Specular.r = 0.2f;  light.Specular.g = 0.2f;  light.Specular.b = 0.2f;
	light.Ambient.r  = 0.5f;   light.Ambient.g = 0.5f;   light.Ambient.b = 0.5f;
	light.Direction = VGet(0.0f, -0.5f, -0.2f);
	this->device->SetLight(0, &light);
	this->device->LightEnable(0, TRUE);
	return 0;
}

void DirectX::Dest(){
	SAFE_RELEASE(this->device);
	SAFE_RELEASE(this->d3d);
	OutputDebugString("��DirectX9�F�f�o�C�X�E�C���^�[�t�F�C�X��j�����܂����B\n");
}



void DirectX::ClearWindow(int col){
	this->device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, col, 1.f, 0);

}

int DirectX::Process(){
	this->device->BeginScene();
	return 0;
}

void DirectX::FlipScreen(){
	this->device->EndScene();
	if(this->device->Present(NULL, NULL, NULL, NULL) == D3DERR_DEVICELOST){
		//�f�o�C�X���X�g
		OutputDebugString("!�f�o�C�X���X�g\n");
		if(this->device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET){
			//�����[�h
		}
	}
	fps.Updata();
}

void DirectX::SetCamera(vec _pos, vec _trg, vec _up){
	this->cam_pos = _pos;
	this->cam_trg = _trg;
	this->cam_up = _up;
}

void DirectX::SetRotateModel(int _hnd, vec _ang){
	try {
		this->data.at(_hnd)->ang = _ang;
	} catch(...){
		OutputDebugString(((std::string)("�~"+_str(_hnd)+"�����ȃ��f���n���h���ł��B\n")).c_str());
	}
}

void DirectX::SetPositionModel(int _hnd, vec _pos){
	try {
		this->data.at(_hnd)->pos = _pos;
	} catch(...){
		OutputDebugString(((std::string)("�~"+_str(_hnd)+"�����ȃ��f���n���h���ł��B\n")).c_str());
	}
}

void DirectX::SetScaleModel(int _hnd, vec _scale){
	try {
		this->data.at(_hnd)->scale = _scale;
	} catch(...){
		OutputDebugString(((std::string)("�~"+_str(_hnd)+"�����ȃ��f���n���h���ł��B\n")).c_str());
	}

}

void DirectX::DrawLine(vec _pos1, vec _pos2, int col){
	struct line_data {
		float x, y, z;
		DWORD col;
	};

	line_data t[2] = {
		{_pos1.x, _pos1.y, _pos1.z, col},
		{_pos2.x, _pos2.y, _pos2.z, col}
	};

	//���[���h�ϊ�
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	D3DXMatrixTranslation(&world, _pos1.x, _pos1.y, _pos1.z);
	this->device->SetTransform(D3DTS_WORLD, &world);

	//�r���[�ϊ�
	D3DXMATRIX view;
	D3DXMatrixLookAtLH(&view, &this->cam_pos, &this->cam_trg, &this->cam_up);
	this->device->SetTransform(D3DTS_VIEW, &view);

	//�ˉe�ϊ�
	D3DXMATRIX projection;
	D3DXMatrixPerspectiveFovLH(&projection, D3DX_PI/4, (float)__SCR_X/__SCR_Y, 1.f, 100.f);
	this->device->SetTransform(D3DTS_PROJECTION, &projection);

	this->device->SetVertexShader(NULL);	//�Ƃ肠�����Œ�p�C�v���C��

	this->device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	HRESULT flg;
	//�`��ݒ�
	this->device->SetRenderState(D3DRS_LIGHTING, TRUE);
	this->device->SetRenderState(D3DRS_AMBIENT, 0x00ffffff);	//����
	this->device->SetRenderState(D3DRS_SPECULARENABLE, TRUE);	//�X�y�L�����[
	this->device->SetRenderState(D3DRS_ZENABLE, TRUE);			//Z�o�b�t�@

	flg = this->device->DrawPrimitiveUP(D3DPT_LINELIST, 1, &t, sizeof(line_data));
}