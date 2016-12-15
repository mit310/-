#include "Collision.h"
#include "Func.h"

bool Hit_Poly_Line(vec _face[], vec _lpos1, vec _lpos2, vec *_hit_pos){
	vec line_vec = _lpos2-_lpos1;
	vec f_norm = VNorm(_face);

	//���ʂɏd�Ȃ��Ă��邩
	if(fabs(VDot(f_norm, _face[0]-_lpos1)) < 0.000001f){
		//*_hit_pos = _lpos1;
		return false;
	}
	//���ʂƕ��s��
	if(fabs(VDot(f_norm, line_vec)) < 0.000001f) return false;

	//���̎n�_�E�I�_���ʂ��ђʂ��Ă��邩
	//���ђʂ��Ă�������ό��ʕ������قȂ� = �|���Z����+�Ȃ�Փ˂��Ă��Ȃ�
	float st = VDot(f_norm, _lpos1-_face[0]);
	float ed = VDot(f_norm, _lpos2-_face[0]);
	float tt = st*ed;
	if(tt > 0.f){

		return false;
	}

	//�����_����ђʓ_���Z�o�A�O�ς��g���|���S�����Ɋђʓ_�����邩�`�F�b�N
	float d1 = GetDist_Face_Point(_face, _lpos1);	//�n�_����ʂ܂ł̋���
	float d2 = GetDist_Face_Point(_face, _lpos2);	//�I�_����ʂ܂ł̋���
	float ratio = d1/(d1+d2);	//�䗦���o��
	vec t_vec;
	D3DXVec3Lerp(&t_vec, &_lpos1, &_lpos2, ratio);	//�ђʓ_���Z�o

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
	normal_n = VUnit(_norm);	//�O�̂��ߐ��K��
    D3DXVec3Normalize(&res, &(_sp - 2.f*VDot(_sp, _norm) * _norm));
	return res;
}