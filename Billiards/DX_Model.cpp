#include "Collision.h"
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
	t_data.scale = VGet(1.f, 1.f, 1.f);
	t_data.ang = VZero;
	t_data.pos = VZero;
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

	D3DXMATRIX world, trans, rotate[3], scale;
	//�s�񏉊���
	D3DXMatrixIdentity(&world);
	this->device->SetTransform(D3DTS_WORLD, &world);
	this->device->SetTransform(D3DTS_VIEW, &world);
	this->device->SetTransform(D3DTS_PROJECTION, &world);

	D3DXMatrixTranslation(&trans,
		this->data.at(_hnd)->pos.x,
		this->data.at(_hnd)->pos.y,
		this->data.at(_hnd)->pos.z
		);
	D3DXMatrixRotationX(&rotate[0], this->data.at(_hnd)->ang.x);
	D3DXMatrixRotationY(&rotate[1], this->data.at(_hnd)->ang.y);
	D3DXMatrixRotationZ(&rotate[2], this->data.at(_hnd)->ang.z);
	D3DXMatrixScaling(&scale,
		this->data.at(_hnd)->scale.x,
		this->data.at(_hnd)->scale.y,
		this->data.at(_hnd)->scale.z
		);

	world = world*rotate[0]*rotate[1]*rotate[2]*scale*trans;
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
	this->device->SetPixelShader(NULL);
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



Model_HitResult_Dim DirectX::GetHitColl_Model_Lay(int _hnd, vec _pos1, vec _pos2){
	Model_HitResult_Dim t_hit;	//�q�b�g�f�[�^

	if(_hnd < 0 || (int)this->data.size()-1 < _hnd){
		OutputDebugString("�~�����ȃ��f���n���h���ł��B\n");
		return t_hit;
	}

	//�����̂ŏȗ��p
	Model_Data *t = this->data.at(_hnd);


	//�o�b�t�@�e��
	BYTE *vtx_buf = NULL;
	WORD *index_buf = NULL;

	DWORD face_num = t->mesh_data->GetNumFaces();
	DWORD vtx_num = t->mesh_data->GetNumVertices();

	DWORD fvf = t->mesh_data->GetFVF();	//fvf
	DWORD vtx_size = D3DXGetFVFVertexSize(fvf);	//���_�`���f�[�^�T�C�Y

	//��������ǂݍ��݋��݂̂ɂ��Ă���
	HRESULT flg;
	flg = t->mesh_data->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID *)&vtx_buf);
	if(FAILED(flg))	OutputDebugString("�~���_���b�N�Ɏ��s\n");
	flg = t->mesh_data->LockIndexBuffer(D3DLOCK_READONLY, (LPVOID *)&index_buf);
	if(FAILED(flg))	OutputDebugString("�~���_���b�N�Ɏ��s\n");

	vec t_pos[3] = {VZero, VZero, VZero};

	//�|���S�����W�擾
	for(DWORD i=0;i<face_num;++i){

		D3DXMATRIX world, trans, rotate[3], scale;
		for(int j=0;j<3;++j){
			t_pos[j] = *(vec *)(vtx_buf + index_buf[i*3 + j]*vtx_size);
	
			//���f�����ړ��E��]�E�g��k�������Ă��邱�Ƃ��l��
			
			world._41 = t_pos[j].x;
			world._42 = t_pos[j].y;
			world._43 = t_pos[j].z;

			D3DXMatrixTranslation(&trans,
				this->data.at(_hnd)->pos.x,
				this->data.at(_hnd)->pos.y,
				this->data.at(_hnd)->pos.z
			);
			D3DXMatrixRotationX(&rotate[0], this->data.at(_hnd)->ang.x);
			D3DXMatrixRotationY(&rotate[1], this->data.at(_hnd)->ang.y);
			D3DXMatrixRotationZ(&rotate[2], this->data.at(_hnd)->ang.z);
			D3DXMatrixScaling(&scale,
				this->data.at(_hnd)->scale.x,
				this->data.at(_hnd)->scale.y,
				this->data.at(_hnd)->scale.z
			);

			world = world*rotate[0]*rotate[1]*rotate[2]*scale*trans;
			t_pos[j].x = world._41;
			t_pos[j].y = world._42;
			t_pos[j].z = world._43;
		}




		//�Փ˔���
		vec t_hitpos = VZero;
		if(Hit_Poly_Line(t_pos, _pos1, _pos2, &t_hitpos)){
			t_hit.data.push_back(new Model_HitResult());
			t_hit.data.back()->hit_poly[0] = t_pos[0];
			t_hit.data.back()->hit_poly[1] = t_pos[1];
			t_hit.data.back()->hit_poly[2] = t_pos[2];
			t_hit.data.back()->pos = t_hitpos;
			t_hit.data.back()->norm = VNorm(t_pos);
		}
	}


	t->mesh_data->UnlockIndexBuffer();
	t->mesh_data->UnlockVertexBuffer();

	return t_hit;
}

void DirectX::DeleteHitColl(Model_HitResult_Dim _hit){
	for(unsigned int i=0;i<_hit.data.size();++i){
		if(_hit.data.at(i) != NULL){
			delete _hit.data.at(i);
			_hit.data.at(i) = NULL;
		}
	}
	_hit.data.clear();
}