#pragma once
#include <sstream>
#include <iomanip>
#include <d3dx9.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include "Sys.h"	//�V�X�e���萔�Q

#define vec D3DXVECTOR3
#define R(r) ((float)(r*D3DX_PI/180.f))	//�x���@���ʓx�@
#define RR(r) ((float)(r*180.f/D3DX_PI))	//�ʓx�@���x���@

#define SAFE_RELEASE(p) if(p){ p->Release(); p = NULL; }
#define SAFE_DELETE(p) if(p){ delete[] p; p = NULL; }

#define _str to_string



//�摜�֘A
void SplitColor(int code, int *r, int *g, int *b);

//3D�֘A
bool GetCol_Sphere(vec m1, float r1, vec m2, float r2);
float GetDist(vec m1, vec m2);

//�x�N�g���g��
extern vec VZero;
extern vec VNorm(const vec *_pos);	//�ʖ@��
extern vec VUnit(const vec _pos);	//���K��
extern vec VGet(float _x, float _y, float _z);
extern vec VCross(const vec _pos1, const vec _pos2);
extern float VDot(const vec _pos1, const vec _pos2);
extern float VSize(const vec _pos);	//�x�N�g����傫���ɂ���
extern float GetDist_Face_Point(const vec *_face_pos, vec _point_pos);

extern vec ToVec(D3DXMATRIX _mtx);
D3DXMATRIX ToMatrix(vec _vector);


//���S�ȃx�N�^�[�z��̃R�s�[
template <class T> typename void Copy(std::vector<T> src, std::vector<T> *dest){
	if(dest->size() < src.size()){
		OutputDebugString(((std::string)("���z���"+_str(src.size())+"�Ɋg��\n")).c_str());
		dest->resize(src.size());
	}
	try {
		std::copy(src.begin(), src.end(), &dest);
	} catch(std::exception e){
		OutputDebugString(e.what());
	}
}

//�x�N�^�[�z��̍폜
template <class T> typename std::vector<T>::iterator Erase(std::vector<T> *data, int id){
	std::vector<T>::iterator it = data->begin();
	it += id;
	it = data->erase(it);
	return it;
}

//�w�肵���x�N�^�[�z����w�肵�������o�ŏ���/�~���\�[�g
//���w��ł��郁���o�͔�r���Z < ���g����int double�Ȃǂ̐�����std::string�̂�
//�y�o�b�t�@�\�[�g����
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

//�����֌W
void SetSeed();
int GetRand(int max);
int GetRand2(int min, int max);
float GetRandF();
float GetRand2F(float min, float max, int lg = 6);
vec GetRandVec2F(float min, float max, int lg = 6);
vec GetRandVec2F(vec min, vec max);

//�����񑀍�
template <class T> std::string to_string(T num, int ln = -1, char len = '\0'){
	std::stringstream a;
	if(ln == -1 || len == '\0') a << num;
	else a << std::setw(ln) << std::setfill(len) << num;
	std::string str(a.str());
	return str;
}

//�I�[�o�[���C�h�ɂ����ꉻ(bool��"true"/"false")
template <> inline std::string to_string<bool>(bool num, int ln, char len){
	std::string str;
	if(!num) str = "false";
	else str = "true";
	return str;
}
/*
 *  stream��char *�ϊ��͕W���œ����Ă���͗l
template <> inline std::string to_string<char *>(char *num){
	std::string str = "";
	for(int i=0;*(num+i)!='\0';i++){
		str += *(num+i);
	}
	return str;
}
*/

//num���w�肵���l�ɂ���B�߂Â��鑬�x��a
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

//std::string��char[]�֕ϊ�
void to_char(char *dest[], std::string _src, int *_arrey = NULL);
void DrawStringCode(int _x, int _y, std::string _len, float size, int col = 0xffffff);
