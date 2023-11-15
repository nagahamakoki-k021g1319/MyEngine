#include "BossEnemy.h"
#include "Player.h"
#include <imgui.h>

BossEnemy::BossEnemy()
{
}

BossEnemy::~BossEnemy()
{
	delete spriteCommon;
	delete Obj_;
	delete Model_;
	delete Model2_;
	delete ModelAt_;

	delete collObj_;
	delete collModel_;

	delete  guidbulletObj_;
	delete	guidbulletModel_;

	for ( int i = 0; i < 5; i++ ){
		delete linkagebulletObj_[ i ];
		delete linkagebulletModel_[i];
	}
	
	delete collPlayerObj_;

	delete hpFlameUI;

	delete hpUI;
}

void BossEnemy::Initialize(DirectXCommon* dxCommon,Input* input)
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
	Model_ = Model::LoadFromOBJ("bossbik");
	Model2_ = Model::LoadFromOBJ("bossbik2");
	ModelAt_ = Model::LoadFromOBJ("bossbikAt");
	Obj_ = Object3d::Create();
	Obj_->SetModel(Model_);
	Obj_->wtf.scale = { 0.7f,0.7f,0.7f };
	Obj_->wtf.position = { 0.0f,-2.0f,-50.0f };

	//ボスの当たり判定のモデル
	collModel_ = Model::LoadFromOBJ("collboll");
	collObj_ = Object3d::Create();
	collObj_->SetModel(collModel_);
	collObj_->wtf.position = { Obj_->wtf.position.x,Obj_->wtf.position.y,Obj_->wtf.position.z - 1.0f };

	
	//誘導弾
	guidbulletModel_ = Model::LoadFromOBJ("collboll");
	guidbulletObj_ = Object3d::Create();
	guidbulletObj_->SetModel(guidbulletModel_);
	guidbulletObj_->wtf.scale = { 1.0f,1.0f,1.0f };
	guidbulletObj_->wtf.position = { Obj_->wtf.position.x,Obj_->wtf.position.y + 1.0f,Obj_->wtf.position.z };

	//5連誘導弾
	for ( int i = 0; i < 5; i++ ){
		linkagebulletModel_[i] = Model::LoadFromOBJ("collboll");
		linkagebulletObj_[i] = Object3d::Create();
		linkagebulletObj_[i]->SetModel(linkagebulletModel_[i]);
		linkagebulletObj_[i]->wtf.scale = { 1.0f,1.0f,1.0f };
		linkagebulletObj_[i]->wtf.position = {Obj_->wtf.position.x,Obj_->wtf.position.y,Obj_->wtf.position.z};
	}


	//自機の当たり判定
	collPlayerObj_ = Object3d::Create();
	collPlayerObj_->SetModel(collModel_);

	//パーティクル生成
	gasParticle = std::make_unique<ParticleManager>();
	gasParticle.get()->Initialize();
	gasParticle->LoadTexture("gas.png");
	gasParticle->Update();

	//UIの初期化(枚数が多いため)
	UIInitialize();
}

void BossEnemy::Update(Vector3 playerPos,Vector3 playerBpos)
{
	Obj_->Update();
	collObj_->Update();
	collObj_->wtf.position = { Obj_->wtf.position.x,Obj_->wtf.position.y + 0.5f,Obj_->wtf.position.z - 1.0f };
	guidbulletObj_->Update();
	for ( int i = 0; i < 5; i++ ){
		linkagebulletObj_[ i ]->Update();
	}
	collPlayerObj_->Update();
	collPlayerObj_->wtf.position = { playerPos};
	EffUpdate();
	isGameStartTimer++;

	bikSpinTimer++;
	//バイクの車輪が動き出す(抜刀)
	if ( bikSpinTimer > 10 ){bikSpinTimer = 0;}
	if ( bikSpinTimer >= 1 && bikSpinTimer <= 5 ){Obj_->SetModel(Model2_);}
	else if ( bikSpinTimer >= 6 && bikSpinTimer <= 10 ){Obj_->SetModel(Model_);}

	
	//ボス登場
	if ( player_->isCameraBehavior >= 2 ){
		isBesideFlag = 1;
		Obj_->wtf.position.z += 0.6f;
	}
	BossStartMovie();

	if ( input_->TriggerKey(DIK_4) )
	{
		hpPosition.x -= 10.0f;//倍ダメ
		hpUI->SetPozition(hpPosition);
	}

	//5連誘導弾
	inductionAttack();

	if ( isBesideFlag >= 4 ){
		SwAtTimer++;
	}
	//ボス登場からの攻撃
	if ( isBesideFlag == 6){
		//簡単な移動
		if (isMoveFlag_ == 0){
			Obj_->wtf.position.x -= 0.04f;
			if ( Obj_->wtf.position.x <= -10.0f ){
				Obj_->wtf.position.x = -10.0f;
				isMoveFlag_ = 1;
			}
		}
		else if ( isMoveFlag_ == 1 ){
			Obj_->wtf.position.x += 0.04f;
			if ( Obj_->wtf.position.x >= 10.0f ){
				Obj_->wtf.position.x = 10.0f;
				isMoveFlag_ = 0;
			}
		}

		//攻撃
		if ( SwAtTimer >= 100 && SwAtTimer <= 130 ){
			if (HP>=1){
				Obj_->SetModel(ModelAt_);
				bikSpinTimer = 6;
				isdurationShootFlag = 1;
			}
		}
		else{
			bikSpinTimer++;
		}

		if ( SwAtTimer >= 240 ){SwAtTimer = 0;}

	}

	if ( HP >= 1 ){
		if ( coll.CircleCollision(playerBpos,Obj_->wtf.position,1.0f,1.0f) ){
			HP--;
			player_->isClearFlag = true;
		}
	}


	ImGui::Begin("BossEnemy");

	ImGui::Text("isGameStartTimer:%d",isGameStartTimer);
	ImGui::Text("SwAtTimer:%d",SwAtTimer);
	ImGui::Text("issampleFlag:%d",isdurationShootFlag);
	ImGui::Text("linkageCoolTimer_:%d",linkageCoolTimer_);

	ImGui::End();

}

void BossEnemy::Draw()
{
	collPlayerObj_->Draw();
	if ( isGameStartTimer >= 200 ){
		if ( HP >= 1 ){
			Obj_->Draw();
			for ( int i = 0; i < 5; i++ ){
				if ( isdurationShootFlag == 1 ){linkagebulletObj_[ i ]->Draw();}
			}
		}

		
	}

}

void BossEnemy::UIInitialize()
{
	//HPフレーム
	hpFlameUI = new Sprite();
	hpFlameUI->Initialize(spriteCommon);
	hpFlameUI->SetPozition({ 0,0 });
	hpFlameUI->SetSize({ 1280.0f, 720.0f });

	//HPゲージ
	hpUI = new Sprite();
	hpUI->Initialize(spriteCommon);
	hpPosition = hpUI->GetPosition();
	hpUI->SetPozition(hpPosition);
	hpUI->SetSize({ 1280.0f, 720.0f });


	//画像読み込み
	//HPゲージ
	spriteCommon->LoadTexture(33,"bosshpFlame.png");
	hpFlameUI->SetTextureIndex(33);

	//HPの裏の黒い部分
	spriteCommon->LoadTexture(34,"bosshp.png");
	hpUI->SetTextureIndex(34);

}

void BossEnemy::UIDraw()
{
	//HP関連
	if ( isGameStartTimer >= 180 )
	{
		hpUI->Draw();
		hpFlameUI->Draw();
	}
}

void BossEnemy::EffUpdate()
{
	if ( isbulletEffFlag_ == 1 )
	{
		bulletEffTimer_++;
	}
	if ( bulletEffTimer_ <= 20 && bulletEffTimer_ >= 1 )
	{
		EffSummary(Vector3(Obj_->wtf.position.x - 0.3f,Obj_->wtf.position.y - 0.5f,Obj_->wtf.position.z + 0.5f));
	}
	if ( bulletEffTimer_ >= 20 )
	{
		isbulletEffFlag_ = 0;
		bulletEffTimer_ = 0;
	}
}

void BossEnemy::EffSummary(Vector3 bulletpos)
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

void BossEnemy::EffDraw()
{

}

Vector3 BossEnemy::GetWorldPosition()
{
	return Vector3();
}

void BossEnemy::inductionAttack()
{
	if ( isdurationShootFlag == 1 ){linkageCoolTimer_++;}

	//誘導弾の攻撃前の拡散
	if ( linkageCoolTimer_ >= 1 && linkageCoolTimer_ <= 30 ){
		//1発目の誘導弾(一番上)
		linkagebulletObj_[ 0 ]->wtf.position.y += 0.3f;
		if ( linkagebulletObj_[ 0 ]->wtf.position.y >= Obj_->wtf.position.y + 3.0f )
		{
			linkagebulletObj_[ 0 ]->wtf.position.y = Obj_->wtf.position.y + 3.0f;
		}
		//2発目の誘導弾(右上)
		linkagebulletObj_[ 1 ]->wtf.position.x += 0.3f;
		if ( linkagebulletObj_[ 1 ]->wtf.position.x >= Obj_->wtf.position.x + 3.0f )
		{
			linkagebulletObj_[ 1 ]->wtf.position.x = Obj_->wtf.position.x + 3.0f;
		}
		linkagebulletObj_[ 1 ]->wtf.position.y += 0.1f;
		if ( linkagebulletObj_[ 1 ]->wtf.position.y >= Obj_->wtf.position.y + 1.0f )
		{
			linkagebulletObj_[ 1 ]->wtf.position.y = Obj_->wtf.position.y + 1.0f;
		}
		//3発目の誘導弾(左上)
		linkagebulletObj_[ 2 ]->wtf.position.x -= 0.3f;
		if ( linkagebulletObj_[ 2 ]->wtf.position.x <= Obj_->wtf.position.x - 3.0f )
		{
			linkagebulletObj_[ 2 ]->wtf.position.x = Obj_->wtf.position.x - 3.0f;
		}
		linkagebulletObj_[ 2 ]->wtf.position.y += 0.1f;
		if ( linkagebulletObj_[ 2 ]->wtf.position.y >= Obj_->wtf.position.y + 1.0f )
		{
			linkagebulletObj_[ 2 ]->wtf.position.y = Obj_->wtf.position.y + 1.0f;
		}
		//4発目の誘導弾(右下)
		linkagebulletObj_[ 3 ]->wtf.position.x += 0.1f;
		if ( linkagebulletObj_[ 3 ]->wtf.position.x >= Obj_->wtf.position.x + 1.0f )
		{
			linkagebulletObj_[ 3 ]->wtf.position.x = Obj_->wtf.position.x + 1.0f;
		}
		linkagebulletObj_[ 3 ]->wtf.position.y -= 0.1f;
		if ( linkagebulletObj_[ 3 ]->wtf.position.y <= Obj_->wtf.position.y - 1.0f )
		{
			linkagebulletObj_[ 3 ]->wtf.position.y = Obj_->wtf.position.y - 1.0f;
		}
		//5発目の誘導弾(左下)
		linkagebulletObj_[ 4 ]->wtf.position.x -= 0.1f;
		if ( linkagebulletObj_[ 4 ]->wtf.position.x <= Obj_->wtf.position.x - 1.0f )
		{
			linkagebulletObj_[ 4 ]->wtf.position.x = Obj_->wtf.position.x - 1.0f;
		}
		linkagebulletObj_[ 4 ]->wtf.position.y -= 0.1f;
		if ( linkagebulletObj_[ 4 ]->wtf.position.y <= Obj_->wtf.position.y - 1.0f )
		{
			linkagebulletObj_[ 4 ]->wtf.position.y = Obj_->wtf.position.y - 1.0f;
		}

	}

	//5連誘導弾の自機ターゲット
	if ( linkageCoolTimer_ == 30 )
	{
		playerlen_[ 0 ] = collPlayerObj_->wtf.position - linkagebulletObj_[ 0 ]->wtf.position;
		playerlen_[ 0 ].nomalize();
		islinkageShootFlag_[ 0 ] = 1;
	}
	else if ( linkageCoolTimer_ == 50 )
	{
		playerlen_[ 1 ] = collPlayerObj_->wtf.position - linkagebulletObj_[ 1 ]->wtf.position;
		playerlen_[ 1 ].nomalize();
		islinkageShootFlag_[ 1 ] = 1;
	}
	else if ( linkageCoolTimer_ == 70 )
	{
		playerlen_[ 2 ] = collPlayerObj_->wtf.position - linkagebulletObj_[ 2 ]->wtf.position;
		playerlen_[ 2 ].nomalize();
		islinkageShootFlag_[ 2 ] = 1;
	}
	else if ( linkageCoolTimer_ == 90 )
	{
		playerlen_[ 3 ] = collPlayerObj_->wtf.position - linkagebulletObj_[ 3 ]->wtf.position;
		playerlen_[ 3 ].nomalize();
		islinkageShootFlag_[ 3 ] = 1;
	}
	else if ( linkageCoolTimer_ == 110 )
	{
		playerlen_[ 4 ] = collPlayerObj_->wtf.position - linkagebulletObj_[ 4 ]->wtf.position;
		playerlen_[ 4 ].nomalize();
		islinkageShootFlag_[ 4 ] = 1;
	}

	//5連誘導弾の発射
	for ( int i = 0; i < 4; i++ )
	{
		if ( islinkageShootFlag_[ i ] == 1 )
		{
			linkageBulletdurationTime_[ i ]++;

			linkagebulletObj_[ i ]->wtf.position += playerlen_[ i ];
			bitweenlen_[ i ] = playerlen_[ i ];
			bitweenlen_[ i ] *= 0.1f;
		}
		else if ( islinkageShootFlag_[ i ] == 0 && linkageCoolTimer_ == 0 )
		{
			linkagebulletObj_[ i ]->wtf.position = { Obj_->wtf.position.x,Obj_->wtf.position.y,Obj_->wtf.position.z };
		}
		if ( linkageBulletdurationTime_[ i ] >= 60.0f )
		{
			linkageBulletdurationTime_[ i ] = 0;
			islinkageShootFlag_[ i ] = 0;
		}
	}

	//5発目を打ち切ったら再セット
	if ( islinkageShootFlag_[ 4 ] == 1 )
	{
		linkageBulletdurationTime_[ 4 ]++;

		linkagebulletObj_[ 4 ]->wtf.position += playerlen_[ 4 ];
		bitweenlen_[ 4 ] = playerlen_[ 4 ];
		bitweenlen_[ 4 ] *= 0.1f;
	}
	else if ( islinkageShootFlag_[ 4 ] == 0 && linkageCoolTimer_ == 0 )
	{
		linkagebulletObj_[ 4 ]->wtf.position = { Obj_->wtf.position.x,Obj_->wtf.position.y,Obj_->wtf.position.z
		};
	}
	if ( linkageBulletdurationTime_[ 4 ] >= 60.0f )
	{
		linkageBulletdurationTime_[ 4 ] = 0;
		islinkageShootFlag_[ 4 ] = 0;
		isdurationShootFlag = 0;
		linkageCoolTimer_ = 0;
	}
}

void BossEnemy::BossStartMovie()
{
	if ( isBesideFlag == 1 )
	{
		Obj_->wtf.position.x += 0.4f;
		if ( Obj_->wtf.position.x >= 15.0f )
		{
			Obj_->wtf.position.x = 15.0f;
			isBesideFlag = 2;
		}
	}
	if ( isBesideFlag == 2 )
	{
		Obj_->wtf.rotation.z += 0.05f;
		if ( Obj_->wtf.rotation.z >= 1.6f )
		{
			Obj_->wtf.rotation.z = 1.6f;
		}
		Obj_->wtf.position.y += 0.1f;
		if ( Obj_->wtf.position.y >= 4.0f )
		{
			Obj_->wtf.position.y = 4.0f;
		}
	}
	if ( isBesideFlag == 3 )
	{
		Obj_->wtf.rotation.z -= 0.4f;
		if ( Obj_->wtf.rotation.z <= 0.0f )
		{
			Obj_->wtf.rotation.z = 0.0f;
		}
		Obj_->wtf.rotation.x -= 0.1f;
		if ( Obj_->wtf.rotation.x <= -0.5f )
		{
			Obj_->wtf.rotation.x = -0.5f;
			isBesideFlag = 4;
		}

	}
	if ( isBesideFlag == 4 )
	{
		Obj_->wtf.position.x = -0.05f;
		if ( Obj_->wtf.position.x <= 5.0f )
		{
			Obj_->wtf.position.x = 5.0f;
		}
		Obj_->wtf.position.y -= 0.2f;
		if ( Obj_->wtf.position.y <= -2.0f )
		{
			Obj_->wtf.position.y = -2.0f;
			isBesideFlag = 5;
		}

	}

	if ( isBesideFlag == 5 )
	{
		Obj_->wtf.rotation.x += 0.1f;
		if ( Obj_->wtf.rotation.x >= 0.0f )
		{
			Obj_->wtf.rotation.x = 0.0f;
			isBesideFlag = 6;
		}
	}

	if ( isBesideFlag == 2 && Obj_->wtf.position.z >= 25.0f )
	{
		Obj_->wtf.position.z = 25.0f;
		isBesideFlag = 3;
	}



}
