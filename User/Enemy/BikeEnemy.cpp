#include "BikeEnemy.h"
#include "Player.h"
#include <imgui.h>

BikeEnemy::BikeEnemy()
{
}

BikeEnemy::~BikeEnemy()
{
	delete spriteCommon;
	for ( int i = 0; i < 9; i++ )
	{
		delete Obj_[i];
		delete Model_[ i ];
		delete Model2_[ i ];
		delete collObj_[ i ];
		delete collModel_[ i ];
		delete bikclushObj_[i];
		delete bikclushModel_[i];
		delete collRightObj_[ i ];
		delete collLeftObj_[ i ];
		delete collLRModel_[i];
	}
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

	for ( int i = 0; i < 9; i++ )
	{
		//雑魚敵(攻撃状態)
		Model_[ i ] = Model::LoadFromOBJ("bikenemy");
		Model2_[ i ] = Model::LoadFromOBJ("bikenemy2");
		Obj_[ i ] = Object3d::Create();
		Obj_[ i ]->SetModel(Model_[ i ]);
		Obj_[ i ]->wtf.scale = { 0.4f,0.4f,0.4f };
		
	}
	//ラウンド1
	Obj_[0]->wtf.position = { -3.0f,-2.0f,-20.0f };
	Obj_[1]->wtf.position = { 3.0f,-2.0f,-20.0f };
	//ラウンド2
	Obj_[2]->wtf.position = { -3.0f,-2.0f,-20.0f };
	Obj_[3]->wtf.position = { 3.0f,-2.0f,-20.0f };
	//ラウンド3
	Obj_[4]->wtf.position = { -3.0f,-2.0f,-20.0f };
	Obj_[5]->wtf.position = { 3.0f,-2.0f,-20.0f };
	Obj_[6]->wtf.position = { 5.0f,-2.0f,-20.0f };
	//ラウンド4
	Obj_[7]->wtf.position = { -6.0f,-2.0f,-20.0f };
	Obj_[8]->wtf.position = { -3.0f,-2.0f,-20.0f };

	//当たり判定のモデル(本体)
	for ( int i = 0; i < 9; i++ )
	{
		collModel_[i] = Model::LoadFromOBJ("collboll");
		collObj_[i] = Object3d::Create();
		collObj_[i]->SetModel(collModel_[ i ]);
		collObj_[i]->wtf.position = { Obj_[i]->wtf.position.x,Obj_[i]->wtf.position.y,Obj_[i]->wtf.position.z - 1.0f };
		//ここでHP設定
		HP_[i] = 1;
	}
	//当たり判定のモデル(左右の衝突してノックバックする用)
	for ( int i = 0; i < 9; i++ )
	{
		collLRModel_[ i ] = Model::LoadFromOBJ("collboll");
		//右
		collRightObj_[ i ] = Object3d::Create();
		collRightObj_[ i ]->SetModel(collLRModel_[ i ]);
		collRightObj_[ i ]->wtf.position = { Obj_[ i ]->wtf.position.x + 0.2f ,Obj_[ i ]->wtf.position.y,Obj_[ i ]->wtf.position.z - 1.0f };
		//左
		collLeftObj_[ i ] = Object3d::Create();
		collLeftObj_[ i ]->SetModel(collLRModel_[ i ]);
		collLeftObj_[ i ]->wtf.position = { Obj_[ i ]->wtf.position.x - 0.2f ,Obj_[ i ]->wtf.position.y,Obj_[ i ]->wtf.position.z - 1.0f };

	}

	//雑魚敵(死んだときのバイクスピン)
	for ( int i = 0; i < 9; i++ )
	{
		bikclushModel_[ i ] = Model::LoadFromOBJ("bikclush");
		bikclushObj_[i] = Object3d::Create();
		bikclushObj_[i]->SetModel(bikclushModel_[ i ]);
		bikclushObj_[i]->wtf.scale = { 0.4f,0.4f,0.4f };
		bikclushObj_[i]->wtf.position = { Obj_[i]->wtf.position.x,Obj_[i]->wtf.position.y,Obj_[i]->wtf.position.z };
	}

	//パーティクル生成
	for ( int i = 0; i < 9; i++ )
	{
		gasParticle_[i] = std::make_unique<ParticleManager>();
		gasParticle_[i].get()->Initialize();
		gasParticle_[i]->LoadTexture("gas.png");
		gasParticle_[i]->Update();
	}
}

void BikeEnemy::Update(Vector3 playerSWPos,bool isCollSWFlag,Vector3 playerSWRightPos,bool isCollSWRightFlag,Vector3 playerRPos,Vector3 playerLPos)
{
	for ( int i = 0; i < 9; i++ )
	{
		Obj_[ i ]->Update();
		collObj_[ i ]->Update();
		collObj_[ i ]->wtf.position = { Obj_[ i ]->wtf.position.x,Obj_[ i ]->wtf.position.y + 0.5f,Obj_[ i ]->wtf.position.z - 1.0f };
		bikclushObj_[ i ]->Update();
		bikclushObj_[ i ]->wtf.position = { Obj_[ i ]->wtf.position.x,Obj_[ i ]->wtf.position.y,Obj_[ i ]->wtf.position.z };
		collRightObj_[ i ]->Update();
		collRightObj_[ i ]->wtf.position = { Obj_[ i ]->wtf.position.x + 0.2f ,Obj_[ i ]->wtf.position.y,Obj_[ i ]->wtf.position.z - 1.0f };
		collLeftObj_[ i ]->Update();
		collLeftObj_[ i ]->wtf.position = { Obj_[ i ]->wtf.position.x - 0.2f ,Obj_[ i ]->wtf.position.y,Obj_[ i ]->wtf.position.z - 1.0f };
	}

	EffUpdate();
	isGameStartTimer++;

	//バイクの車輪が動き出す
	bikstSpinTimer++;
	if ( bikstSpinTimer > 10 )
	{
		bikstSpinTimer = 0;
	}
	for ( int i = 0; i < 9; i++ )
	{
		if ( bikstSpinTimer >= 1 && bikstSpinTimer <= 5 )
		{
			Obj_[ i ]->SetModel(Model_[ i ]);
		}
		else if ( bikstSpinTimer >= 6 && bikstSpinTimer <= 10 )
		{
			Obj_[ i ]->SetModel(Model2_[ i ]);
		}
	}

	//バイク兵のエントリー
	BikeEnemyEntry();

	//エントリー後のバイク兵のアクション
	for ( int i = 0; i < 9; i++ ){
		if ( isBackEntryFlag_[i] == 1 ){actionTimer_[i]++;}
	}
	BikeEnemyAction();
	


	//自機の加減速でバイク兵のZ軸移動
	for ( int i = 0; i < 9; i++ ){
		if ( isBackEntryFlag_[ i ] == 1 ){
			if ( input_->PushKey(DIK_W) ){Obj_[ i ]->wtf.position.z -= 0.06f;}
			else if ( input_->PushKey(DIK_S) ){Obj_[ i ]->wtf.position.z += 0.06f;}
		}
	}

	//当たり判定
	for ( int i = 0; i < 9; i++ )
	{
		//自機の左攻撃の当たり判定
		if ( HP_[i] >= 1 )
		{
			if ( isCollSWFlag == true )
			{
				if ( coll.CircleCollision(playerSWPos,collObj_[i]->wtf.position,0.6f,0.6f) )
				{
					HP_[i]--;
				}
			}
		}

		//自機の右攻撃の当たり判定
		if ( HP_[ i ] >= 1 )
		{
			if ( isCollSWRightFlag == true )
			{
				if ( coll.CircleCollision(playerSWRightPos,collObj_[ i ]->wtf.position,0.6f,0.6f) )
				{
					HP_[ i ]--;
				}
			}
		}

		////////////////////////////////
		//--自機とバイク兵の押し出し処理-////	
		//自機が左にいるバイク兵に対して衝突するとき
		if ( HP_[ i ] >= 1 )
		{
			if ( coll.CircleCollision(playerLPos,collRightObj_[ i ]->wtf.position,0.6f,0.6f) )
			{
				player_->limitmove = true;
				limitRightmove_[ i ] = 1;
			}
		}
		//左にいるバイク兵のノックバック
		if ( limitRightmove_[ i ] == 1 )
		{
			knockbackTimer2_[ i ]++;
		}
		if ( knockbackTimer2_[ i ] >= 1 && knockbackTimer2_[ i ] <= 10 )
		{
			//右側にノックバック
			Obj_[ i ]->wtf.position.x -= 0.03f;
		}
		if ( knockbackTimer2_[ i ] >= 11 )
		{
			player_->limitmove = false;
			limitRightmove_[ i ] = 0;
			knockbackTimer2_[ i ] = 0;
		}

		//自機が右にいるバイク兵に対して衝突するとき
		if ( HP_[ i ] >= 1 )
		{
			if ( coll.CircleCollision(playerRPos,collLeftObj_[ i ]->wtf.position,0.6f,0.6f) )
			{
				player_->limitmove2 = true;
				limitLeftmove_[ i ] = 1;
			}
		}
		//右にいるバイク兵のノックバック
		if ( limitLeftmove_[ i ] == 1 ){
			knockbackTimer_[ i ]++;
		}
		if ( knockbackTimer_[ i ] >= 1 && knockbackTimer_[ i ] <= 10){
			//右側にノックバック
			Obj_[ i ]->wtf.position.x += 0.03f;
		}
		if ( knockbackTimer_[ i ] >= 11 )
		{
			player_->limitmove2 = false;
			limitLeftmove_[ i ] = 0;
			knockbackTimer_[ i ] = 0;
		}

	}

	//バイク兵のガス噴射
	for ( int i = 0; i < 9; i++ ){
		if ( HP_[i] <= 0 )
		{
			isBikclushFlag_[i] = 1;
			isbulletEffFlag_[i] = 0;
			bulletEffTimer_[i] = 0;
		}
		else
		{
			isbulletEffFlag_[i] = 1;
		}
	}


	//バイクが前進しながら倒れる
	for ( int i = 0; i < 9; i++ )
	{
		if ( isBackEntryFlag_[i] == 1 )
		{
			if ( isBikclushFlag_[ i ] == 1 )
			{
				Obj_[i]->wtf.position.z += 0.5f;
				bikclushObj_[i]->wtf.position.z += 0.5f;
				bikclushObj_[i]->wtf.rotation.z += 0.1f;
			}
			if ( bikclushObj_[i]->wtf.rotation.z >= 1.5f )
			{
				bikclushObj_[i]->wtf.rotation.z = 1.5f;
				isBikSpinFlag_[i] = 1;
			}
			//バイクが倒れて後ろに行く
			if ( isBikSpinFlag_[i] == 1 )
			{
				Obj_[i]->wtf.position.z -= 0.8f;
				bikclushObj_[i]->wtf.position.z -= 0.8f;
				bikclushObj_[i]->wtf.rotation.x -= 0.1f;
			}
		}
	}

	ImGui::Begin("bikeEnemy");

	ImGui::Text("Position_[1]:%f,%f,%f",Obj_[ 1 ]->wtf.position.x,Obj_[ 1 ]->wtf.position.y,Obj_[ 1 ]->wtf.position.z);
	ImGui::Text("HP:%d",HP_[ 0 ]);
	ImGui::Text("isMoveFlag_[ 1 ]:%d",isMoveFlag_[ 1 ]);
	ImGui::Text("isBackEntryFlag:%d",isBackEntryFlag_[1]);

	ImGui::End();

}

void BikeEnemy::Draw()
{
	if ( isGameStartTimer >= 200 ){
		for ( int i = 0; i < 9; i++ )
		{
			//バイク兵のモデル
			if ( isBikclushFlag_[i] == 0 )
			{
				Obj_[i]->Draw();
				/*collObj_[i]->Draw();*/
				/*collRightObj_[ i ]->Draw();
				collLeftObj_[ i ]->Draw();*/
			}

			//バイク兵の撃破モデル
			if ( isBikclushFlag_[i] == 1 )
			{
				if ( isBackEntryFlag_[i] == 1 )
				{
					if ( bikclushObj_[i]->wtf.position.z >= -20.0f )
					{
						bikclushObj_[i]->Draw();
					}
				}
			}


		}

		
	}

}

void BikeEnemy::EffUpdate()
{
	for ( int i = 0; i < 9; i++ )
	{
		if ( isbulletEffFlag_[i] == 1 )
		{
			bulletEffTimer_[i]++;
		}
		if ( bulletEffTimer_[i] <= 20 && bulletEffTimer_[i] >= 1 )
		{
			EffSummary(Vector3(Obj_[i]->wtf.position.x,Obj_[i]->wtf.position.y,Obj_[i]->wtf.position.z),i);
		}
		if ( bulletEffTimer_[i] >= 20 )
		{
			isbulletEffFlag_[i] = 0;
			bulletEffTimer_[i] = 0;
		}
	}
}

void BikeEnemy::EffSummary(Vector3 bulletpos,int num)
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
		gasParticle_[num]->Add(60,posG,velG,accG,0.1f,0.0f);

		gasParticle_[ num ]->Update();

	}
}

void BikeEnemy::EffDraw()
{
	for ( int i = 0; i < 9; i++ )
	{
		if ( isGameStartTimer >= 200 ){
			if ( isBikclushFlag_[i] == 0 ) {
				if ( isbulletEffFlag_[i] == 1 ) {
					gasParticle_[i]->Draw();
				}
			}
		}
	}
}

Vector3 BikeEnemy::GetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;

	//collObj_->wtf.UpdateMat();
	////ワールド行列の平行移動成分
	//worldPos.x = collObj_->wtf.matWorld.m[ 3 ][ 0 ];
	//worldPos.y = collObj_->wtf.matWorld.m[ 3 ][ 1 ];
	//worldPos.z = collObj_->wtf.matWorld.m[ 3 ][ 2 ];

	return worldPos;
}

void BikeEnemy::BikeEnemyEntry()
{
	//バイク兵のエントリー(ラウンド1)
	for ( int i = 0; i < 2; i++ )
	{
		if ( isBackEntryFlag_[ i ] == 0 )
		{
			if ( isGameStartTimer >= 220 )
			{
				Obj_[ i ]->wtf.position.z += 0.5f;
			}
			if ( Obj_[ i ]->wtf.position.z >= 5.0f )
			{
				Obj_[ i ]->wtf.position.z = 5.0f;
				isBackEntryFlag_[ i ] = 1;
			}
		}
	}

	//バイク兵のエントリー(ラウンド2)
	if ( player_->isRoundFlag == 1 )
	{
		AliveR2Timer++;
		if ( AliveR2Timer >= 5 )
		{
			if ( isBackEntryFlag_[ 2 ] == 0 )
			{
				if ( isGameStartTimer >= 220 )
				{
					Obj_[ 2 ]->wtf.position.z += 0.5f;
				}
				if ( Obj_[ 2 ]->wtf.position.z >= 5.0f )
				{
					Obj_[ 2 ]->wtf.position.z = 5.0f;
					isBackEntryFlag_[ 2 ] = 1;
				}
			}
		}
		if ( AliveR2Timer >= 30 )
		{
			if ( isBackEntryFlag_[ 3 ] == 0 )
			{
				if ( isGameStartTimer >= 220 )
				{
					Obj_[ 3 ]->wtf.position.z += 0.5f;
				}
				if ( Obj_[ 3 ]->wtf.position.z >= 5.0f )
				{
					Obj_[ 3 ]->wtf.position.z = 5.0f;
					isBackEntryFlag_[ 3 ] = 1;
				}
			}
		}

	}

	//バイク兵のエントリー(ラウンド3)
	if ( player_->isRoundFlag == 3 )
	{
		AliveR3Timer++;
		if ( AliveR3Timer >= 5 )
		{
			if ( isBackEntryFlag_[ 4 ] == 0 )
			{
				if ( isGameStartTimer >= 220 )
				{
					Obj_[ 4 ]->wtf.position.z += 0.5f;
				}
				if ( Obj_[ 4 ]->wtf.position.z >= 10.0f )
				{
					Obj_[ 4 ]->wtf.position.z = 10.0f;
					isBackEntryFlag_[ 4 ] = 1;
				}
			}
		}

		if ( AliveR3Timer >= 15 )
		{
			if ( isBackEntryFlag_[ 5 ] == 0 )
			{
				if ( isGameStartTimer >= 220 )
				{
					Obj_[ 5 ]->wtf.position.z += 0.5f;
				}
				if ( Obj_[ 5 ]->wtf.position.z >= 10.0f )
				{
					Obj_[ 5 ]->wtf.position.z = 10.0f;
					isBackEntryFlag_[ 5 ] = 1;
				}
			}
		}

		if ( AliveR3Timer >= 20 )
		{
			if ( isBackEntryFlag_[ 6 ] == 0 )
			{
				if ( isGameStartTimer >= 220 )
				{
					Obj_[ 6 ]->wtf.position.z += 1.0f;
				}
				if ( Obj_[ 6 ]->wtf.position.z >= 20.0f )
				{
					Obj_[ 6 ]->wtf.position.z = 20.0f;
					isBackEntryFlag_[ 6 ] = 1;
				}
			}
		}
	}

	//バイク兵のエントリー(ラウンド4)
	if ( player_->isRoundFlag == 5 )
	{
		AliveR4Timer++;
		if ( AliveR4Timer >= 5 )
		{
			if ( isBackEntryFlag_[7] == 0 )
			{
				if ( isGameStartTimer >= 220 )
				{
					Obj_[7]->wtf.position.z += 0.5f;
				}
				if ( Obj_[7]->wtf.position.z >= 5.0f )
				{
					Obj_[7 ]->wtf.position.z = 5.0f;
					isBackEntryFlag_[7] = 1;
				}
			}
		}

		if ( AliveR4Timer >= 5 )
		{
			if ( isBackEntryFlag_[8] == 0 )
			{
				if ( isGameStartTimer >= 220 )
				{
					Obj_[ 8 ]->wtf.position.z += 0.5f;
				}
				if ( Obj_[8]->wtf.position.z >= 10.0f )
				{
					Obj_[ 8 ]->wtf.position.z = 10.0f;
					isBackEntryFlag_[ 8 ] = 1;
				}
			}
		}
	}

}

void BikeEnemy::BikeEnemyAction()
{
	//ラウンド1の左のバイク兵
	if ( actionTimer_[ 0 ] >= 10 ){
		if ( isMoveFlag_[ 0 ] == 1 && limitLeftmove_[0] == 0 ){
			//左に移動
			Obj_[ 0 ]->wtf.position.x -= 0.03f;
			Obj_[ 0 ]->wtf.rotation.z += 0.03f;
			//傾き制限
			if ( Obj_[ 0 ]->wtf.rotation.z >= 0.2f )
			{
				Obj_[ 0 ]->wtf.rotation.z = 0.2f;
			}

			//一定距離でフラグ切り替え
			if ( Obj_[ 0 ]->wtf.position.x <= -6.0f )
			{
				Obj_[ 0 ]->wtf.position.x = -6.0f;
				isMoveFlag_[ 0 ] = 0;
			}

			//衝突時のノックバックの移動制限
			if ( Obj_[ 0 ]->wtf.position.x >= -0.5f )
			{
				Obj_[ 0 ]->wtf.position.x = -0.5f;
			}

		}

		if ( isMoveFlag_[ 0 ] == 0 && limitRightmove_[ 0 ] == 0 ){
			//右に移動
			Obj_[ 0 ]->wtf.position.x += 0.03f;
			Obj_[ 0 ]->wtf.rotation.z -= 0.03f;
			//傾き制限
			if ( Obj_[ 0 ]->wtf.rotation.z <= -0.2f )
			{
				Obj_[ 0 ]->wtf.rotation.z = -0.2f;
			}

			//一定距離でフラグ切り替え
			if ( Obj_[ 0 ]->wtf.position.x >= -0.5f )
			{
				Obj_[ 0 ]->wtf.position.x = -0.5f;
				isMoveFlag_[ 0 ] = 1;
			}

			//衝突時のノックバックの移動制限
			if ( Obj_[ 0 ]->wtf.position.x <= -6.0f )
			{
				Obj_[ 0 ]->wtf.position.x = -6.0f;
			}
		}
	}

	//ラウンド1の右のバイク兵
	if ( actionTimer_[ 1 ] >= 25 )
	{
		if ( isMoveFlag_[ 1 ] == 0 )
		{
			stopTimer_[ 1 ] = 0;
			//左に移動
			Obj_[ 1 ]->wtf.position.x -= 0.03f;
			Obj_[ 1 ]->wtf.rotation.z += 0.03f;
			//傾き制限
			if ( Obj_[ 1 ]->wtf.rotation.z >= 0.2f )
			{
				Obj_[ 1 ]->wtf.rotation.z = 0.2f;
			}

			//一定距離でフラグ切り替え
			if ( Obj_[ 1 ]->wtf.position.x <= 1.5f )
			{
				Obj_[ 1 ]->wtf.position.x = 1.5f;
				isMoveFlag_[ 1 ] = 1;
			}
		}
		//左に移動後一旦止まる(攻撃するかも？)
		if ( isMoveFlag_[ 1 ] == 1 ){
			stopTimer_[1]++;
			//傾きリセット
			Obj_[ 1 ]->wtf.rotation.z -= 0.03f;
			//傾き制限
			if ( Obj_[ 1 ]->wtf.rotation.z <= 0.0f ){Obj_[ 1 ]->wtf.rotation.z = 0.0f;}
		}
		if(stopTimer_[1] >= 60.0f ){isMoveFlag_[ 1 ] = 2;}

		if ( isMoveFlag_[ 1 ] == 2 )
		{
			stopTimer_[ 1 ] = 0;
			//右に移動
			Obj_[ 1 ]->wtf.position.x += 0.03f;
			Obj_[ 1 ]->wtf.rotation.z -= 0.03f;
			//傾き制限
			if ( Obj_[ 1 ]->wtf.rotation.z <= -0.2f )
			{
				Obj_[ 1 ]->wtf.rotation.z = -0.2f;
			}

			//一定距離でフラグ切り替え
			if ( Obj_[ 1 ]->wtf.position.x >= 7.0f )
			{
				Obj_[ 1 ]->wtf.position.x = 7.0f;
				isMoveFlag_[ 1 ] = 3;
			}
		}
		//右に移動後一旦止まる(攻撃するかも？)
		if ( isMoveFlag_[ 1 ] == 3 )
		{
			stopTimer_[ 1 ]++;
			//傾きリセット
			Obj_[ 1 ]->wtf.rotation.z += 0.03f;
			//傾き制限
			if ( Obj_[ 1 ]->wtf.rotation.z >= 0.0f )
			{
				Obj_[ 1 ]->wtf.rotation.z = 0.0f;
			}

		}
		if ( stopTimer_[ 1 ] >= 60.0f )
		{
			isMoveFlag_[ 1 ] = 0;
		}


	}

	//ラウンド2の左のバイク兵
	if ( actionTimer_[2] >= 40 )
	{
		if ( isMoveFlag_[2] == 1 )
		{
			//左に移動
			Obj_[2]->wtf.position.x -= 0.03f;
			Obj_[2]->wtf.rotation.z += 0.03f;
			//傾き制限
			if ( Obj_[2]->wtf.rotation.z >= 0.2f )
			{
				Obj_[2]->wtf.rotation.z = 0.2f;
			}

			//一定距離でフラグ切り替え
			if ( Obj_[2]->wtf.position.x <= -6.0f )
			{
				Obj_[2]->wtf.position.x = -6.0f;
				isMoveFlag_[2] = 0;
			}
		}

		if ( isMoveFlag_[2] == 0 ){
			//右に移動
			Obj_[2]->wtf.position.x += 0.03f;
			Obj_[2]->wtf.rotation.z -= 0.03f;
			//傾き制限
			if ( Obj_[2]->wtf.rotation.z <= -0.2f )
			{
				Obj_[2]->wtf.rotation.z = -0.2f;
			}

			//一定距離でフラグ切り替え
			if ( Obj_[2]->wtf.position.x >= -0.5f )
			{
				Obj_[2]->wtf.position.x = -0.5f;
				isMoveFlag_[2] = 1;
			}
		}
	}

	//ラウンド2の右のバイク兵
	if ( actionTimer_[3] >= 25 )
	{
		if ( isMoveFlag_[3] == 0 )
		{
			stopTimer_[3] = 0;
			//左に移動
			Obj_[ 3]->wtf.position.x -= 0.03f;
			Obj_[ 3 ]->wtf.rotation.z += 0.03f;
			//傾き制限
			if ( Obj_[ 3 ]->wtf.rotation.z >= 0.2f )
			{
				Obj_[ 3 ]->wtf.rotation.z = 0.2f;
			}

			//一定距離でフラグ切り替え
			if ( Obj_[ 3 ]->wtf.position.x <= 1.5f )
			{
				Obj_[ 3 ]->wtf.position.x = 1.5f;
				isMoveFlag_[ 3 ] = 1;
			}
		}
		//左に移動後一旦止まる(攻撃するかも？)
		if ( isMoveFlag_[ 3 ] == 1 )
		{
			stopTimer_[ 3 ]++;
			//傾きリセット
			Obj_[ 3 ]->wtf.rotation.z -= 0.03f;
			//傾き制限
			if ( Obj_[ 3 ]->wtf.rotation.z <= 0.0f )
			{
				Obj_[ 3 ]->wtf.rotation.z = 0.0f;
			}
		}
		if ( stopTimer_[ 3 ] >= 60.0f )
		{
			isMoveFlag_[ 3 ] = 2;
		}

		if ( isMoveFlag_[ 3 ] == 2 )
		{
			stopTimer_[ 3 ] = 0;
			//右に移動
			Obj_[ 3 ]->wtf.position.x += 0.03f;
			Obj_[ 3 ]->wtf.rotation.z -= 0.03f;
			//傾き制限
			if ( Obj_[ 3 ]->wtf.rotation.z <= -0.2f )
			{
				Obj_[ 3 ]->wtf.rotation.z = -0.2f;
			}

			//一定距離でフラグ切り替え
			if ( Obj_[ 3 ]->wtf.position.x >= 7.0f )
			{
				Obj_[ 3 ]->wtf.position.x = 7.0f;
				isMoveFlag_[ 3 ] = 3;
			}
		}
		//右に移動後一旦止まる(攻撃するかも？)
		if ( isMoveFlag_[ 3 ] == 3 )
		{
			stopTimer_[ 3 ]++;
			//傾きリセット
			Obj_[ 3 ]->wtf.rotation.z += 0.03f;
			//傾き制限
			if ( Obj_[ 3 ]->wtf.rotation.z >= 0.0f )
			{
				Obj_[ 3 ]->wtf.rotation.z = 0.0f;
			}

		}
		if ( stopTimer_[ 3 ] >= 60.0f )
		{
			isMoveFlag_[ 3 ] = 0;
		}


	}

	//ラウンド4の右のバイク兵
	if ( actionTimer_[8] >= 5 )
	{
		if ( isMoveFlag_[8 ] == 1 )
		{
			//左に移動
			Obj_[ 8 ]->wtf.position.x -= 0.03f;
			Obj_[ 8 ]->wtf.position.z += 0.03f;
			Obj_[ 8 ]->wtf.rotation.z += 0.03f;
			//傾き制限
			if ( Obj_[ 8 ]->wtf.rotation.z >= 0.2f )
			{
				Obj_[ 8 ]->wtf.rotation.z = 0.2f;
			}

			//一定距離でフラグ切り替え
			if ( Obj_[8 ]->wtf.position.x <= 0.5f )
			{
				Obj_[ 8 ]->wtf.position.x = 0.5f;
				isMoveFlag_[ 8 ] = 0;
			}
		}

		if ( isMoveFlag_[8] == 0 )
		{
			//右に移動
			Obj_[8]->wtf.position.x += 0.03f;
			Obj_[8]->wtf.rotation.z -= 0.03f;
			//傾き制限
			if ( Obj_[8]->wtf.rotation.z <= -0.2f )
			{
				Obj_[8]->wtf.rotation.z = -0.2f;
			}

			//一定距離でフラグ切り替え
			if ( Obj_[ 8 ]->wtf.position.x >= 6.0f )
			{
				Obj_[ 8 ]->wtf.position.x = 6.0f;
				isMoveFlag_[ 8 ] = 1;
			}
		}
	}

}
