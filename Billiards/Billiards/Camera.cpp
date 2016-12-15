#include "Ball.h"
#include "Camera.h"
#include "DirectX.h"
#include "Input.h"
#include "Process.h"

static const float MOVE_CAM_SPEED = R(5.f);
static const float MAX_CAM_ANGLE = R(60.f);
static const float MIN_CAM_ANGLE = R(15.f);

void Camera::Init(){
	this->dist = 1.5f;
	this->h_ang = R(0.f);
	this->v_ang = R(45.f);
}

void Camera::Process(){

	//‘€ì
	if(key.GetKeyPush(DIK_F)){	//‰“‚´‚¯‚é
		this->dist += MOVE_CAM_SPEED;
	}
	if(key.GetKeyPush(DIK_N)){	//‰“‚´‚¯‚é
		this->dist -= MOVE_CAM_SPEED;
	}

	if(key.GetKeyPush(DIK_A)){	//©‰ñ“]
		this->h_ang -= MOVE_CAM_SPEED;
	}

	if(key.GetKeyPush(DIK_D)){	//©‰ñ“]
		this->h_ang += MOVE_CAM_SPEED;
	}

	if(this->v_ang < MAX_CAM_ANGLE && key.GetKeyPush(DIK_W)){	//ª‰ñ“]
		this->v_ang += MOVE_CAM_SPEED;
		if(MAX_CAM_ANGLE < this->v_ang) this->v_ang = MAX_CAM_ANGLE;
	}

	if(MIN_CAM_ANGLE < this->v_ang && key.GetKeyPush(DIK_S)){	//«‰ñ“]
		this->v_ang -= MOVE_CAM_SPEED;
		if(this->v_ang < MIN_CAM_ANGLE) this->v_ang = MIN_CAM_ANGLE;
	}


	//”½‰f
	vec t_vec = VGet(this->dist, 0.f, 0.f);
	D3DXMATRIX t_mtx = ToMatrix(t_vec);
	D3DXMATRIX rot_z, rot_y;
	D3DXMatrixRotationZ(&rot_z, this->v_ang);	//ã‰º•ûŒü‰ñ“]
	D3DXMatrixRotationY(&rot_y, this->h_ang);	//…•½•ûŒü‰ñ“]
	t_mtx = t_mtx*rot_z*rot_y;
	t_vec = ToVec(t_mtx);	//’Ž‹“_‚©‚ç‚ÌƒJƒƒ‰ˆÊ’u


	dx.SetCamera(
		ball.GetDataList().at(0)->GetPos()+t_vec+VGet(0.f, 0.1f, 0.f),
		ball.GetDataList().at(0)->GetPos()+VGet(0.f, 0.1f, 0.f),
		VGet(0.f, 1.f, 0.f)
	);

}

void Camera::GetAngle(float *_h, float *_v){
	if(_h != NULL) *_h = this->h_ang;
	if(_v != NULL) *_v = this->v_ang;
}
