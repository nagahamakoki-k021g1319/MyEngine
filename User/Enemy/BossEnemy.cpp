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

	for ( int i = 0; i < 8; i++ ){delete swObj_[i];}
	delete swModel_;

	delete collswObj_;
	delete collswModel_;

	delete  guidbulletObj_;
	delete	guidbulletModel_;

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

	//当たり判定のモデル
	collModel_ = Model::LoadFromOBJ("collboll");
	collObj_ = Object3d::Create();
	collObj_->SetModel(collModel_);
	collObj_->wtf.position = { Obj_->wtf.position.x,Obj_->wtf.position.y,Obj_->wtf.position.z - 1.0f };

	//攻撃時のモデル
	swModel_ = Model::LoadFromOBJ("enesw");
	for ( int i = 0; i < 8; i++ )
	{
		swObj_[i] = Object3d::Create();
		swObj_[i]->SetModel(swModel_);
		swObj_[i]->wtf.scale = { 0.7f,0.7f,0.7f };
		
	}

	swObj_[0]->wtf.position = { 5.0f,-16.0f, 20.0f };//-7.3
	swObj_[1]->wtf.position = { 5.0f,-16.0f, 17.0f };
	swObj_[2]->wtf.position = { 5.0f,-16.0f, 14.0f };
	swObj_[3]->wtf.position = { 5.0f,-16.0f, 11.0f };
	swObj_[4]->wtf.position = { 5.0f,-16.0f, 8.7f };
	swObj_[5]->wtf.position = { 5.0f,-16.0f, 5.0f };//-7.3
	swObj_[6]->wtf.position = { 5.0f,-16.0f, 2.0f };
	swObj_[7]->wtf.position = { 5.0f,-16.0f, -0.7f};

	//弾の当たり判定のモデル
	collswModel_ = Model::LoadFromOBJ("collboll");
	collswObj_ = Object3d::Create();
	collswObj_->SetModel(collswModel_);
	collswObj_->wtf.position = { 5.0f,-2.0f,25.0f };

	guidbulletModel_ = Model::LoadFromOBJ("esp");
	guidbulletObj_ = Object3d::Create();
	guidbulletObj_->SetModel(guidbulletModel_);
	guidbulletObj_->wtf.scale = { 0.7f,0.7f,0.7f };
	guidbulletObj_->wtf.position = { 0.0f,-6.0f,5.0f };


	//パーティクル生成
	gasParticle = std::make_unique<ParticleManager>();
	gasParticle.get()->Initialize();
	gasParticle->LoadTexture("gas.png");
	gasParticle->Update();

}

void BossEnemy::Update(Vector3 playerBpos)
{
	Obj_->Update();
	collObj_->Update();
	collObj_->wtf.position = { Obj_->wtf.position.x,Obj_->wtf.position.y + 0.5f,Obj_->wtf.position.z - 1.0f };
	for ( int i = 0; i < 8; i++ ){swObj_[i]->Update();}
	collswObj_->Update();
	guidbulletObj_->Update();
	EffUpdate();
	isGameStartTimer++;

	bikSpinTimer++;
	//バイクの車輪が動き出す(抜刀)
	if ( bikSpinTimer > 10 ){bikSpinTimer = 0;}
	if ( bikSpinTimer >= 1 && bikSpinTimer <= 5 ){Obj_->SetModel(Model2_);}
	else if ( bikSpinTimer >= 6 && bikSpinTimer <= 10 ){Obj_->SetModel(Model_);}

	if ( input_->PushKey(DIK_T) ){
		
	}

	

	//地面から剣
	if( isSWFlag == true ){SWTimer++;}
	for ( int i = 0; i < 8; i++ ){
		if ( SWTimer >= 8 + 10 * i){
			swObj_[i]->wtf.position.y += 1.0f;
		}
	}
	for ( int i = 0; i < 8; i++ ){
		if ( swObj_[ i ]->wtf.position.y >= -7.3f ){
			swObj_[ i ]->wtf.position.y = -7.3f;
		}
	}

	//剣リセット
	if ( SWTimer >= 150 )
	{
		SWTimer = 0;
		swObj_[ 0 ]->wtf.position = { Obj_->wtf.position.x,-16.0f, 20.0f };//-7.3
		swObj_[ 1 ]->wtf.position = { Obj_->wtf.position.x,-16.0f, 17.0f };
		swObj_[ 2 ]->wtf.position = { Obj_->wtf.position.x,-16.0f, 14.0f };
		swObj_[ 3 ]->wtf.position = { Obj_->wtf.position.x,-16.0f, 11.0f };
		swObj_[ 4 ]->wtf.position = { Obj_->wtf.position.x,-16.0f, 8.7f  };
		swObj_[ 5 ]->wtf.position = { Obj_->wtf.position.x,-16.0f, 5.0f  };//-7.3
		swObj_[ 6 ]->wtf.position = { Obj_->wtf.position.x,-16.0f, 2.0f };
		swObj_[ 7 ]->wtf.position = { Obj_->wtf.position.x,-16.0f, -0.7f};
		isSWFlag = false;
	}


	//当たり判定用の弾
	if ( isshotFlag == true){
		collswObj_->wtf.position.z -= 0.22f;
	}
	//弾リセット
	if ( collswObj_->wtf.position.z <= -10 ){
		collswObj_->wtf.position = { Obj_->wtf.position.x,-2.0f,25.0f };
		isshotFlag = false;
	}

	//ボス登場
	if ( player_->isCameraBehavior >= 2 ){
		isBesideFlag = 1;
		Obj_->wtf.position.z += 0.6f;
	}
	BossStartMovie();

	if ( isBesideFlag >= 4 ){
		SwAtTimer++;
	}
	//ボス登場からの攻撃
	if ( isBesideFlag == 6){

		Obj_->wtf.position.x -= 0.01f;
		if ( Obj_->wtf.position.x <= 0.0f)
		{
			Obj_->wtf.position.x = 0.0f;
		}

		if ( SwAtTimer >= 160 && SwAtTimer <= 190 ){
			if (HP>=1)
			{
				Obj_->SetModel(ModelAt_);
				isSWFlag = true;
				isshotFlag = true;

				bikSpinTimer = 6;
				Obj_->wtf.rotation.x = -0.7f;
				Obj_->wtf.rotation.y = -0.8f;
				Obj_->wtf.position.y = -1.0f;
			}
		}
		else{
			bikSpinTimer++;
			Obj_->wtf.position.y = -2.0f;
			Obj_->wtf.rotation.x = 0.0f;
			Obj_->wtf.rotation.y = 0.0f;
		}

		if ( SwAtTimer >= 240 )
		{
			SwAtTimer = 0;
		}
	}

	if ( HP >= 1 ){
		if ( coll.CircleCollision(playerBpos,Obj_->wtf.position,1.0f,1.0f) ){
			HP--;
			player_->isClearFlag = true;
		}
	}


	ImGui::Begin("BossEnemy");

	ImGui::Text("isGameStartTimer:%d",isGameStartTimer);
	ImGui::Text("position:%f,%f,%f",collswObj_->wtf.position.x,collswObj_->wtf.position.y,collswObj_->wtf.position.z);
	ImGui::Text("rotation:%f,%f,%f",swObj_[0]->wtf.rotation.x,swObj_[0]->wtf.rotation.y,swObj_[0]->wtf.rotation.z);
	ImGui::Text("SwAtTimer:%d",SwAtTimer);


	ImGui::End();

}

void BossEnemy::Draw()
{
	if ( isGameStartTimer >= 200 ){
		if ( HP >= 1 ){
			Obj_->Draw();
			/*guidbulletObj_->Draw();*/
			for ( int i = 0; i < 8; i++ )
			{
				swObj_[ i ]->Draw();
			}
			/*collObj_->Draw();
			collswObj_->Draw();*/
		}
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
