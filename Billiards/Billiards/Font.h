#pragma once
#include <string>
#include <vector>

#include "DirectX.h"

struct Font_Vertex {
	float x, y, z;
	float u, v;
	Font_Vertex();
	Font_Vertex(float _x, float _y, float _z, float _u, float _v);
};

struct Font_data {
	int x, y;
	int size;
	int ang;
	Font_data(int _x, int _y, int _size);
};

class Font : public DirectX {
	static const int MAX_CHARACTER = 1024;
	int font_size;
	int font_weight;
	Font_Vertex vtx[4];
	HFONT font;	//フォントハンドル
	HFONT old_font;	//バックアップ。解放時に使う
	HDC font_hdc;	//フォント生成用にインスタンスを取得

	IDirect3DVertexBuffer9 *vtx_buf;	//頂点バッファ
	std::vector<Font_data *> data;

public:
	int Init();
	void CreateChar(int _x, int _y, wchar_t data[], int _col = 0x00ffffff);
	void DrawString(int _x, int _y, std::string _data, int _col);
	void Dest();
	void dbg(TEXTMETRIC tm, GLYPHMETRICS gm, int ox, int oy);
	Font();
	~Font();
};