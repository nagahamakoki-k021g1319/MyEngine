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
	// nullptrÉ`ÉFÉbÉN
	assert(dxCommon);
	assert(input);

	this->dxCommon = dxCommon;
	input_ = input;

	//îÚÇÒÇ≈Ç≠ÇÈè·äQï®
	obstacleModel_ = Model::LoadFromOBJ("rock");
	for (int i = 0; i < 4; i++) {
		obstacleObj_[i] = Object3d::Create();
		obstacleObj_[i]->SetModel(obstacleModel_);
		obstacleObj_[i]->wtf.scale = { 0.08f,0.08f,0.08f };
	}
	obstacleObj_[0]->wtf.position = { 0.5f,0.3f,12.0f };
	obstacleObj_[1]->wtf.position = { -0.5f,0.1f,17.0f };
	obstacleObj_[2]->wtf.position = { -0.1f,0.4f,22.0f };
	obstacleObj_[3]->wtf.position = { 0.3f,-0.1f,27.0f }; //27

}

void Obstacle::Update()
{
	for (int i = 0; i < 4; i++) {
		obstacleObj_[i]->Update();
	}

	//è·äQï®Ç™ìÆÇ´énÇﬂÇÈ
	if (enemy_->bossGostAt == true) {
		

	}

	//ìñÇΩÇËîªíË(é©ã@íeÇ∆éGãõìG)
	for (int i = 0; i < 4; i++) {
		if (coll.CircleCollision(player_->GetSwordWorldPosition(), obstacleObj_[i]->wtf.position, 0.1f, 0.1f)) {
			isObsAliveFlag_[i] = 1;
		};
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
