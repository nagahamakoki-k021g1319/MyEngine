#include "Obstacle.h"
#include "Player.h"

Obstacle::Obstacle()
{
}

Obstacle::~Obstacle()
{
	for (int i = 0; i < 8; i++) { delete obstacleObj_[i]; }
	delete obstacleModel_;
}

void Obstacle::Initialize(DirectXCommon* dxCommon, Input* input)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);

	this->dxCommon_ = dxCommon;
	input_ = input;

	//飛んでくる障害物
	obstacleModel_ = Model::LoadFromOBJ("rock");
	for (int i = 0; i < 8; i++) {
		obstacleObj_[i] = Object3d::Create();
		obstacleObj_[i]->SetModel(obstacleModel_);
		obstacleObj_[i]->wtf.scale = { 1.0f,1.0f,1.0f };
	}
	obstacleObj_[0]->wtf.position = { 0.5f,0.3f,40.0f };
	obstacleObj_[1]->wtf.position = { -2.0f,-1.0f,48.0f };
	obstacleObj_[2]->wtf.position = { 3.0f,0.4f,56.0f };
	obstacleObj_[3]->wtf.position = { 0.3f,-0.1f,64.0f }; //27

	obstacleObj_[4]->wtf.position = { 0.5f,0.6f,90.0f };
	obstacleObj_[5]->wtf.position = { -2.0f,-0.7f,98.0f };
	obstacleObj_[6]->wtf.position = { 3.0f,0.7f,106.0f };
	obstacleObj_[7]->wtf.position = { 0.3f,0.2f,114.0f };
}

void Obstacle::Update()
{
	for (int i = 0; i < 8; i++) {
		obstacleObj_[i]->Update();
	}

	////障害物が動き始める
	//if (enemy_->bossGostAt == true) {
	//	for (int i = 0; i < 8; i++) {
	//		obstacleObj_[i]->wtf.rotation.z -= 0.03f;
	//		obstacleObj_[i]->wtf.position.z -= 0.3f;
	//	}

	//}

}

void Obstacle::Draw()
{
	/*if (enemy_->bossGostAt == true) {
		for (int i = 0; i < 8; i++) {
			if (isObsAliveFlag_[i] == 0) {
				obstacleObj_[i]->Draw();
			}
		}
	}*/
}

void Obstacle::FbxDraw()
{
}

Vector3 Obstacle::GetWorldPosition()
{
	return Vector3();
}
