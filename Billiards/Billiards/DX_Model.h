#pragma once
#include <vector>
#include <d3d9.h>
#include <d3dx9.h>
#include "Func.h"

//���f���f�[�^�\����
struct Model_Data {
	//���f���{�̂̏��
	LPD3DXMESH mesh_data;
	DWORD mat_num;
	D3DMATERIAL9 *mat_data;
	LPDIRECT3DTEXTURE9 *mesh_tex;

	//���f���̏��
	vec pos;
	vec ang;	//�p�x(��)
};

//���f���Ǘ��N���X
class DX_Model {
protected:
	static const int MAX_MODEL = 1024;
	std::vector<Model_Data *> data;
public:
	DX_Model();
	~DX_Model();
};