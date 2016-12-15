#include <sstream>
#include <Windows.h>

#include "Font.h"
#include "Sys.h"

Font_Vertex::Font_Vertex(float _x, float _y, float _z, float _u, float _v){
	this->x = _x;
	this->y = _y;
	this->z = _z;
	this->u = _u;
	this->v = _v;
}

Font_Vertex::Font_Vertex(){
	this->x = 0.f;
	this->y = 0.f;
	this->z = 0.f;
	this->u = 0.f;
	this->v = 0.f;
}

Font_data::Font_data(int _x, int _y, int _size){
	this->x = _x;
	this->y = _y;
	this->size = _size;
}

Font::Font(){
}

Font::~Font(){
	this->Dest();
}

static D3DXMATRIX world;
IDirect3DTexture9 *pTex = 0;

int Font::Init(){
	// フォントの生成
	setlocale(LC_ALL,"japanese");	//ワイド文字の設定
	this->font_size = 32;
	this->font_weight = 600;
	LOGFONT lf = {this->font_size, 0, 0, 0, this->font_weight, 0, 0, 0, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_MODERN, "メイリオ"};
	this->font = CreateFontIndirect(&lf);
	if(this->font == NULL){
		OutputDebugString("×デフォルトフォントの作成に失敗しました。\n");
		return -1;
	}

	// 単位フォントポリゴン作成
	this->vtx[0] = Font_Vertex(0.f, -1.f, 1.f, 0.f, 1.f);
	this->vtx[1] = Font_Vertex(0.f,  0.f, 1.f, 0.f, 0.f);
	this->vtx[2] = Font_Vertex(1.f, -1.f, 1.f, 1.f, 1.f);
	this->vtx[3] = Font_Vertex(1.f,  0.f, 1.f, 1.f, 0.f);
	Font_Vertex *p = NULL;

	//頂点ロック(重いのでメインループでは行わないこと)
       this->device->CreateVertexBuffer(sizeof(this->vtx), 0, 0, D3DPOOL_MANAGED, &this->vtx_buf, 0);
	this->vtx_buf->Lock(0, 0, (void **)&p, 0);
	memcpy(p, this->vtx, sizeof(this->vtx));
	this->vtx_buf->Unlock();

	OutputDebugString("○デフォルトフォントを生成しました。\n");
	return 0;
}

void Font::CreateChar(int _x, int _y, wchar_t data[], int _col){
	// デバイスにフォントを持たせないとGetGlyphOutline関数はエラーとなる
	this->font_hdc = GetDC(NULL);
	this->old_font = (HFONT)SelectObject(this->font_hdc, this->font);

	//フォントビットマップ取得
	UINT code = (UINT)data[0];
	const int gradFlag = GGO_GRAY2_BITMAP;
	int grad = 4; // 階調の最大値
	if(grad == 0){
		OutputDebugString("×フォントビットマップの作成に失敗しました。\n└アンチエイリアス階調設定の失敗\n");
		return;
	}

	TEXTMETRIC tm;
	GetTextMetrics(this->font_hdc, &tm );
	GLYPHMETRICS gm;
	CONST MAT2 mat = {{0,1},{0,0},{0,0},{0,1}};
	DWORD size = GetGlyphOutlineW(this->font_hdc, code, gradFlag, &gm, 0, NULL, &mat);
	BYTE *pMono = new BYTE[size];
	GetGlyphOutlineW(this->font_hdc, code, gradFlag, &gm, size, pMono, &mat);

	// デバイスコンテキストとフォントハンドルはもういらないので解放
	SelectObject(this->font_hdc, this->font_hdc);
	ReleaseDC(NULL, this->font_hdc);

	// テクスチャ作成

	int fontWidth = (gm.gmBlackBoxX+3)/4 * 4;
	int fontHeight = gm.gmBlackBoxY;
	if(FAILED(this->device->CreateTexture(fontWidth, fontHeight, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTex, NULL))){
		OutputDebugString("×テクスチャの生成に失敗しました。\n");
		delete[] pMono;
		return;
	}

	// テクスチャにフォントビットマップ情報を書き込み
	D3DLOCKED_RECT lockedRect;
	pTex->LockRect(0, &lockedRect, NULL, 0);  // ロック
	DWORD *pTexBuf = (DWORD*)lockedRect.pBits;   // テクスチャメモリへのポインタ

	for(int ty=0;ty<fontHeight;++ty){
		for(int tx=0;tx<fontWidth;++tx){
			DWORD alpha = pMono[ty*fontWidth+tx] * 255/grad;
			pTexBuf[ty*fontWidth+tx] = (alpha << 24) | _col;
		}
	}

	pTex->UnlockRect(0);
	delete[] pMono;

	//行列初期化
	D3DXMatrixIdentity(&world);
	this->device->SetTransform(D3DTS_WORLD, &world);
	this->device->SetTransform(D3DTS_VIEW, &world);
	this->device->SetTransform(D3DTS_PROJECTION, &world);

	D3DXMATRIX localScale;
	D3DXMATRIX localOffset;
	D3DXMatrixScaling(&localScale, (float)fontWidth, (float)fontHeight, 1.f);
	D3DXMatrixTranslation( &localOffset, (float)gm.gmptGlyphOrigin.x, (float)gm.gmptGlyphOrigin.y, 0.0f);
	D3DXMATRIX localMat = localScale * localOffset;

	int ox = _x-__SCR_X/2, oy = -_y+(__SCR_Y-32)/2;	//表示座標

	D3DXMATRIX worldOffset;
	D3DXMatrixTranslation( &worldOffset, (float)ox - 0.5f, (float)oy + 0.5f, 0.0f );
	world = localMat * worldOffset;

	D3DXMATRIX ortho;
	D3DXMatrixOrthoLH(&ortho, (float)__SCR_X, (float)__SCR_Y, 0.0f, 1000.0f);
	this->device->SetTransform(D3DTS_PROJECTION, &ortho);

	//描画
	this->device->SetRenderState(D3DRS_LIGHTING, FALSE);

	this->device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	this->device->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
	this->device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	this->device->SetRenderState(D3DRS_ZENABLE, TRUE);			//Zバッファ

	this->device->SetTransform(D3DTS_WORLD, &world);
	this->device->SetStreamSource(0, this->vtx_buf, 0, sizeof(Font_Vertex));
	this->device->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
	this->device->SetTexture(0, pTex);
	this->device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	pTex->Release();
}

void Font::DrawString(int _x, int _y, std::string _data, int _col){
	size_t len = 0;
	size_t buf = (_data.size()+1)*2;	//wchar_tの格納

	wchar_t *t_data = new wchar_t[buf];
	mbstowcs_s(&len, t_data, buf, _data.c_str(), _TRUNCATE);


	for(unsigned int i=0;i<buf/2;++i){
		CreateChar(_x+i*this->font_size, _y, &t_data[i], _col);
	}
	delete[] t_data;
}

void Font::Dest(){
	SelectObject(GetDC(NULL), this->old_font);
	ReleaseDC(NULL, this->font_hdc);

	SAFE_RELEASE(vtx_buf);
}
