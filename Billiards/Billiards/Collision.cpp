#include "Collision.h"
#include "Func.h"

bool Hit_Poly_Line(vec _face[], vec _lpos1, vec _lpos2, vec *_hit_pos){
	vec line_vec = _lpos2-_lpos1;
	vec f_norm = VNorm(_face);

	//平面に重なっているか
	if(fabs(VDot(f_norm, _face[0]-_lpos1)) < 0.000001f){
		//*_hit_pos = _lpos1;
		return false;
	}
	//平面と並行か
	if(fabs(VDot(f_norm, line_vec)) < 0.000001f) return false;

	//線の始点・終点が面を貫通しているか
	//※貫通していたら内積結果符号が異なる = 掛け算して+なら衝突していない
	float st = VDot(f_norm, _lpos1-_face[0]);
	float ed = VDot(f_norm, _lpos2-_face[0]);
	float tt = st*ed;
	if(tt > 0.f){

		return false;
	}

	//内分点から貫通点を算出、外積を使いポリゴン内に貫通点があるかチェック
	float d1 = GetDist_Face_Point(_face, _lpos1);	//始点から面までの距離
	float d2 = GetDist_Face_Point(_face, _lpos2);	//終点から面までの距離
	float ratio = d1/(d1+d2);	//比率を出す
	vec t_vec;
	D3DXVec3Lerp(&t_vec, &_lpos1, &_lpos2, ratio);	//貫通点を算出

	vec t_face[3] = {_face[0], _face[1], _face[2]};
	vec face_cross = VNorm(t_face);
	vec p_cross;
	for(int i=0;i<3;++i){
		p_cross = VUnit(VCross(t_face[(i+1)%3]-t_face[i], t_vec-t_face[(i+1)%3]));
		if(
			abs(face_cross.x-p_cross.x) > 0.0001f ||
			abs(face_cross.y-p_cross.y) > 0.0001f ||
			abs(face_cross.z-p_cross.z) > 0.0001f
		){
			return false;
		}
	}
	*_hit_pos = t_vec;
	return true;
}

vec ReflectVector(const vec &_sp, const vec &_norm){
    vec normal_n;
	vec res;
	normal_n = VUnit(_norm);	//念のため正規化
    D3DXVec3Normalize(&res, &(_sp - 2.f*VDot(_sp, _norm) * _norm));
	return res;
}