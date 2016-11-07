#include <d3d9.h>
#include "DX_Model.h"
#include "DirectX.h"

DX_Model::DX_Model(){
	this->data.reserve(this->MAX_MODEL);
	this->data.clear();
}

DX_Model::~DX_Model(){

}

int DirectX::LoadModel(std::string _file){

	LPD3DXBUFFER t_buf;	//�ꎞ�o�b�t�@
	Model_Data t_data;
	if(FAILED(D3DXLoadMeshFromX(_file.c_str(), D3DXMESH_SYSTEMMEM, this->device, NULL, &t_buf, NULL, &t_data.mat_num, &t_data.mesh_data))){
		OutputDebugString(((std::string)("�~"+_file+":�t�@�C����ǂݍ��߂܂���B\n")).c_str());
		return -1;
	}
	if(!(t_data.mesh_data->GetFVF() & D3DFVF_NORMAL)){	//�@�����Ȃ�����
		ID3DXMesh *tmp_mesh = NULL;
		t_data.mesh_data->CloneMeshFVF(
			t_data.mesh_data->GetOptions(),
			t_data.mesh_data->GetFVF() | D3DFVF_NORMAL,
			this->device,
			&tmp_mesh
			);
		D3DXComputeNormals(tmp_mesh, NULL);
		SAFE_RELEASE(t_data.mesh_data);	//�O�̂�j��
		t_data.mesh_data = tmp_mesh;	//�v�Z��
	}

	t_data.mat_data = new D3DMATERIAL9[t_data.mat_num];
	t_data.mesh_tex = new LPDIRECT3DTEXTURE9[t_data.mat_num];

	D3DXMATERIAL *t_mats = (D3DXMATERIAL *)t_buf->GetBufferPointer();
	for(int i=0;i<(int)t_data.mat_num;++i){
		//�}�e���A��
		t_data.mat_data[i] = t_mats[i].MatD3D;
		t_data.mat_data[i].Ambient = t_data.mat_data[i].Diffuse;

		t_data.mesh_tex[i] = NULL;
		HRESULT flg;
		if(t_mats[i].pTextureFilename != NULL){
			if(FAILED(flg = D3DXCreateTextureFromFile(this->device, ((std::string)(_str(t_mats[i].pTextureFilename)+"\0")).c_str(), &t_data.mesh_tex[i]))){
				OutputDebugString(((std::string)("�~"+_str(t_mats[i].pTextureFilename)+":�e�N�X�`�����ǂݍ��߂܂���B\n")).c_str());
			}
		}
	}
	SAFE_RELEASE(t_buf);
	this->data.push_back(new Model_Data(t_data));
	return this->data.size()-1;
}

void DirectX::DrawModel(int _hnd){
	if(_hnd < 0 || (int)this->data.size()-1 < _hnd){
		OutputDebugString("�~�����ȃ��f���n���h���ł��B\n");
		return;
	}
	//���[���h�ϊ�
	D3DXMATRIX world, rotate[3];
	D3DXMatrixIdentity(&world);
	D3DXMatrixRotationX(&rotate[0], this->data.at(_hnd)->ang.x);
	D3DXMatrixRotationY(&rotate[1], this->data.at(_hnd)->ang.y);
	D3DXMatrixRotationZ(&rotate[2], this->data.at(_hnd)->ang.z);
	world = world*rotate[0]*rotate[1]*rotate[2];
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
	this->device->SetFVF(this->data.at(_hnd)->mesh_data->GetFVF());

	//�`��ݒ�
	this->device->SetRenderState(D3DRS_LIGHTING, TRUE);
	this->device->SetRenderState(D3DRS_AMBIENT, 0x00ffffff);	//����
	this->device->SetRenderState(D3DRS_SPECULARENABLE, TRUE);	//�X�y�L�����[
	this->device->SetRenderState(D3DRS_ZENABLE, TRUE);			//Z�o�b�t�@

	for(unsigned int i=0;i<this->data.at(_hnd)->mat_num;++i){	//���b�V�����Ƃɕ`��
		this->device->SetMaterial(&this->data.at(_hnd)->mat_data[i]);
		this->device->SetTexture(0, this->data.at(_hnd)->mesh_tex[i]);
		this->data.at(_hnd)->mesh_data->DrawSubset(i);
	}
}