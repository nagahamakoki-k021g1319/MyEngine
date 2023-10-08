#include "EnemyBoss.h"
#include "Enemy.h"
#include "Player.h"


EnemyBoss::EnemyBoss()
{
}

EnemyBoss::~EnemyBoss()
{
	delete spriteCommon;
	delete fbxModel_;
	delete fbxObject3d_;
	delete Model_;
	delete Obj_;
	delete bosshpUI;

}

void EnemyBoss::Initialize(DirectXCommon* dxCommon, Input* input)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);

	this->dxCommon_ = dxCommon;
	input_ = input;

	//スプライト共通部分の初期化
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon);

	fbxModel_ = FbxLoader::GetInstance()->LoadModelFromFile("kuma");

	fbxObject3d_ = new FBXObject3d;
	fbxObject3d_->Initialize();
	fbxObject3d_->SetModel(fbxModel_);
	fbxObject3d_->wtf.position = { 0.0f,-5.0f,200.0f };
	fbxObject3d_->wtf.rotation = { 0.0f,2.7f,0.0f };
	fbxObject3d_->PlayAnimation(1.0f, true);

	Model_ = Model::LoadFromOBJ("boll");
	Obj_ = Object3d::Create();
	Obj_->SetModel(Model_);
	Obj_->wtf.position = { 0.0f,0.0f,9.0f };

	//ボスのHPゲージ
	bosshpUI = new Sprite();
	bosshpUI->Initialize(spriteCommon);
	bosshpPosition = bosshpUI->GetPosition();
	bosshpUI->SetPozition(bosshpPosition);
	bosshpUI->SetSize({ 1280.0f, 720.0f });

	//ボスのHPゲージ
	spriteCommon->LoadTexture(31,"bosshp.png");
	bosshpUI->SetTextureIndex(31);
}

void EnemyBoss::Update()
{
	if (input_->PushKey(DIK_4)){
		bosshpPosition.x -= 2.5f;
		bosshpUI->SetPozition(bosshpPosition);
	}

	fbxObject3d_->Update();
	Obj_->Update();
	if (enemy_->bossGostAt == true) {

	}
}

void EnemyBoss::Draw()
{
	Obj_->Draw();
}

void EnemyBoss::FbxDraw()
{
	if (player_->splinePosition_->GetIndex() >= 18) {
		fbxObject3d_->Draw(dxCommon_->GetCommandList());
	}
}

void EnemyBoss::UIDraw()
{
	bosshpUI->Draw();
}

Vector3 EnemyBoss::GetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;


	fbxObject3d_->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	worldPos.x = fbxObject3d_->wtf.matWorld.m[3][0];
	worldPos.y = fbxObject3d_->wtf.matWorld.m[3][1];
	worldPos.z = fbxObject3d_->wtf.matWorld.m[3][2];

	return worldPos;
}
