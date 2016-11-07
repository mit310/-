#pragma once
#include <vector>
#include <d3d9.h>
#include <d3dx9.h>
#include "Func.h"

//モデルデータ構造体
struct Model_Data {
	//モデル本体の情報
	LPD3DXMESH mesh_data;
	DWORD mat_num;
	D3DMATERIAL9 *mat_data;
	LPDIRECT3DTEXTURE9 *mesh_tex;

	//モデルの状態
	vec pos;
	vec ang;	//角度(軸)
};

//モデル管理クラス
class DX_Model {
protected:
	static const int MAX_MODEL = 1024;
	std::vector<Model_Data *> data;
public:
	DX_Model();
	~DX_Model();
};