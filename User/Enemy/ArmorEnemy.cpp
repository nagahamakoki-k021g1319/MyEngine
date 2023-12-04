#include "ArmorEnemy.h"
#include <imgui.h>
#include "Player.h"

ArmorEnemy::ArmorEnemy()
{
}

ArmorEnemy::~ArmorEnemy()
{
	delete spriteCommon;
	for ( int i = 0; i < 4; i++ )
	{
		delete Obj_[i];
		delete Model_[ i ];
		delete Modelst_[ i ];
		delete collObj_[i];
		delete collModel_[i];
		delete bulletObj_[i];
		delete bulletModel_[i];
	}	
	delete collObj3_;
	
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


	for ( int i = 0; i < 4; i++ ){
	//雑魚敵(攻撃状態)
		Model_[i] = Model::LoadFromOBJ("armorenemy");
		//雑魚敵(待機状態)
		Modelst_[i] = Model::LoadFromOBJ("armorenemyst");

		Obj_[i] = Object3d::Create();
		Obj_[i]->SetModel(Modelst_[i]);
		Obj_[i]->wtf.scale = { 0.4f,0.4f,0.4f };
		
		//ポリゴン爆散の情報                 大きさ 回転  飛ぶ量 
		Obj_[i]->SetPolygonExplosion({ 0.0f,-1.0f,6.28f,20.0f });
	}
	Obj_[ 0 ]->wtf.position = { 10.0f,-2.0f,-10.0f };
	Obj_[ 1 ]->wtf.position = {-10.0f,-2.0f,-10.0f };
	Obj_[ 2 ]->wtf.position = { 10.0f,-2.0f,-20.0f };
	Obj_[ 3 ]->wtf.position = { -10.0f,-2.0f,-20.0f };

	//ここでHP設定
	HP_[0] = 12;
	HP_[1] = 12;
	HP_[2] = 12;
	HP_[3] = 12;

	//大砲の弾
	for ( int i = 0; i < 4; i++ ){
		bulletModel_[i] = Model::LoadFromOBJ("eneboll");
		bulletObj_[i] = Object3d::Create();
		bulletObj_[i]->SetModel(bulletModel_[i]);
		bulletObj_[i]->wtf.position = { Obj_[i]->wtf.position.x,Obj_[i]->wtf.position.y + 2.5f,Obj_[i]->wtf.position.z - 3.0f };
	}

	

	//当たり判定のモデル
	for ( int i = 0; i < 4; i++ ){
		collModel_[i] = Model::LoadFromOBJ("collboll");
		collObj_[i] = Object3d::Create();
		collObj_[i]->SetModel(collModel_[i]);
		collObj_[i]->wtf.scale = { 2.0f,2.0f,2.0f };
		collObj_[i]->wtf.position = { Obj_[i]->wtf.position.x,Obj_[i]->wtf.position.y + 1.0f,Obj_[i]->wtf.position.z };
	}

	//自機の当たり判定
	collObj3_ = Object3d::Create();
	collObj3_->SetModel(collModel_[0]);

	//パーティクル生成
	for ( int i = 0; i < 4; i++ ){
		//エフェクトの情報(地面のズサ)
		gasParticle_[i] = std::make_unique<ParticleManager>();
		gasParticle_[i].get()->Initialize();
		gasParticle_[i]->LoadTexture("gas.png");
		gasParticle_[i]->Update();

		gasParticle2_[i] = std::make_unique<ParticleManager>();
		gasParticle2_[i].get()->Initialize();
		gasParticle2_[i]->LoadTexture("gas.png");
		gasParticle2_[i]->Update();

		//エフェクトの情報(背中の噴射ガス)
		gasParticle3_[i] = std::make_unique<ParticleManager>();
		gasParticle3_[i].get()->Initialize();
		gasParticle3_[i]->LoadTexture("fire.png");
		gasParticle3_[i]->Update();

		gasParticle4_[i] = std::make_unique<ParticleManager>();
		gasParticle4_[i].get()->Initialize();
		gasParticle4_[i]->LoadTexture("fire.png");
		gasParticle4_[i]->Update();

		//攻撃受けた時の火花のパーティクル
		DamageParticle_[i] = std::make_unique<ParticleManager>();
		DamageParticle_[i].get()->Initialize();
		DamageParticle_[i]->LoadTexture("fire.png");
		DamageParticle_[i]->Update();
	}
}

void ArmorEnemy::Update(Vector3 playerPos,Vector3 playerBpos,bool playerShootFlag)
{
	for ( int i = 0; i < 4; i++ ){
		if ( isAliveFlag_[ i ] == 0 ){
			Obj_[ i ]->Update();
			collObj_[i]->Update();
			collObj_[i]->wtf.position = {Obj_[i]->wtf.position.x,Obj_[i]->wtf.position.y + 1.0f,Obj_[i]->wtf.position.z};
			bulletObj_[i]->Update();
		}
	}
	collObj3_->Update();
	collObj3_->wtf.position = { playerPos };
	EffUpdate();
	isGameStartTimer++;
	

	//魔導兵が後ろから登場(ラウンド1)
	for ( int i = 0; i < 2; i++ ){
		if ( isAliveFlag_[ i ] == 0 ){
			if ( isMoveFlag_[ i ] == 0 )
			{
				if ( isGameStartTimer >= 200 )
				{
					Obj_[ i ]->wtf.position.z += 0.7f;
				}
				if ( Obj_[ i ]->wtf.position.z <= 20.0f && Obj_[ i ]->wtf.position.z >= 15.0f )
				{
					BulletCoolTime_[ i ] = -50 + i * -20;
				}
				if ( Obj_[ i ]->wtf.position.z >= 35.0f )
				{
					Obj_[ i ]->wtf.position.z = 35.0f;
					isMoveFlag_[ i ] = 1;
				}
			}
		}
	}

	//魔導兵が後ろから登場(ラウンド2)
	if ( player_->isRoundFlag == 0 )
	{
		BulletCoolTime_[ 2 ] = 0;
	}
	if ( player_->isRoundFlag == 1 ){
		//ここで描画させるタイマーの起動
		AliveR2Timer++;
		if( AliveR2Timer >= 1 && AliveR2Timer <= 2){
			isAliveFlag_[ 2 ] = 0;
		}

		//後ろから登場して左右移動
		if ( isAliveFlag_[2] == 0 ){
			if ( isMoveFlag_[ 2 ] == 0 ){
				if ( isGameStartTimer >= 200 ){
					Obj_[ 2 ]->wtf.position.z += 1.0f;
				}
				if ( Obj_[ 2 ]->wtf.position.z <= 20.0f && Obj_[ 2 ]->wtf.position.z >= 15.0f && player_->isRoundFlag == 1 )
				{
					BulletCoolTime_[ 2 ] = -50;
				}
				if ( Obj_[ 2 ]->wtf.position.z >= 35.0f )
				{
					Obj_[ 2 ]->wtf.position.z = 35.0f;
					isMoveFlag_[ 2 ] = 1;
				}
			}
		}
	}

	//魔導兵が後ろから登場(ラウンド3)
	if ( player_->isRoundFlag <= 2 )
	{
		BulletCoolTime_[ 3 ] = 0;
	}
	if ( player_->isRoundFlag == 3 ){
		//ここで描画させるタイマーの起動
		AliveR3Timer++;
		if ( AliveR3Timer >= 1 && AliveR3Timer <= 2 )
		{
			isAliveFlag_[ 3 ] = 0;
		}

		//後ろから登場して左右移動
		if ( isAliveFlag_[ 3 ] == 0 )
		{
			if ( isMoveFlag_[ 3 ] == 0 )
			{
				if ( isGameStartTimer >= 200 )
				{
					Obj_[ 3 ]->wtf.position.z += 1.0f;
				}
				if ( Obj_[ 3 ]->wtf.position.z <= 20.0f && Obj_[ 3 ]->wtf.position.z >= 15.0f && player_->isRoundFlag == 3 )
				{
					BulletCoolTime_[ 3 ] = -50;
				}
				if ( Obj_[ 3 ]->wtf.position.z >= 35.0f )
				{
					Obj_[ 3 ]->wtf.position.z = 35.0f;
					isMoveFlag_[ 3 ] = 1;
				}
			}
		}
	}


	//自機の加減速でバイク兵のZ軸移動
	for ( int i = 0; i < 4; i++ ){
		if ( isAliveFlag_[ i ] == 0 ){
			if ( isMoveFlag_[ i ] == 1 ){
				if ( input_->PushKey(DIK_W) )
				{
					Obj_[ i ]->wtf.position.z -= 0.02f;
				}
				else if ( input_->PushKey(DIK_S) )
				{
					Obj_[ i ]->wtf.position.z += 0.02f;
				}
			}

			//定位置についたら魔導兵が左右に少しづつ動く
			if ( isMoveFlag_[ i ] == 1 )
			{
				if ( isbesideFlag_[ i ] == 0 )
				{
					Obj_[ i ]->wtf.position.x -= 0.02f;
				}
				else
				{
					Obj_[ i ]->wtf.position.x += 0.02f;
				}
			}
			//移動制限
			if ( isMoveFlag_[ i ] == 1 && Obj_[ i ]->wtf.position.x <= moveLLimit_[ i ] )
			{
				//左に移動制限
				isbesideFlag_[ i ] = 1;
			}
			else if ( isMoveFlag_[ i ] == 1 && Obj_[ i ]->wtf.position.x >= moveRLimit_[ i ] )
			{
				//右に移動制限
				isbesideFlag_[ i ] = 0;
			}

			//障害物に当たって自機が減速
			if ( player_->isDecelerationFlag == true )
			{
				Obj_[ i ]->wtf.position.z += 0.1f;
			}

		}

	}

	//魔導兵のガス噴射
	for ( int i = 0; i < 4; i++ ){
		if ( isAliveFlag_[ i ] == 0 )
		{
			if ( HP_[ i ] >= 1 )
			{
				isgasEffFlag_[ i ] = 1;
			}
			else
			{
				isgasEffFlag_[ i ] = 0;
				gasEffTimer_[ i ] = 0;
			}
		}
	}

	//魔導兵の射撃
	for ( int i = 0; i < 4; i++ )
	{
		if ( isAliveFlag_[ i ] == 0 )
		{
			if ( isGameStartTimer >= 200 && isShootFlag_[ i ] == 0 )
			{
				if ( isAliveFlag_[ i ] == 0 )
				{
					BulletCoolTime_[ i ]++;
				}
			}
			if ( BulletCoolTime_[ i ] == 219 )
			{
				playerlen_[ i ] = collObj3_->wtf.position - bulletObj_[ i ]->wtf.position;
				playerlen_[ i ].nomalize();
			}
			if ( BulletCoolTime_[ i ] >= 220 )
			{
				BulletCoolTime_[ i ] = 220;
				isShootFlag_[ i ] = 1;
			}
			//攻撃時モデルが変わる
			if ( BulletCoolTime_[ i ] >= 210 )
			{
				Obj_[ i ]->SetModel(Model_[ i ]);
			}
			else
			{
				Obj_[ i ]->SetModel(Modelst_[ i ]);
			}

			//誘導弾
			if ( isShootFlag_[ i ] == 1 )
			{
				BulletdurationTime_[ i ]++;

				bulletObj_[ i ]->wtf.position += playerlen_[ i ];
				bitweenlen_[ i ] = playerlen_[ i ];
				bitweenlen_[ i ] *= 0.1f;
			}
			else
			{
				bulletObj_[ i ]->wtf.position = { Obj_[ i ]->wtf.position.x,Obj_[ i ]->wtf.position.y + 2.5f,Obj_[ i ]->wtf.position.z - 3.0f };
			}
			if ( BulletdurationTime_[ i ] >= 80.0f )
			{
				BulletdurationTime_[ i ] = 0;
				isShootFlag_[ i ] = 0;
				BulletCoolTime_[ i ] = 0;
			}
		}
	}

	//ポリゴン爆散
	for ( int i = 0; i < 4; i++ ){
		if ( isAliveFlag_[ i ] == 0 ){
			if ( HP_[ i ] <= 0 )
			{
				isExpolFlag_[ i ] = 1;
			}
			if ( isExpolFlag_[ i ] == 1 )
			{
				ExpolTimer_[ i ]++;

				polygon_[ i ] = ExpolTimer_[ i ] / ExpolMT_[ i ];

				Obj_[ i ]->SetDestruction(polygon_[ i ]);
				if ( ExpolTimer_[ i ] >= ExpolMT_[ i ] )
				{
					gasEffTimer_[ i ] = 0;
					isAliveFlag_[ i ] = 1;
					isShootFlag_[ i ] = 0;
				}
			}
		}
	}

	//当たり判定(プレイヤー弾から魔導兵)
	if ( playerShootFlag == true){
		for ( int i = 0; i < 4; i++ ){
			if ( isAliveFlag_[ i ] == 0 ) {
				if ( coll.CircleCollision(playerBpos,collObj_[ i ]->wtf.position,1.0f,1.0f) ) {
					HP_[i]--;
					isdamEffFlag_[ i ] = 1;
					playerShootFlag = false;
				}
			}
		}
	}
	
	//当たり判定(魔導兵弾からプレイヤー)
	for ( int i = 0; i < 4; i++ ){
		if ( isAliveFlag_[ i ] == 0 ){
			if ( coll.CircleCollision(bulletObj_[ i ]->wtf.position,collObj3_->wtf.position,0.6f,0.6f) )
			{
				isShootFlag_[ i ] = 0;
				player_->isCamShake = 1;
				player_->camShakeTimer = player_->camShakeLimit;
				player_->hpgreenPosition.x -= 10.0f;//倍ダメ
				player_->hpgreenUI->SetPozition(player_->hpgreenPosition);
			}
		}
	}

	ImGui::Begin("ArmorEnemy");

	ImGui::Text("isGameStartTimer:%d",isGameStartTimer);
	ImGui::Text("BulletCoolTime:%d",BulletCoolTime_[2]);
	ImGui::Text("HP:%d",ExpolMT_[ 1 ]);

	ImGui::End();

}

void ArmorEnemy::Draw()
{

	//collObj3_->Draw();
	if ( isGameStartTimer >= 200 ){
		for ( int i = 0; i < 4; i++ ){
			//モデル描画
			if ( isAliveFlag_[i] == 0 ){
				Obj_[i]->Draw();
				/*collObj_[ i ]->Draw();*/
				//魔導兵弾描画
				if ( isShootFlag_[i] == 1 ){bulletObj_[i]->Draw();}
			}
		}
	}
}

void ArmorEnemy::Reset()
{

}

void ArmorEnemy::EffUpdate()
{
	for ( int i = 0; i < 4; i++ )
	{
		//ガス噴射
		if ( isgasEffFlag_[i] == 1 )
		{
			gasEffTimer_[ i ]++;
		}
		if ( gasEffTimer_[ i ] <= 20 && gasEffTimer_[ i ] >= 1 )
		{
			EffSummary(Vector3( Obj_[i]->wtf.position.x + 1.0f,Obj_[i]->wtf.position.y - 1.5f,Obj_[i]->wtf.position.z),i);
			EffSummary2(Vector3(Obj_[i]->wtf.position.x - 1.0f,Obj_[i]->wtf.position.y - 1.5f,Obj_[i]->wtf.position.z),i);
			EffSummary3(Vector3(Obj_[i]->wtf.position.x + 0.3f,Obj_[i]->wtf.position.y + 2.0f,Obj_[i]->wtf.position.z + 2.0f),i);
			EffSummary4(Vector3(Obj_[i]->wtf.position.x - 0.3f,Obj_[i]->wtf.position.y + 2.0f,Obj_[i]->wtf.position.z + 2.0f),i);
		}
		if ( gasEffTimer_[ i ] >= 20 )
		{
			isgasEffFlag_[ i ] = 0;
			gasEffTimer_[ i ] = 0;
		}

		//ダメージ
		if ( isdamEffFlag_[ i ] == 1 )
		{
			damEffTimer_[i]++;
		}
		if ( damEffTimer_[i] <= 10 && damEffTimer_[i] >= 1) {
			DamageSummary(Vector3(Obj_[i]->wtf.position.x,Obj_[i]->wtf.position.y,Obj_[i]->wtf.position.z),i);
		}
		if ( damEffTimer_[i] >= 10) {
			isdamEffFlag_[i] = 0;
			damEffTimer_[i] = 0;
		}
	}
}

void ArmorEnemy::EffSummary(Vector3 bulletpos,int num)
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
		const float rnd_velGz = 0.1f;
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
		gasParticle_[ num ]->Add(60,posG,velG,accG,0.5f,0.0f);

		gasParticle_[ num ]->Update();
	}
}

void ArmorEnemy::EffSummary2(Vector3 bulletpos2,int num2)
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
		const float rnd_velz2 = 0.1f;
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
		gasParticle2_[ num2 ]->Add(60,pos2,vel2,acc2,0.5f,0.0f);

		gasParticle2_[ num2 ]->Update();

	}

}

void ArmorEnemy::EffSummary3(Vector3 bulletpos3,int num3)
{
	//パーティクル範囲
	for ( int i = 0; i < 5; i++ )
	{
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_pos3 = 0.0f;
		const float rnd_posy3 = 0.0f;
		const float rnd_posz3 = 0.0f;
		Vector3 pos3{};
		pos3.x += ( float ) rand() / RAND_MAX * rnd_pos3 - rnd_pos3 / 2.0f;
		pos3.y += ( float ) rand() / RAND_MAX * rnd_posy3 - rnd_posy3 / 2.0f;
		pos3.z += ( float ) rand() / RAND_MAX * rnd_posz3 - rnd_posz3 / 2.0f;
		pos3 += bulletpos3;
		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		const float rnd_vel3 = 0.0f;
		const float rnd_vely3 = -0.05f;
		const float rnd_velz3 = 0.0f;
		Vector3 vel3{};
		vel3.x = ( float ) rand() / RAND_MAX * rnd_vel3 - rnd_vel3 / 2.0f;
		vel3.y = ( float ) rand() / RAND_MAX * rnd_vely3 - rnd_vely3 / 2.0f;
		vel3.z = ( float ) rand() / RAND_MAX * rnd_velz3 - rnd_velz3 / 1.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		const float rnd_acc3 = 0.000001f;
		Vector3 acc3{};
		acc3.x = ( float ) rand() / RAND_MAX * rnd_acc3 - rnd_acc3 / 2.0f;
		acc3.y = ( float ) rand() / RAND_MAX * rnd_acc3 - rnd_acc3 / 2.0f;

		//追加
		gasParticle3_[ num3 ]->Add(60,pos3,vel3,acc3,0.5f,0.0f);

		gasParticle3_[ num3 ]->Update();

	}
}

void ArmorEnemy::EffSummary4(Vector3 bulletpos4,int num4)
{
	//パーティクル範囲
	for ( int i = 0; i < 5; i++ )
	{
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_pos4 = 0.0f;
		const float rnd_posy4 = 0.0f;
		const float rnd_posz4 = 0.0f;
		Vector3 pos4{};
		pos4.x += ( float ) rand() / RAND_MAX * rnd_pos4 - rnd_pos4 / 2.0f;
		pos4.y += ( float ) rand() / RAND_MAX * rnd_posy4 - rnd_posy4 / 2.0f;
		pos4.z += ( float ) rand() / RAND_MAX * rnd_posz4 - rnd_posz4 / 2.0f;
		pos4 += bulletpos4;
		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		const float rnd_vel4 = 0.0f;
		const float rnd_vely4 = -0.05f;
		const float rnd_velz4 = 0.0f;
		Vector3 vel4{};
		vel4.x = ( float ) rand() / RAND_MAX * rnd_vel4 - rnd_vel4 / 2.0f;
		vel4.y = ( float ) rand() / RAND_MAX * rnd_vely4 - rnd_vely4 / 2.0f;
		vel4.z = ( float ) rand() / RAND_MAX * rnd_velz4 - rnd_velz4 / 2.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		const float rnd_acc4 = 0.000001f;
		Vector3 acc4{};
		acc4.x = ( float ) rand() / RAND_MAX * rnd_acc4 - rnd_acc4 / 2.0f;
		acc4.y = ( float ) rand() / RAND_MAX * rnd_acc4 - rnd_acc4 / 2.0f;

		//追加
		gasParticle4_[ num4 ]->Add(60,pos4,vel4,acc4,0.5f,0.0f);

		gasParticle4_[ num4 ]->Update();

	}
}

void ArmorEnemy::DamageSummary(Vector3 EnePos,int eneNum)
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
		pos += EnePos;
		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		const float rnd_vel = 0.5f;
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
		DamageParticle_[ eneNum ]->Add(60,pos,vel,acc,2.0f,1.0f);

		DamageParticle_[ eneNum ]->Update();
	}
}

void ArmorEnemy::EffDraw()
{
	if ( isGameStartTimer >= 200 ){
		for ( int i = 0; i < 4; i++ ){
			//背中の噴射ガス
			if ( isgasEffFlag_[i] == 1 ) {
				if ( isAliveFlag_[i] == 0 ){
					gasParticle_[i]->Draw();
					gasParticle2_[i]->Draw();
					if ( Obj_[i]->wtf.position.z >= 25.0f ){
						gasParticle3_[i]->Draw();
						gasParticle4_[i]->Draw();
					}
				}
			}
			//攻撃受けた時の火花のパーティクル
			if ( isdamEffFlag_[ i ] == 1 )
			{
				DamageParticle_[i]->Draw();
			}
		}
	}

}

