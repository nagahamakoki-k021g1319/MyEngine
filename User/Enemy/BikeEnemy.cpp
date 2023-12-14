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
		delete collFrontObj_[ i ];
		delete collBackObj_[ i ];

	}
	for ( int i = 0; i < 5; i++ )
	{
		delete collRushObj_[ i ];
		delete collRushObjL_[ i ];
		delete collRushModel_[ i ];
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
	Obj_[0]->wtf.position = { -3.0f,-2.0f,-30.0f };
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
		collRightObj_[ i ]->wtf.position = { Obj_[ i ]->wtf.position.x + 0.1f ,Obj_[ i ]->wtf.position.y,Obj_[ i ]->wtf.position.z - 1.0f };
		//左
		collLeftObj_[ i ] = Object3d::Create();
		collLeftObj_[ i ]->SetModel(collLRModel_[ i ]);
		collLeftObj_[ i ]->wtf.position = { Obj_[ i ]->wtf.position.x - 0.1f ,Obj_[ i ]->wtf.position.y,Obj_[ i ]->wtf.position.z - 1.0f };
		//前
		collFrontObj_[ i ] = Object3d::Create();
		collFrontObj_[ i ]->SetModel(collLRModel_[ i ]);
		collFrontObj_[ i ]->wtf.position = { Obj_[ i ]->wtf.position.x,Obj_[ i ]->wtf.position.y,Obj_[ i ]->wtf.position.z + 1.0f };
		//後
		collBackObj_[ i ] = Object3d::Create();
		collBackObj_[ i ]->SetModel(collLRModel_[ i ]);
		collBackObj_[ i ]->wtf.position = { Obj_[ i ]->wtf.position.x,Obj_[ i ]->wtf.position.y,Obj_[ i ]->wtf.position.z - 1.0f };

	}

	//突進用に自機に対して横一列になってる当たり判定モデル
	for ( int i = 0; i < 5; i++ )
	{
		collRushModel_[ i ] = Model::LoadFromOBJ("collboll");
		collRushObj_[ i ] = Object3d::Create();
		collRushObj_[ i ]->SetModel(collRushModel_[ i ]);
		collRushObjL_[ i ] = Object3d::Create();
		collRushObjL_[ i ]->SetModel(collRushModel_[ i ]);
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
		collRightObj_[ i ]->wtf.position = { Obj_[ i ]->wtf.position.x + 0.1f ,Obj_[ i ]->wtf.position.y,Obj_[ i ]->wtf.position.z - 1.0f };
		collLeftObj_[ i ]->Update();
		collLeftObj_[ i ]->wtf.position = { Obj_[ i ]->wtf.position.x - 0.1f ,Obj_[ i ]->wtf.position.y,Obj_[ i ]->wtf.position.z - 1.0f };
		collFrontObj_[ i ]->Update();
		collFrontObj_[ i ]->wtf.position = { Obj_[ i ]->wtf.position.x,Obj_[ i ]->wtf.position.y,Obj_[ i ]->wtf.position.z + 1.0f };
		collBackObj_[ i ]->Update();
		collBackObj_[ i ]->wtf.position = { Obj_[ i ]->wtf.position.x,Obj_[ i ]->wtf.position.y,Obj_[ i ]->wtf.position.z - 1.0f };
	}
	//自機から横に出ているモデル
	for ( int i = 0; i < 5; i++ ){
		collRushObj_[i]->Update();
		collRushObj_[ i ]->wtf.position = { playerRPos.x + i + 0.5f,playerRPos.y - 0.5f,playerRPos.z};
		collRushObjL_[ i ]->Update();
		collRushObjL_[ i ]->wtf.position = { playerRPos.x - i + 0.5f,playerRPos.y - 0.5f,playerRPos.z };
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
	//バイク兵の挙動と自機より後ろにいるときカメラを少し下げる
	BikeEnemyAction();


	//突進攻撃の当たり判定
	for ( int i = 0; i < 5; i++ ){
		for ( int j = 0; j < 9; j++ ){
			//突進攻撃の当たり判定(右の敵と右側の自機の横並びモデル)
			if ( isBackEntryFlag_[j] == 1 && HP_[j] >= 1 && isRushFlag_[j] == 0 )
			{
				if ( coll.CircleCollision(collRushObj_[ i ]->wtf.position,collObj_[j]->wtf.position,0.5f,0.5f) )
				{
					isRushFlag_[j] = 1;
				}
			}
			//突進攻撃の当たり判定(左の敵と左側の自機の横並びモデル)
			if ( isBackEntryFlag_[j] == 1 && HP_[j] >= 1 && isRushFlag_[j] == 0 )
			{
				if ( coll.CircleCollision(collRushObjL_[ i ]->wtf.position,collObj_[j]->wtf.position,0.5f,0.5f) )
				{
					isRushFlag_[j] = 4;
				}
			}
		}
	}
	//突進攻撃の当たり判定(右の敵と自機)
	for ( int i = 0; i < 9; i++ )
	{
		if ( isRushFlag_[i] == 1 )
		{
			Obj_[i]->wtf.position.x -= 0.08f;
			if ( Obj_[i]->wtf.position.x <= -3.0f )
			{
				Obj_[i]->wtf.position.x = -3.0f;
				isRushFlag_[ i ] = 3;
			}
			Obj_[i]->wtf.rotation.z += 0.03f;
			if ( Obj_[i]->wtf.rotation.z >= 0.45f )
			{
				Obj_[i]->wtf.rotation.z = 0.45f;
			}
			//突進中に自機と当たったらフラグ切り替え
			if ( coll.CircleCollision(playerRPos,collLeftObj_[i]->wtf.position,0.6f,0.6f) )
			{
				isRushKnockbackFlag_[ i ] = 1;
				player_->isKnockbackFlag = true;
				player_->isCamShake = 1;
				player_->camShakeTimer = player_->camShakeLimit;
				player_->hpgreenPosition.x -= 10.0f;//倍ダメ
				player_->hpgreenUI->SetPozition(player_->hpgreenPosition);
			}
		}
		//当たったらノックバックタイマー起動
		if ( isRushKnockbackFlag_[ i ] == 1 )
		{
			rushKnockbackTimer_[ i ]++;
			isRushFlag_[ i ] = 2;
		}
		//タイマー起動時元の位置に戻る
		if ( rushKnockbackTimer_[ i ] >= 1 )
		{
			Obj_[i]->wtf.rotation.z -= 0.03f;
			if ( Obj_[i]->wtf.rotation.z <= 0.0f )
			{
				Obj_[i]->wtf.rotation.z = 0.0f;
			}
			Obj_[i]->wtf.position.x += 0.03f;
			if ( Obj_[i]->wtf.position.x >= 4.0f )
			{
				Obj_[i]->wtf.position.x = 4.0f;
				isRushFlag_[ i ] = 3;
				isRushKnockbackFlag_[ i ] = 0;
				rushKnockbackTimer_[ i ] = 0;
			}
		}
		//突進後クールタイム
		if ( isRushFlag_[ i ] == 3 )
		{
			Obj_[i]->wtf.rotation.z -= 0.03f;
			if ( Obj_[i]->wtf.rotation.z <= 0.0f )
			{
				Obj_[i]->wtf.rotation.z = 0.0f;
			}
			rushCoolTimer_[ i ]++;
		}

		//突進攻撃の当たり判定(左の敵と自機)
		if ( isRushFlag_[ i ] == 4 )
		{
			Obj_[i]->wtf.position.x += 0.08f;
			if ( Obj_[i]->wtf.position.x >= 3.0f )
			{
				Obj_[i]->wtf.position.x = 3.0f;
				isRushFlag_[ i ] = 5;
			}
			Obj_[i]->wtf.rotation.z -= 0.03f;
			if ( Obj_[i]->wtf.rotation.z <= -0.45f )
			{
				Obj_[i]->wtf.rotation.z = -0.45f;
			}
			//突進中に自機と当たったらフラグ切り替え
			if ( coll.CircleCollision(playerLPos,collRightObj_[i]->wtf.position,0.6f,0.6f) )
			{
				isRushKnockbackFlag_[ i ] = 2;
				player_->isKnockbackFlagL = true;
				player_->isCamShake = 1;
				player_->camShakeTimer = player_->camShakeLimit;
				player_->hpgreenPosition.x -= 10.0f;//倍ダメ
				player_->hpgreenUI->SetPozition(player_->hpgreenPosition);
			}
		}
		//当たったらノックバックタイマー起動
		if ( isRushKnockbackFlag_[ i ] == 2 )
		{
			rushKnockbackTimerR_[ i ]++;
			isRushFlag_[ i ] = 6;
		}
		//タイマー起動時元の位置に戻る
		if ( rushKnockbackTimerR_[ i ] >= 1 )
		{
			Obj_[i]->wtf.rotation.z += 0.03f;
			if ( Obj_[i]->wtf.rotation.z >= 0.0f )
			{
				Obj_[i]->wtf.rotation.z = 0.0f;
			}
			Obj_[i]->wtf.position.x -= 0.03f;
			if ( Obj_[i]->wtf.position.x <= -4.0f )
			{
				Obj_[i]->wtf.position.x = -4.0f;
				isRushFlag_[ i ] = 5;
				isRushKnockbackFlag_[ i ] = 0;
				rushKnockbackTimerR_[ i ] = 0;
			}
		}
		//突進後クールタイム
		if ( isRushFlag_[ i ] == 5 )
		{
			Obj_[i]->wtf.rotation.z += 0.03f;
			if ( Obj_[i]->wtf.rotation.z >= 0.0f )
			{
				Obj_[i]->wtf.rotation.z = 0.0f;
			}
			rushCoolTimer_[ i ]++;
		}
		if ( rushCoolTimer_[ i ] >= 200 )
		{
			isRushFlag_[ i ] = 0;
			rushCoolTimer_[ i ] = 0;
		}
	}

	//自機の加減速でバイク兵のZ軸移動
	for ( int i = 0; i < 9; i++ ){
		if ( isBackEntryFlag_[ i ] == 1 ){
			if ( input_->PushKey(DIK_W) ){Obj_[ i ]->wtf.position.z -= 0.06f;}
			else if ( input_->PushKey(DIK_S) ){Obj_[ i ]->wtf.position.z += 0.06f;}
			//自機が障害物と当たった時
			if ( player_->backTimer >= 1 && player_->backTimer <= 29 )
			{
				Obj_[ i ]->wtf.position.z += 0.07f;
			}
			
		}
	}

	

	//当たり判定(自機とバイク兵の当たり判定)
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
		if ( HP_[ i ] >= 1 ){
			if ( coll.CircleCollision(playerLPos,collRightObj_[ i ]->wtf.position,0.6f,0.6f) )
			{
				player_->limitmove = true;
				limitRightmove_[ i ] = 1;
				isHit_[ i ]++;
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
		if ( HP_[ i ] >= 1 ){
			if ( coll.CircleCollision(playerRPos,collLeftObj_[ i ]->wtf.position,0.6f,0.6f) )
			{
				player_->limitmove2 = true;
				limitLeftmove_[ i ] = 1;
				isHit_[ i ]++;
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

	//当たり判定(バイク兵同士の当たり判定)
	BiketoBikeColl();
	
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

	//数回タックルされたらやられる
	for ( int i = 0; i < 9; i++ )
	{
		if ( isHit_[ i ] >= 20 )
		{
			HP_[ i ] = 0;
		}
	}

	ImGui::Begin("bikeEnemy");
	ImGui::Text("position_:%f,%f,%f",Obj_[ 1]->wtf.position.x,Obj_[ 1 ]->wtf.position.y,Obj_[ 1 ]->wtf.position.z);
	ImGui::End();
	/*ImGui::Text("HP:%d",HP_[ 0 ]);
	ImGui::Text("isHit_:%d",isHit_[1]);
	ImGui::Text("isBackEntryFlag:%d",isBackEntryFlag_[1]);*/

	

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
			/*	collObj_[i]->Draw();
				collRightObj_[ i ]->Draw();
				collLeftObj_[ i ]->Draw();*/
				/*collFrontObj_[ i ]->Draw();
				collBackObj_[ i ]->Draw(); */

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
		//突進用に自機に対して横一列になってる当たり判定モデル
		for ( int i = 0; i < 5; i++ )
		{
			/*collRushObj_[ i ]->Draw();
			collRushObjL_[ i ]->Draw();*/
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
	if ( isBackEntryFlag_[ 0 ] == 0 )
	{
		if ( isGameStartTimer >= 220 )
		{
			Obj_[ 0 ]->wtf.position.z += 0.5f;
		}
		if ( Obj_[ 0 ]->wtf.position.z >= 2.0f )
		{
			Obj_[0]->wtf.position.z = 2.0f;
			isBackEntryFlag_[ 0 ] = 1;
		}
	}
	if ( isBackEntryFlag_[1] == 0 )
	{
		if ( isGameStartTimer >= 220 )
		{
			Obj_[1]->wtf.position.z += 0.5f;
		}
		if ( Obj_[1]->wtf.position.z >= 7.0f )
		{
			Obj_[1]->wtf.position.z = 7.0f;
			isBackEntryFlag_[1] = 1;
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
				if ( Obj_[ 2 ]->wtf.position.z >= 3.0f )
				{
					Obj_[ 2 ]->wtf.position.z = 3.0f;
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
				if ( Obj_[ 3 ]->wtf.position.z >= 3.0f )
				{
					Obj_[ 3 ]->wtf.position.z = 3.0f;
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
				if ( Obj_[ 5 ]->wtf.position.z >= 8.0f )
				{
					Obj_[ 5 ]->wtf.position.z = 8.0f;
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
				if ( Obj_[ 6 ]->wtf.position.z >= 8.0f )
				{
					Obj_[ 6 ]->wtf.position.z = 8.0f;
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
				if ( Obj_[7]->wtf.position.z >= 3.0f )
				{
					Obj_[7 ]->wtf.position.z = 3.0f;
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
				if ( Obj_[8]->wtf.position.z >= 8.0f )
				{
					Obj_[ 8 ]->wtf.position.z = 8.0f;
					isBackEntryFlag_[ 8 ] = 1;
				}
			}
		}
	}

}

void BikeEnemy::BikeEnemyAction()
{
	//バイク兵が自機より後ろにいるときカメラを少し下げる
	for ( int i = 0; i < 9; i++ )
	{
		if ( isBackEntryFlag_[ i ] == 1 && isBikclushFlag_[ i ] == 0 )
		{
			if ( standardPos == i )
			{
				if ( player_->standardCamera == 0 )
				{
					if ( Obj_[ i ]->wtf.position.z <= -3.0f )
					{
						player_->standardCamera = 1;
					}
				
				}

				if ( player_->standardCamera == 1 )
				{
					if ( Obj_[ i ]->wtf.position.z >= 3.0f )
					{
						player_->standardCamera = 0;
					}
				}
			}
		}
		//移動制限
		if ( isBackEntryFlag_[ i ] == 1 && isBikclushFlag_[ i ] == 0 )
		{
			if ( Obj_[ i ]->wtf.position.z <= -8.0f )
			{
				Obj_[ i ]->wtf.position.z = -8.0f;
			}
			if ( Obj_[ i ]->wtf.position.z >= 10.0f )
			{
				Obj_[ i ]->wtf.position.z = 10.0f;
			}
		}
		//ウェーブの一番自機に近いやつがカメラの主導権を握る
		if ( isBikclushFlag_[ i ] == 1 )
		{
			standardPos = i + 1;
			player_->standardCamera = 0;
		}
	}
	

}

void BikeEnemy::BiketoBikeColl()
{
	//////////////////////////////////////
	//--当たり判定(バイク兵同士の当たり判定)--//
	/////////////////////////////////////
	
	//ウェーブ1のバイク兵同士の当たり判定
	if ( isBackEntryFlag_[0] == 1 && isBikclushFlag_[0] == 0 && isBackEntryFlag_[ 1 ] == 1 && isBikclushFlag_[ 1 ] == 0 ){
		if ( HP_[0] >= 1 && HP_[ 1 ] >= 1 ){
			if ( isEachKnockbackFlag_ == 0){
				if ( coll.CircleCollision(collFrontObj_[ 0 ]->wtf.position,collBackObj_[ 1 ]->wtf.position,0.6f,0.6f) ){
					isEachKnockbackFlag_ = 1;
				}
			}
		}
	}
	if ( isEachKnockbackFlag_ == 1 ){eachKnockbackTimer_++;}
	if ( eachKnockbackTimer_ >= 1 && eachKnockbackTimer_ <= 20){
		Obj_[ 1 ]->wtf.position.z += 0.15f;
		Obj_[ 0 ]->wtf.position.z -= 0.15f;
	}
	if ( eachKnockbackTimer_ >= 21 )
	{
		isEachKnockbackFlag_ = 0;
		eachKnockbackTimer_ = 0;
	}
}
