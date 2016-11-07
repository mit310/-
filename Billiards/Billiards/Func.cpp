/*
 *�@���색�C�u�����̃A���S���Y�������ł��B
 *�@���̕����͊��ԓ��ɂ��ׂč�������̂ł͂Ȃ��A
 *�@�K�v�ȕ�����؂�o��DirectX9�p�ɕҏW�������̂ł��B
 */

#pragma once

#include <string>
#include <math.h>
#include <stdlib.h>
#include <d3dx9.h>
#include <sys\types.h>
#include <sys\timeb.h>
#include "Func.h"

static _timeb seed;

void SplitColor(int code, int *r, int *g, int *b){
	*r = code >> 16 & 0xff;
	*g = code >> 8 & 0xff;
	*b = code & 0xff;
}

void SetSeed(){
	_ftime_s(&seed);
}

int GetRand(int max){


	static unsigned long x = 123456789;
	static unsigned long y = 362436069;
	static unsigned long z = 521288629;
	static unsigned long w = seed.millitm^19<<8;
	unsigned long t;
	
	t = x^(x<<11);
	x = y;
	y = z;
	z = w;
	w = (w^(w>>19))^(t^(t>>8));
	return (int)w;
}

//�͈͓��̗����𐶐�(max���܂܂Ȃ�)
int GetRand2(int min, int max){
	if(min == max) return min;
	if(min > max){
		int a = max;
		max = min, min = a;
	}
	int t = min+GetRand(max-min-1);
	return t;
}

//0.f�`1.f�̒l��Ԃ�(7��)
float GetRandF(){
	long long int t = GetRand(1000000);	//�O�̂���32byte�m��
	return (float)t/1000000.f;	//tt��0�`1�̏���
}

//�͈͓��̎��������𐶐�(max���܂�, lg�ɏ����_�ȉ��̗L��������������(�����������܂߁A11���𒴂���ƕ΂肪������))
float GetRand2F(float min, float max, int lg){
	if(min == max) return min;
	if(min > max){
		float a = max;
		max = min, min = a;
	}

	//�����_���������߂�
	float t1 = max-min;	//���ۂ̕�
	float t2 = t1-(int)t1;	//�����_�ȉ������߂�[�K��+�ɂȂ�]
	t1 = (float)((int)t1);		//��������
	return min+GetRand((int)t1)+GetRandF()*t2;
}

//�����̗̈旐��
vec GetRandVec2F(float min, float max, int lg){
	return VGet(GetRand2F(min, max, lg), GetRand2F(min, max, lg), GetRand2F(min, max, lg));
}

vec GetRandVec2F(vec min, vec max){
	return VGet(GetRand2F(min.x, max.x), GetRand2F(min.y, max.y), GetRand2F(min.z, max.z));
}

//3D��ԏ�2�_�̋������Ƃ�
float GetDist(vec m1, vec m2){
	return fabs((float)sqrtf((m1.x-m2.x)*(m1.x-m2.x) + (m1.y-m2.y)*(m1.y-m2.y) + (m1.z-m2.z)*(m1.z-m2.z)));	//����
}



//������֌W

//���I�[�o�[�t���[�ɒ��ӁI
void to_char(char *dest[], std::string _src, int *_arrey){

	std::string src = _src+'\0';
	int size = src.length();

	if(_arrey != NULL) *_arrey = src.size();
	strcpy_s(*dest, size, src.c_str());

	return;
}


vec VZero = VGet(0.f, 0.f, 0.f);

vec VGet(float _x, float _y, float _z){
	return vec(_x, _y, _z);
}