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
	delete bosshpflameUI;
	delete bosshpRedUI;
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

	Model_ = Model::LoadFromOBJ("BossColboll");
	Obj_ = Object3d::Create();
	Obj_->SetModel(Model_);
	Obj_->wtf.position = { 0.0f,-2.0f,189.0f };


	//パーティクル生成
	DamageParticle = std::make_unique<ParticleManager>();
	DamageParticle.get()->Initialize();
	DamageParticle->LoadTexture("bossclush.png");
	DamageParticle->Update();


	//ボスのHPフレーム
	bosshpflameUI = new Sprite();
	bosshpflameUI->Initialize(spriteCommon);
	bosshpflameUI->SetPozition({ 0,0 });
	bosshpflameUI->SetSize({ 1280.0f, 720.0f });

	//ボスのHPゲージ(下の赤い部分)
	bosshpRedUI = new Sprite();
	bosshpRedUI->Initialize(spriteCommon);
	bosshpRedUI->SetPozition({ 0,0 });
	bosshpRedUI->SetSize({ 1280.0f, 720.0f });

	//ボスのHPゲージ
	bosshpUI = new Sprite();
	bosshpUI->Initialize(spriteCommon);
	bosshpPosition = bosshpUI->GetPosition();
	bosshpUI->SetPozition(bosshpPosition);
	bosshpUI->SetSize({ 1280.0f, 720.0f });


	//ボスのHPフレーム
	spriteCommon->LoadTexture(29,"bosshpflame.png");
	bosshpflameUI->SetTextureIndex(29);

	//ボスのHPゲージ//ボスのHPゲージ(下の赤い部分)
	spriteCommon->LoadTexture(30,"bosshpred.png");
	bosshpRedUI->SetTextureIndex(30);

	//ボスのHPゲージ
	spriteCommon->LoadTexture(31,"bosshp.png");
	bosshpUI->SetTextureIndex(31);
}

void EnemyBoss::Update()
{
	if (input_->PushKey(DIK_4)){
		isCollFlag = true;
	}
	if ( player_->splinePosition_->GetIndex() >= 18 )
	{
		isCollFlag = true;
	}


	fbxObject3d_->Update();
	Obj_->Update();
	EffUpdate();


	if (isCollFlag == true)
	{
		if ( coll.CircleCollision(player_->GetBulletWorldPosition(),Obj_->wtf.position,1.0f,2.0f) )
		{
			bosshpPosition.x -= 5.0f;
			bosshpUI->SetPozition(bosshpPosition);
		};
	}
	if ( isCollFlag == true )
	{
		if ( coll.CircleCollision(player_->GetBulletStWorldPosition(),Obj_->wtf.position,1.0f,1.0f) )
		{
			bosshpPosition.x -= 50.0f;
			bosshpUI->SetPozition(bosshpPosition);
		};
	}

	if ( bosshpPosition.x <= -50 )
	{
		clushingTimer++;
	}

	if (clushingTimer>= 1 && clushingTimer <= 20)
	{
		isEffFlag_ = 1;
	}

}

void EnemyBoss::Draw()
{
	/*Obj_->Draw();*/
}

void EnemyBoss::FbxDraw()
{
	if (player_->splinePosition_->GetIndex() >= 18) {
		if ( clushingTimer <= 20 ){
			fbxObject3d_->Draw(dxCommon_->GetCommandList());
		}
	}
}

void EnemyBoss::UIDraw()
{
	if ( enemy_->winpArrivalTimer >= 1600 )
	{
		bosshpRedUI->Draw();
		bosshpUI->Draw();
		bosshpflameUI->Draw();
	}

}

void EnemyBoss::EffUpdate()
{
	if ( isEffFlag_ == 1 )
	{
		EffTimer_++;
	}
	if ( EffTimer_ <= 20 && EffTimer_ >= 1 )
	{
		EffSummary(Vector3(fbxObject3d_->wtf.position.x,fbxObject3d_->wtf.position.y,fbxObject3d_->wtf.position.z));
	}
	if ( EffTimer_ >= 20 )
	{
		isEffFlag_ = 0;
		EffTimer_ = 0;
	}
}

void EnemyBoss::EffSummary(Vector3 Bosspos)
{
	//パーティクル範囲
	for ( int i = 0; i < 5; i++ )
	{
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_pos = 5.0f;
		Vector3 pos{};
		pos.x += ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y += ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z += ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos += Bosspos;
		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		const float rnd_vel = 0.1f;
		Vector3 vel{};
		vel.x = ( float ) rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = ( float ) rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = ( float ) rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		const float rnd_acc = 0.00001f;
		Vector3 acc{};
		acc.x = ( float ) rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;
		acc.y = ( float ) rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;

		//追加
		DamageParticle->Add(60,pos,vel,acc,1.0f,0.0f);

		DamageParticle->Update();

	}

}

void EnemyBoss::EffDraw()
{
	if ( isEffFlag_ == 1 )
	{
		// 3Dオブクジェクトの描画
		DamageParticle->Draw();
	}
}

Vector3 EnemyBoss::GetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;


	Obj_->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	worldPos.x = Obj_->wtf.matWorld.m[3][0];
	worldPos.y = Obj_->wtf.matWorld.m[3][1];
	worldPos.z = Obj_->wtf.matWorld.m[3][2];

	return worldPos;
}
