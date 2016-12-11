#include "Ball.h"
#include "Framerate.h"
#include "Input.h"
#include "Process.h"
#include "DirectX.h"

Ball::Ball(){
	this->data.reserve(BALL_NUM);
	this->data.clear();
}

Ball::~Ball(){
	//全てのボールを削除
	this->DeleteAll();
}

void Ball::Init(){
	this->Add(0, VGet(0.0f, 0.2f, -1.0f), true);

	this->Add(1, VGet(0.f, 0.2f, 0.1f), false);

	this->Add(2, VGet(-0.05f, 0.2f, 0.2f), false);
	this->Add(3, VGet( 0.05f, 0.2f, 0.2f), false);

	this->Add(4, VGet(-0.1f, 0.2f, 0.3f), false);
	this->Add(5, VGet( 0.f, 0.2f, 0.3f), false);
	this->Add(6, VGet( 0.1f, 0.2f, 0.3f), false);

	this->Add( 7, VGet(-0.15f, 0.2f, 0.4f), false);
	this->Add( 8, VGet(-0.05f, 0.2f, 0.4f), false);
	this->Add( 9, VGet(0.05f, 0.2f, 0.4f), false);
	this->Add(10, VGet(0.15f, 0.2f, 0.4f), false);
}

void Ball::Process(){
	for(std::vector<Ball_data *>::iterator it=this->data.begin();it!=this->data.end();){
		(*it)->Process();
		//球が落下していたら削除し、得点に加える
		++it;

	}

	//球同士の衝突判定
	for(unsigned int i=0;i<this->data.size();++i){
		for(unsigned int j=0;j<this->data.size();++j){
			if(i == j) continue;

			//ボールが衝突している場合
			if(GetDist(this->data.at(i)->GetPos(), this->data.at(j)->GetPos()) < this->data.at(i)->GetBallSize()+this->data.at(j)->GetBallSize()){
				//反射方向を算出する
				float ref_rate = (1.f + 1.f);	//反発率(今回は1.f)
				float weight = 2.f;	//質量(今回はすべての玉が1.f)
				vec t_axis;
				D3DXVec3Normalize(&t_axis, &(this->data.at(i)->GetPos()-this->data.at(j)->GetPos()));	//衝突軸

				//内積
				float t = VDot(this->data.at(i)->GetSpeed()-this->data.at(j)->GetSpeed(), t_axis);
				vec c_vec = ref_rate*t/weight*t_axis;
				this->data.at(i)->SetSpeed(-1.f*c_vec+this->data.at(i)->GetSpeed());
				this->data.at(j)->SetSpeed( 1.f*c_vec+this->data.at(j)->GetSpeed());

			}
		}
	}
}

void Ball::Draw(){
	for(unsigned int i=0;i<data.size();++i){
		this->data.at(i)->Draw();
	}
}

void Ball::Delete(int _id){
	for(std::vector<Ball_data *>::iterator it=this->data.begin();it!=this->data.end();){
		if((*it)->GetId() == _id){
			if(*it != NULL){
				delete (*it);
				*it = NULL;
			}
			it = this->data.erase(it);
		} else ++it;
		//球が落下していたら削除し、得点に加える
	}
}

void Ball::DeleteAll(){
	for(unsigned int i=0;i<this->data.size();++i){
		if(this->data.at(i) != NULL){
			delete this->data.at(i);
			this->data.at(i) = NULL;
		}
	}
	this->data.clear();
}

void Ball::Add(int _id, vec _pos, bool _my){
	this->data.push_back(new Ball_data(_id, _pos, _my));
}

void Ball::AddSpeed(int _id, vec _sp){
	for(unsigned int i=0;i<this->data.size();++i){
		if(this->data.at(i)->GetId() == _id){
			this->data.at(i)->SetSpeed(this->data.at(i)->GetSpeed()+_sp);
			return;
		}
	}
}

std::vector<Ball_data *> Ball::GetDataList(){
	return this->data;
}

Ball_data::Ball_data(int _id, vec _pos, bool _my){
	this->id = _id;
	this->hnd = -1;
	this->pos = _pos;
	this->speed = VZero;
	this->a = VZero;
	this->my = _my;
	this->g = 0.f;
	this->r = 1.f;	//今回は半径固定

	this->Init();
}

Ball_data::~Ball_data(){
}

void Ball_data::Init(){
	if(this->my) this->hnd = dx.LoadModel("my_ball.x");
	else this->hnd = dx.LoadModel("ball_"+_str(this->id)+".x");

	if(this->hnd == -1){
		OutputDebugString("予期しないエラー\n└モデルの読み込みに失敗しました。");
	}

	dx.SetPositionModel(this->hnd, this->pos);
	dx.SetRotateModel(this->hnd, VZero);
	dx.SetScaleModel(this->hnd, VGet(0.05f, 0.05f, 0.05f));
	this->r *= 0.1f;

}

void Ball_data::Process(){

	Model_HitResult_Dim res = dx.GetHitColl_Model_Lay(table_hnd, this->pos, this->pos-VGet(0.f, this->r, 0.f));
	if(res.data.empty()){
		if(fps.GetFPS() > 0.1f){
			this->g += G *1/60.f * 0.01f;
			this->speed += VGet(0.f, -this->g, 0.f);

		}
	} else {
		this->pos = res.data.at(0)->pos+VGet(0.f, this->r, 0.f);
		this->g = 0.f;
		this->speed.y = 0.f;
	}
	dx.DeleteHitColl(res);
	this->speed += this->a;
	this->pos += this->speed;
	
	//モデルデータ更新
	dx.SetPositionModel(this->hnd, this->pos);
	dx.SetRotateModel(this->hnd, VZero);
}

void Ball_data::Draw(){
	dx.DrawModel(this->hnd);

}

int Ball_data::GetId(){
	return this->id;
}

vec Ball_data::GetPos(){
	return this->pos;
}

vec Ball_data::GetSpeed(){
	return this->speed;
}

float Ball_data::GetBallSize(){
	return this->r;
}

void Ball_data::SetSpeed(vec _sp){
	this->speed = _sp;
}

#ifdef _DEBUG
float Ball_data::GetG(){
	return this->g;
}
#endif