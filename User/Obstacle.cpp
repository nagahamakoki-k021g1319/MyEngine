#include "Obstacle.h"
#include "Player.h"

Obstacle::Obstacle()
{
}

Obstacle::~Obstacle()
{
	for (int i = 0; i < 4; i++) { delete obstacleObj_[i]; }
	delete obstacleModel_;
}

void Obstacle::Initialize(DirectXCommon* dxCommon, Input* input)
{
	// nullptr�`�F�b�N
	assert(dxCommon);
	assert(input);

	this->dxCommon_ = dxCommon;
	input_ = input;

	//���ł����Q��
	obstacleModel_ = Model::LoadFromOBJ("rock");
	for (int i = 0; i < 4; i++) {
		obstacleObj_[i] = Object3d::Create();
		obstacleObj_[i]->SetModel(obstacleModel_);
		obstacleObj_[i]->wtf.scale = { 1.0f,1.0f,1.0f };
	}
	obstacleObj_[0]->wtf.position = { 0.5f,0.3f,40.0f };
	obstacleObj_[1]->wtf.position = { -2.0f,-1.0f,48.0f };
	obstacleObj_[2]->wtf.position = { 3.0f,0.4f,56.0f };
	obstacleObj_[3]->wtf.position = { 0.3f,-0.1f,64.0f }; //27

}

void Obstacle::Update()
{
	for (int i = 0; i < 4; i++) {
		obstacleObj_[i]->Update();
	}

	//��Q���������n�߂�
	if (enemy_->bossGostAt == true) {
		for (int i = 0; i < 4; i++) {
			obstacleObj_[i]->wtf.rotation.z -= 0.03f;
			obstacleObj_[i]->wtf.position.z -= 0.3f;
		}

	}

}

void Obstacle::Draw()
{
	if (enemy_->bossGostAt == true) {
		for (int i = 0; i < 4; i++) {
			if (isObsAliveFlag_[i] == 0) {
				obstacleObj_[i]->Draw();
			}
		}
	}
}

void Obstacle::FbxDraw()
{
}

Vector3 Obstacle::GetWorldPosition()
{
	return Vector3();
}
