#include "ArmorEnemy.h"
#include <imgui.h>
#include "Player.h"

ArmorEnemy::ArmorEnemy()
{
}

ArmorEnemy::~ArmorEnemy()
{
	delete spriteCommon;
	delete Obj_;
	delete Model_;
	delete Modelst_;

	delete bulletObj_;
	delete bulletModel_;

}

void ArmorEnemy::Initialize(DirectXCommon* dxCommon,Input* input)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);

	this->dxCommon_ = dxCommon;
	input_ = input;
	//スプライト共通部分の初期化
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon);

	//雑魚敵(攻撃状態)
	Model_ = Model::LoadFromOBJ("armorenemy");
	//雑魚敵(待機状態)
	Modelst_ = Model::LoadFromOBJ("armorenemyst");
	Obj_ = Object3d::Create();
	Obj_->SetModel(Modelst_);
	Obj_->wtf.scale = { 0.4f,0.4f,0.4f };
	Obj_->wtf.position = { 3.0f,-2.0f,25.0f };
	//ポリゴン爆散の情報                 大きさ 回転  飛ぶ量 
	Obj_->SetPolygonExplosion({ 0.0f,-1.0f,6.28f,20.0f });

	//大砲の弾
	bulletModel_ = Model::LoadFromOBJ("eneboll");
	bulletObj_ = Object3d::Create();
	bulletObj_->SetModel(bulletModel_);
	bulletObj_->wtf.position = { 3.0f,0.5f,23.0f };

	//パーティクル生成
	gasParticle = std::make_unique<ParticleManager>();
	gasParticle.get()->Initialize();
	gasParticle->LoadTexture("gas.png");
	gasParticle->Update();

	gasParticle2 = std::make_unique<ParticleManager>();
	gasParticle2.get()->Initialize();
	gasParticle2->LoadTexture("gas.png");
	gasParticle2->Update();

}

void ArmorEnemy::Update(Vector3 playerPos)
{
	Obj_->Update();
	bulletObj_->Update();
	EffUpdate();
	isGameStartTimer++;
	isbulletEffFlag_ = 1;

	if ( isGameStartTimer >= 200 && isShootFlag == false )
	{
		BulletCoolTime++;
	}
	if ( BulletCoolTime == 59 )
	{
		playerlen = playerPos - bulletObj_->wtf.position;
		playerlen.nomalize();
	}
	//攻撃時モデルが変わる
	if ( BulletCoolTime >= 60 )
	{
		BulletCoolTime = 60;
		Obj_->SetModel(Model_);
		isShootFlag = true;
	}
	else
	{
		Obj_->SetModel(Modelst_);
	}

	if ( isShootFlag == true )
	{
		BulletdurationTime++;

		bulletObj_->wtf.position += playerlen;
		bitweenlen = playerlen;
		bitweenlen *= 0.1f;
	}
	else
	{
		bulletObj_->wtf.position = { 3.0f,0.5f,23.0f };
	}
	if ( BulletdurationTime >= 40.0f )
	{
		BulletdurationTime = 0;
		isShootFlag = false;
		BulletCoolTime = 0;
	}

	

//ポリゴン爆散
	if ( input_->TriggerKey(DIK_5) )
	{
		isExpolFlag = true;
	}
	if ( isExpolFlag == true )
	{
		ExpolTimer++;

		float polygon = ExpolTimer / ExpolMT;

		Obj_->SetDestruction(polygon);
		if ( ExpolTimer >= ExpolMT )
		{
			isAliveFlag = false;
		}
	}

	ImGui::Begin("ArmorEnemy");

	ImGui::Text("isGameStartTimer:%d",isGameStartTimer);
	

	ImGui::End();

}

void ArmorEnemy::Draw()
{
	if ( isGameStartTimer >= 200 )
	{
		if ( isAliveFlag == true )
		{
			Obj_->Draw();
			bulletObj_->Draw();
		}
	}
}

void ArmorEnemy::EffUpdate()
{
	if ( isbulletEffFlag_ == 1 )
	{
		bulletEffTimer_++;
	}
	if ( bulletEffTimer_ <= 20 && bulletEffTimer_ >= 1 )
	{
		EffSummary(Vector3(Obj_->wtf.position.x + 1.0f,Obj_->wtf.position.y - 1.5f,Obj_->wtf.position.z));
		EffSummary2(Vector3(Obj_->wtf.position.x - 1.0f,Obj_->wtf.position.y - 1.5f,Obj_->wtf.position.z));
	}
	if ( bulletEffTimer_ >= 20 )
	{
		isbulletEffFlag_ = 0;
		bulletEffTimer_ = 0;
	}
}

void ArmorEnemy::EffSummary(Vector3 bulletpos)
{
	//パーティクル範囲
	for ( int i = 0; i < 5; i++ )
	{
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_posG = 0.0f;
		const float rnd_posGy = 0.0f;
		const float rnd_posGz = 0.0f;
		Vector3 posG{};
		posG.x += ( float ) rand() / RAND_MAX * rnd_posG - rnd_posG / 2.0f;
		posG.y += ( float ) rand() / RAND_MAX * rnd_posGy - rnd_posGy / 2.0f;
		posG.z += ( float ) rand() / RAND_MAX * rnd_posGz - rnd_posGz / 2.0f;
		posG += bulletpos;
		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		const float rnd_velG = 0.0f;
		const float rnd_velGy = 0.0f;
		const float rnd_velGz = 0.5f;
		Vector3 velG{};
		velG.x = ( float ) rand() / RAND_MAX * rnd_velG - rnd_velG / 2.0f;
		velG.y = ( float ) rand() / RAND_MAX * rnd_velGy - rnd_velGy / 2.0f;
		velG.z = ( float ) rand() / RAND_MAX * rnd_velGz - rnd_velGz / 1.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		const float rnd_accG = 0.000001f;
		Vector3 accG{};
		accG.x = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;
		accG.y = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;

		//追加
		gasParticle->Add(60,posG,velG,accG,0.5f,0.0f);

		gasParticle->Update();

	}
}

void ArmorEnemy::EffSummary2(Vector3 bulletpos2)
{
	//パーティクル範囲
	for ( int i = 0; i < 5; i++ )
	{
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_pos2 = 0.0f;
		const float rnd_posy2 = 0.0f;
		const float rnd_posz2 = 0.0f;
		Vector3 pos2{};
		pos2.x += ( float ) rand() / RAND_MAX * rnd_pos2 - rnd_pos2 / 2.0f;
		pos2.y += ( float ) rand() / RAND_MAX * rnd_posy2 - rnd_posy2 / 2.0f;
		pos2.z += ( float ) rand() / RAND_MAX * rnd_posz2 - rnd_posz2 / 2.0f;
		pos2 += bulletpos2;
		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		const float rnd_vel2 = 0.0f;
		const float rnd_vely2 = 0.0f;
		const float rnd_velz2 = 0.5f;
		Vector3 vel2{};
		vel2.x = ( float ) rand() / RAND_MAX * rnd_vel2 - rnd_vel2 / 2.0f;
		vel2.y = ( float ) rand() / RAND_MAX * rnd_vely2 - rnd_vely2 / 2.0f;
		vel2.z = ( float ) rand() / RAND_MAX * rnd_velz2 - rnd_velz2 / 1.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		const float rnd_acc2 = 0.000001f;
		Vector3 acc2{};
		acc2.x = ( float ) rand() / RAND_MAX * rnd_acc2 - rnd_acc2 / 2.0f;
		acc2.y = ( float ) rand() / RAND_MAX * rnd_acc2 - rnd_acc2 / 2.0f;

		//追加
		gasParticle2->Add(60,pos2,vel2,acc2,0.5f,0.0f);

		gasParticle2->Update();

	}

}

void ArmorEnemy::EffDraw()
{
	if ( isbulletEffFlag_ == 1 )
	{
		gasParticle->Draw();
		gasParticle2->Draw();
	}

}

Vector3 ArmorEnemy::GetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;

	bulletObj_->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	worldPos.x = bulletObj_->wtf.matWorld.m[ 3 ][ 0 ];
	worldPos.y = bulletObj_->wtf.matWorld.m[ 3 ][ 1 ];
	worldPos.z = bulletObj_->wtf.matWorld.m[ 3 ][ 2 ];

	return worldPos;
}

Vector3 ArmorEnemy::GetWorldBulletPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldbulletPos;

	bulletObj_->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	worldbulletPos.x = bulletObj_->wtf.matWorld.m[ 3 ][ 0 ];
	worldbulletPos.y = bulletObj_->wtf.matWorld.m[ 3 ][ 1 ];
	worldbulletPos.z = bulletObj_->wtf.matWorld.m[ 3 ][ 2 ];

	return worldbulletPos;
}


