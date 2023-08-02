#include "EnemyBoss.h"
#include "Enemy.h"

EnemyBoss::EnemyBoss()
{
}

EnemyBoss::~EnemyBoss()
{
	delete fbxModel_;
	delete fbxObject3d_;
}

void EnemyBoss::Initialize(DirectXCommon* dxCommon, Input* input)
{
	// nullptr�`�F�b�N
	assert(dxCommon);
	assert(input);

	this->dxCommon = dxCommon;
	input_ = input;

	fbxModel_ = FbxLoader::GetInstance()->LoadModelFromFile("kuma");

	fbxObject3d_ = new FBXObject3d;
	fbxObject3d_->Initialize();
	fbxObject3d_->SetModel(fbxModel_);
	fbxObject3d_->wtf.position = { 0.0f,-0.3f,30.0f };
	fbxObject3d_->wtf.rotation = { 0.0f,2.7f,0.0f };
	fbxObject3d_->PlayAnimation(1.0f, true);

}

void EnemyBoss::Update()
{
	fbxObject3d_->Update();

	if (enemy_->bossGostAt == true) {

	}
}

void EnemyBoss::Draw()
{
	
}

void EnemyBoss::FbxDraw()
{
	/*if (enemy_->bossGostAt == true) {
		fbxObject3d_->Draw(dxCommon->GetCommandList());
	}*/
}

Vector3 EnemyBoss::GetWorldPosition()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;

	fbxObject3d_->wtf.UpdateMat();
	//���[���h�s��̕��s�ړ�����
	worldPos.x = fbxObject3d_->wtf.matWorld.m[3][0];
	worldPos.y = fbxObject3d_->wtf.matWorld.m[3][1];
	worldPos.z = fbxObject3d_->wtf.matWorld.m[3][2];

	return worldPos;
}
