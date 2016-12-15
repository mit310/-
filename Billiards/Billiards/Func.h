#pragma once
#include <sstream>
#include <iomanip>
#include <d3dx9.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include "Sys.h"	//システム定数群

#define vec D3DXVECTOR3
#define R(r) ((float)(r*D3DX_PI/180.f))	//度数法→弧度法
#define RR(r) ((float)(r*180.f/D3DX_PI))	//弧度法→度数法

#define SAFE_RELEASE(p) if(p){ p->Release(); p = NULL; }
#define SAFE_DELETE(p) if(p){ delete[] p; p = NULL; }

#define _str to_string



//画像関連
void SplitColor(int code, int *r, int *g, int *b);

//3D関連
bool GetCol_Sphere(vec m1, float r1, vec m2, float r2);
float GetDist(vec m1, vec m2);

//ベクトル拡張
extern vec VZero;
extern vec VNorm(const vec *_pos);	//面法線
extern vec VUnit(const vec _pos);	//正規化
extern vec VGet(float _x, float _y, float _z);
extern vec VCross(const vec _pos1, const vec _pos2);
extern float VDot(const vec _pos1, const vec _pos2);
extern float VSize(const vec _pos);	//ベクトルを大きさにする
extern float GetDist_Face_Point(const vec *_face_pos, vec _point_pos);

extern vec ToVec(D3DXMATRIX _mtx);
D3DXMATRIX ToMatrix(vec _vector);


//安全なベクター配列のコピー
template <class T> typename void Copy(std::vector<T> src, std::vector<T> *dest){
	if(dest->size() < src.size()){
		OutputDebugString(((std::string)("◇配列を"+_str(src.size())+"に拡張\n")).c_str());
		dest->resize(src.size());
	}
	try {
		std::copy(src.begin(), src.end(), &dest);
	} catch(std::exception e){
		OutputDebugString(e.what());
	}
}

//ベクター配列の削除
template <class T> typename std::vector<T>::iterator Erase(std::vector<T> *data, int id){
	std::vector<T>::iterator it = data->begin();
	it += id;
	it = data->erase(it);
	return it;
}

//指定したベクター配列を指定したメンバで昇順/降順ソート
//※指定できるメンバは比較演算 < が使えるint doubleなどの数字とstd::stringのみ
//Ｚバッファソート向け
template <class T, class S> typename void SortEx(std::vector<T> *data, S T::* ptr, char sg = '<'){
	switch(sg){
	case '<':
		std::sort(data->begin(), data->end(), [&](const T &lt, const T &rt){return lt.*ptr < rt.*ptr;});
		break;
	case '>':
		std::sort(data->begin(), data->end(), [&](const T &lt, const T &rt){return lt.*ptr > rt.*ptr;});
		break;
	}
}

template <class T, class S> typename void SortEx(std::vector<T> *data, S (T::* ptr)(), char sg = '<'){
	switch(sg){
	case '<':
		std::sort(data->begin(), data->end(), [&](const T &lt, const T &rt){return lt.*ptr() < rt.*ptr();});
		break;
	case '>':
		std::sort(data->begin(), data->end(), [&](const T &lt, const T &rt){return lt.*ptr() > rt.*ptr();});
		break;
	}
}

//乱数関係
void SetSeed();
int GetRand(int max);
int GetRand2(int min, int max);
float GetRandF();
float GetRand2F(float min, float max, int lg = 6);
vec GetRandVec2F(float min, float max, int lg = 6);
vec GetRandVec2F(vec min, vec max);

//文字列操作
template <class T> std::string to_string(T num, int ln = -1, char len = '\0'){
	std::stringstream a;
	if(ln == -1 || len == '\0') a << num;
	else a << std::setw(ln) << std::setfill(len) << num;
	std::string str(a.str());
	return str;
}

//オーバーライドによる特殊化(bool→"true"/"false")
template <> inline std::string to_string<bool>(bool num, int ln, char len){
	std::string str;
	if(!num) str = "false";
	else str = "true";
	return str;
}
/*
 *  streamにchar *変換は標準で入っている模様
template <> inline std::string to_string<char *>(char *num){
	std::string str = "";
	for(int i=0;*(num+i)!='\0';i++){
		str += *(num+i);
	}
	return str;
}
*/

//numを指定した値にする。近づける速度はa
template <class T> inline void SetTrgLim(T *num, T lim, T a){
	if(*num == lim) return;
	if(*num < lim){
		*num += a;
		if(lim < *num){
			*num = lim;
			return;
		}
	}
	if(lim < *num){
		*num -= a;
		if(*num < lim){
			*num = lim;
			return;
		}
	}
}

//std::string→char[]へ変換
void to_char(char *dest[], std::string _src, int *_arrey = NULL);
void DrawStringCode(int _x, int _y, std::string _len, float size, int col = 0xffffff);
