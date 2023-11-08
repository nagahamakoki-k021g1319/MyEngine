#include "BikeEnemy.h"
#include "Player.h"
#include <imgui.h>

BikeEnemy::BikeEnemy()
{
}

BikeEnemy::~BikeEnemy()
{
	delete spriteCommon;
	for ( int i = 0; i < 2; i++ )
	{
		delete Obj_[i];
		delete Model_[ i ];
	}
	delete collObj_;
	delete collModel_;
	delete bikclushObj_;
	delete bikclushModel_;

	delete blowenemyObj_;
	delete blowenemyModel_;
}

void BikeEnemy::Initialize(DirectXCommon* dxCommon,Input* input)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);

	this->dxCommon_ = dxCommon;
	input_ = input;
	//スプライト共通部分の初期化
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon);

	for ( int i = 0; i < 2; i++ )
	{
		//雑魚敵(攻撃状態)
		Model_[ i ] = Model::LoadFromOBJ("bikenemy");
		Obj_[ i ] = Object3d::Create();
		Obj_[ i ]->SetModel(Model_[ i ]);
		Obj_[ i ]->wtf.scale = { 0.4f,0.4f,0.4f };
		
	}
	Obj_[0]->wtf.position = { -3.0f,-2.0f,-20.0f };
	Obj_[1]->wtf.position = { 1.0f,-2.0f,0.0f };

	
		//当たり判定のモデル
		collModel_ = Model::LoadFromOBJ("collboll");
		collObj_ = Object3d::Create();
		collObj_->SetModel(collModel_);
		collObj_->wtf.position = { Obj_[0]->wtf.position.x,Obj_[0]->wtf.position.y,Obj_[0]->wtf.position.z - 1.0f};
	

	//雑魚敵(死んだときのバイクスピン)
	bikclushModel_ = Model::LoadFromOBJ("bikclush");
	bikclushObj_ = Object3d::Create();
	bikclushObj_->SetModel(bikclushModel_);
	bikclushObj_->wtf.scale = { 0.4f,0.4f,0.4f };
	bikclushObj_->wtf.position = { Obj_[0]->wtf.position.x,Obj_[0]->wtf.position.y,Obj_[0]->wtf.position.z};

	//パーティクル生成
	gasParticle = std::make_unique<ParticleManager>();
	gasParticle.get()->Initialize();
	gasParticle->LoadTexture("gas.png");
	gasParticle->Update();

}

void BikeEnemy::Update(Vector3 playerSWPos,bool isCollSWFlag)
{
	for ( int i = 0; i < 2; i++ )
	{
		Obj_[i]->Update();
		
	}
	collObj_->Update();
	collObj_->wtf.position = { Obj_[0]->wtf.position.x,Obj_[0]->wtf.position.y + 0.5f,Obj_[0]->wtf.position.z - 1.0f };
	bikclushObj_->Update();
	bikclushObj_->wtf.position = { Obj_[0]->wtf.position.x,Obj_[0]->wtf.position.y,Obj_[0]->wtf.position.z};

	
	EffUpdate();
	isGameStartTimer++;

	
		if (isBackEntryFlag == 0 ) {
			if ( isGameStartTimer >= 220 )
			{
				Obj_[0]->wtf.position.z += 0.5f;
			}
			if ( Obj_[0]->wtf.position.z >= 0.0f )
			{
				Obj_[0]->wtf.position.z = 0.0f;
				isBackEntryFlag= 1;
			}
		}
	

	
		if ( HP >= 1 )
		{
			if ( isCollSWFlag == true )
			{
				if ( coll.CircleCollision(playerSWPos,collObj_->wtf.position,0.6f,0.6f) )
				{
					HP--;
				}
			}
		}
	
	//バイクスピン
	
		if ( HP <= 0 )
		{
			isBikclushFlag = true;
			isbulletEffFlag_ = 0;
			bulletEffTimer_ = 0;
		}
		else
		{
			isbulletEffFlag_ = 1;
		}
	


	//バイクが前進しながら倒れる
	if ( isBackEntryFlag == 1 ) {
		if ( isBikclushFlag == true ){
			Obj_[ 0 ]->wtf.position.z += 0.3f;
			bikclushObj_->wtf.position.z += 0.3f;
			bikclushObj_->wtf.rotation.z += 0.1f;
		}
		if ( bikclushObj_->wtf.rotation.z >= 1.5f ){
			bikclushObj_->wtf.rotation.z = 1.5f;
			isBikSpinFlag = true;
		}
		//バイクが倒れて後ろに行く
		if ( isBikSpinFlag == true ){
			Obj_[ 0 ]->wtf.position.z -= 0.6f;
			bikclushObj_->wtf.position.z -= 0.6f;
			bikclushObj_->wtf.rotation.x -= 0.1f;
		}
	}

	ImGui::Begin("bikeEnemy");

	ImGui::Text("isBackEntryFlag:%d",isBackEntryFlag);

	ImGui::End();

}

void BikeEnemy::Draw()
{
	if ( isGameStartTimer >= 200 ){
		for ( int i = 0; i < 2; i++ )
		{
			if ( isBikclushFlag == false )
			{
				Obj_[i]->Draw();
				/*collObj_->Draw();*/
			}
		}

		if ( isBikclushFlag == true ){
			if ( isBackEntryFlag == 1 ){
				if ( bikclushObj_->wtf.position.z >= -20.0f ){
					bikclushObj_->Draw();
				}
			}
		}
	}

}

void BikeEnemy::EffUpdate()
{
	if ( isbulletEffFlag_ == 1 )
	{
		bulletEffTimer_++;
	}
	if ( bulletEffTimer_ <= 20 && bulletEffTimer_ >= 1 )
	{
		EffSummary(Vector3(Obj_[0]->wtf.position.x - 0.3f,Obj_[0]->wtf.position.y - 0.5f,Obj_[0]->wtf.position.z + 0.5f));
	}
	if ( bulletEffTimer_ >= 20 )
	{
		isbulletEffFlag_ = 0;
		bulletEffTimer_ = 0;
	}
}

void BikeEnemy::EffSummary(Vector3 bulletpos)
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
		velG.z = ( float ) rand() / RAND_MAX * rnd_velGz - rnd_velGz / 0.5f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		const float rnd_accG = 0.000001f;
		Vector3 accG{};
		accG.x = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;
		accG.y = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;

		//追加
		gasParticle->Add(60,posG,velG,accG,0.1f,0.0f);

		gasParticle->Update();

	}
}

void BikeEnemy::EffDraw()
{
	if ( isGameStartTimer >= 200 ){
		if ( isBikclushFlag == false ){
			if ( isbulletEffFlag_ == 1 ){
				gasParticle->Draw();
			}
		}
	}
}

Vector3 BikeEnemy::GetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;

	collObj_->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	worldPos.x = collObj_->wtf.matWorld.m[ 3 ][ 0 ];
	worldPos.y = collObj_->wtf.matWorld.m[ 3 ][ 1 ];
	worldPos.z = collObj_->wtf.matWorld.m[ 3 ][ 2 ];

	return worldPos;
}
