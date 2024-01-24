#include "BikeEnemy.h"
#include "Player.h"
#include "ornament/Block.h"
#include <imgui.h>

BikeEnemy::BikeEnemy()
{
}

BikeEnemy::~BikeEnemy()
{
	delete spriteCommon;
	//バイクのモデル
	for ( int i = 0; i < 9; i++ )
	{
		delete Obj_[ i ];
		delete Model_[ i ];
		delete Model2_[ i ];
		delete collObj_[ i ];
		delete collModel_[ i ];
		delete bikclushObj_[ i ];
		delete bikclushModel_[ i ];
		delete collRightObj_[ i ];
		delete collLeftObj_[ i ];
		delete collLRModel_[ i ];
		delete collFrontObj_[ i ];
		delete collBackObj_[ i ];

	}
	//攻撃範囲
	for ( int i = 0; i < 5; i++ )
	{
		delete collRushObj_[ i ];
		delete collRushObjL_[ i ];
		delete collRushModel_[ i ];
	}

	//木箱の当たり判定モデル
	delete collBlockModel_;
	for ( int i = 0; i < 12; i++ )
	{
		delete collBoxObj_[i];
	}
	//コーンの当たり判定モデル
	for ( int i = 0; i < 2; i++ )
	{
		delete collKonObj_[ i ];
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
	Obj_[ 0 ]->wtf.position = { -3.0f,-2.0f,-30.0f };
	Obj_[ 1 ]->wtf.position = { 3.0f,-2.0f,-20.0f };
	//ラウンド2
	Obj_[ 2 ]->wtf.position = { -3.0f,-2.0f,-20.0f };
	Obj_[ 3 ]->wtf.position = { 3.0f,-2.0f,-20.0f };
	//ラウンド3
	Obj_[ 4 ]->wtf.position = { -3.0f,-2.0f,-20.0f };
	Obj_[ 5 ]->wtf.position = { 3.0f,-2.0f,-20.0f };
	Obj_[ 6 ]->wtf.position = { 5.0f,-2.0f,-20.0f };
	//ラウンド4
	Obj_[ 7 ]->wtf.position = { -6.0f,-2.0f,-20.0f };
	Obj_[ 8 ]->wtf.position = { -3.0f,-2.0f,-20.0f };

	//当たり判定のモデル(本体)
	for ( int i = 0; i < 9; i++ )
	{
		collModel_[ i ] = Model::LoadFromOBJ("collboll");
		collObj_[ i ] = Object3d::Create();
		collObj_[ i ]->SetModel(collModel_[ i ]);
		collObj_[ i ]->wtf.position = { Obj_[ i ]->wtf.position.x,Obj_[ i ]->wtf.position.y,Obj_[ i ]->wtf.position.z - 1.0f };
		//ここでHP設定
		HP_[ i ] = 1;
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
		bikclushObj_[ i ] = Object3d::Create();
		bikclushObj_[ i ]->SetModel(bikclushModel_[ i ]);
		bikclushObj_[ i ]->wtf.scale = { 0.4f,0.4f,0.4f };
		bikclushObj_[ i ]->wtf.position = { Obj_[ i ]->wtf.position.x,Obj_[ i ]->wtf.position.y,Obj_[ i ]->wtf.position.z };
	}

	//パーティクル生成
	for ( int i = 0; i < 9; i++ )
	{
		//ガス
		gasParticle_[ i ] = std::make_unique<ParticleManager>();
		gasParticle_[ i ].get()->Initialize();
		gasParticle_[ i ]->LoadTexture("gas.png");
		gasParticle_[ i ]->Update();
		//火花(左側)
		DamageLeftParticle_[ i ] = std::make_unique<ParticleManager>();
		DamageLeftParticle_[ i ].get()->Initialize();
		DamageLeftParticle_[ i ]->LoadTexture("fire.png");
		DamageLeftParticle_[ i ]->Update();
		//火花(右側)
		DamageRightParticle_[ i ] = std::make_unique<ParticleManager>();
		DamageRightParticle_[ i ].get()->Initialize();
		DamageRightParticle_[ i ]->LoadTexture("fire.png");
		DamageRightParticle_[ i ]->Update();


		//煙(体力半分)
		smokeParticle_[ i ] = std::make_unique<ParticleManager>();
		smokeParticle_[ i ].get()->Initialize();
		smokeParticle_[ i ]->LoadTexture("gas.png");
		smokeParticle_[ i ]->Update();

		//煙(体力ミリ)
		blazeParticle_[ i ] = std::make_unique<ParticleManager>();
		blazeParticle_[ i ].get()->Initialize();
		blazeParticle_[ i ]->LoadTexture("blaze.png");
		blazeParticle_[ i ]->Update();

		blazeSmokeParticle_[ i ] = std::make_unique<ParticleManager>();
		blazeSmokeParticle_[ i ].get()->Initialize();
		blazeSmokeParticle_[ i ]->LoadTexture("gas.png");
		blazeSmokeParticle_[ i ]->Update();

		//やられた時の炎
		heatParticle_[ i ] = std::make_unique<ParticleManager>();
		heatParticle_[ i ].get()->Initialize();
		heatParticle_[ i ]->LoadTexture("blaze.png");
		heatParticle_[ i ]->Update();

	}

	//障害物用の当たり判定モデル
	collBlockModel_ = Model::LoadFromOBJ("collboll");
	for ( int i = 0; i < 12; i++ )
	{
		collBoxObj_[ i ] = Object3d::Create();
		collBoxObj_[ i ]->SetModel(collBlockModel_);
		collBoxObj_[ i ]->wtf.scale = { 0.4f,0.4f,0.4f };
	}
	for ( int i = 0; i < 2; i++ )
	{
		collKonObj_[ i ] = Object3d::Create();
		collKonObj_[ i ]->SetModel(collBlockModel_);
		collKonObj_[ i ]->wtf.scale = { 0.4f,0.4f,0.4f };
	}

}

void BikeEnemy::Update(Vector3 playerSWPos,bool isCollSWFlag,Vector3 playerSWRightPos,bool isCollSWRightFlag,Vector3 playerRPos,Vector3 playerLPos,Vector3 BlockPos)
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
	for ( int i = 0; i < 5; i++ )
	{
		collRushObj_[ i ]->Update();
		collRushObj_[ i ]->wtf.position = { playerRPos.x + i + 0.5f,playerRPos.y - 0.5f,playerRPos.z };
		collRushObjL_[ i ]->Update();
		collRushObjL_[ i ]->wtf.position = { playerRPos.x - i + 0.5f,playerRPos.y - 0.5f,playerRPos.z };
	}
	//木箱用当たり判定モデル
	for ( int i = 0; i < 12; i++ )
	{
		collBoxObj_[ i ]->Update();
		collBoxObj_[i]->wtf.position = { BlockPos };
	}
	collBoxObj_[ 1 ]->wtf.position = { collBoxObj_[0]->wtf.position.x + 0.3f, collBoxObj_[ 0 ]->wtf.position.y-0.5f, collBoxObj_[ 0 ]->wtf.position.z};
	collBoxObj_[ 2 ]->wtf.position = { collBoxObj_[0]->wtf.position.x - 0.3f, collBoxObj_[ 0 ]->wtf.position.y - 0.5f, collBoxObj_[ 0 ]->wtf.position.z };

	collBoxObj_[3]->wtf.position = { collBoxObj_[ 0 ]->wtf.position.x + 1.5f, collBoxObj_[ 0 ]->wtf.position.y, collBoxObj_[ 0 ]->wtf.position.z + 100.0f};
	collBoxObj_[4]->wtf.position = { collBoxObj_[3]->wtf.position.x + 0.3f, collBoxObj_[3]->wtf.position.y - 0.5f, collBoxObj_[3]->wtf.position.z };
	collBoxObj_[5]->wtf.position = { collBoxObj_[3]->wtf.position.x - 0.3f, collBoxObj_[3]->wtf.position.y - 0.5f, collBoxObj_[3]->wtf.position.z };

	collBoxObj_[6]->wtf.position = { collBoxObj_[ 0 ]->wtf.position.x + 1.5f, collBoxObj_[ 0 ]->wtf.position.y, collBoxObj_[ 0 ]->wtf.position.z + 100.0f };

	//コーン用当たり判定モデル
	for ( int i = 0; i < 2; i++ )
	{
		collKonObj_[ i ]->Update();
	}
	collKonObj_[0]->wtf.position = { BlockPos.x + 0.5f,BlockPos.y - 0.5f,BlockPos.z - 1.0f };
	collKonObj_[1]->wtf.position = { BlockPos.x - 0.5f,BlockPos.y - 0.5f,BlockPos.z - 1.0f };

	EffUpdate();
	isGameStartTimer++;

	//バイクの車輪が動き出す
	bikstSpinTimer++;
	if ( bikstSpinTimer > 10.0f )
	{
		bikstSpinTimer = 0;
	}
	for ( int i = 0; i < 9; i++ )
	{
		const float SpinAdjust = 1.0f;
		const float SpinMax = 5.0f;
		const float SpinAdjust2 = 6.0f;
		const float SpinMax2 = 10.0f;
		if ( bikstSpinTimer >= SpinAdjust && bikstSpinTimer <= SpinMax )
		{
			Obj_[ i ]->SetModel(Model_[ i ]);
		}
		else if ( bikstSpinTimer >= SpinAdjust2 && bikstSpinTimer <= SpinMax2 )
		{
			Obj_[ i ]->SetModel(Model2_[ i ]);
		}
	}

	//バイク兵のエントリー
	BikeEnemyEntry();

	//エントリー後のバイク兵のアクション
	for ( int i = 0; i < 9; i++ )
	{
		if ( isBackEntryFlag_[ i ] == 1 )
		{
			actionTimer_[ i ]++;
		}
	}
	//バイク兵の挙動と自機より後ろにいるときカメラを少し下げる
	BikeEnemyAction();

	if ( input_->PushKey(DIK_H))
	{
		Obj_[ 2 ]->wtf.position.x -= 0.05f;
	}
	if ( input_->PushKey(DIK_K) )
	{
		Obj_[ 2 ]->wtf.position.x += 0.05f;
	}

	//突進攻撃の当たり判定
	for ( int i = 0; i < 5; i++ )
	{
		for ( int j = 0; j < 9; j++ )
		{
			//突進攻撃の当たり判定(右の敵と右側の自機の横並びモデル)
			if ( isBackEntryFlag_[ j ] == 1 && HP_[ j ] >= 1 && isRushFlag_[ j ] == 0 )
			{
				if ( coll.CircleCollision(collRushObj_[ i ]->wtf.position,collObj_[ j ]->wtf.position,0.5f,0.5f) )
				{
					isRushFlag_[ j ] = 1;
				}
			}
			//突進攻撃の当たり判定(左の敵と左側の自機の横並びモデル)
			if ( isBackEntryFlag_[ j ] == 1 && HP_[ j ] >= 1 && isRushFlag_[ j ] == 0 )
			{
				if ( coll.CircleCollision(collRushObjL_[ i ]->wtf.position,collObj_[ j ]->wtf.position,0.5f,0.5f) )
				{
					isRushFlag_[ j ] = 4;
				}
			}
		}
	}
	//突進攻撃の当たり判定(右の敵と自機)
	for ( int i = 0; i < 9; i++ )
	{
		if ( isRushFlag_[ i ] == 1 )
		{
			const float bikeSpeed = 0.08f;
			const float posLimit = -3.0f;
			const float rotaSpeed = 0.03f;
			const float rotaLimit = 0.45f;
			const float damage = 10.0f;
			Obj_[ i ]->wtf.position.x -= bikeSpeed;
			if ( Obj_[ i ]->wtf.position.x <= posLimit )
			{
				Obj_[ i ]->wtf.position.x = posLimit;
				isRushFlag_[ i ] = 3;
			}
			Obj_[ i ]->wtf.rotation.z += rotaSpeed;
			if ( Obj_[ i ]->wtf.rotation.z >= rotaLimit )
			{
				Obj_[ i ]->wtf.rotation.z = rotaLimit;
			}
			//突進中に自機と当たったらフラグ切り替え
			if ( coll.CircleCollision(playerRPos,collLeftObj_[ i ]->wtf.position,0.6f,0.6f) )
			{
				isRushKnockbackFlag_[ i ] = 1;
				player_->isKnockbackFlag = true;
				player_->isCamShake = 1;
				player_->camShakeTimer = player_->camShakeLimit;
				player_->hpgreenPosition.x -= damage;//倍ダメ
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
			const float posSpeed = 0.03f;
			const float posLimit = 4.0f;
			const float rotaSpeed = 0.03f;
			const float rotaLimit = 0.0f;
			Obj_[ i ]->wtf.rotation.z -= rotaSpeed;
			if ( Obj_[ i ]->wtf.rotation.z <= rotaLimit )
			{
				Obj_[ i ]->wtf.rotation.z = rotaLimit;
			}
			Obj_[ i ]->wtf.position.x += posSpeed;
			if ( Obj_[ i ]->wtf.position.x >= posLimit )
			{
				Obj_[ i ]->wtf.position.x = posLimit;
				isRushFlag_[ i ] = 3;
				isRushKnockbackFlag_[ i ] = 0;
				rushKnockbackTimer_[ i ] = 0;
			}
		}
		//突進後クールタイム
		if ( isRushFlag_[ i ] == 3 )
		{
			const float rotaSpeed = 0.03f;
			const float rotaLimit = 0.0f;
			Obj_[ i ]->wtf.rotation.z -= rotaSpeed;
			if ( Obj_[ i ]->wtf.rotation.z <= rotaLimit )
			{
				Obj_[ i ]->wtf.rotation.z = rotaLimit;
			}
			rushCoolTimer_[ i ]++;
		}

		//突進攻撃の当たり判定(左の敵と自機)
		if ( isRushFlag_[ i ] == 4 )
		{
			const float bikeSpeed = 0.08f;
			const float posLimit = 3.0f;
			const float rotaSpeed = 0.03f;
			const float rotaLimit = -0.45f;
			const float damage = 10.0f;
			Obj_[ i ]->wtf.position.x += bikeSpeed;
			if ( Obj_[ i ]->wtf.position.x >= posLimit )
			{
				Obj_[ i ]->wtf.position.x = posLimit;
				isRushFlag_[ i ] = 5;
			}
			Obj_[ i ]->wtf.rotation.z -= rotaSpeed;
			if ( Obj_[ i ]->wtf.rotation.z <= rotaLimit )
			{
				Obj_[ i ]->wtf.rotation.z = rotaLimit;
			}
			//突進中に自機と当たったらフラグ切り替え
			if ( coll.CircleCollision(playerLPos,collRightObj_[ i ]->wtf.position,0.6f,0.6f) )
			{
				isRushKnockbackFlag_[ i ] = 2;
				player_->isKnockbackFlagL = true;
				player_->isCamShake = 1;
				player_->camShakeTimer = player_->camShakeLimit;
				player_->hpgreenPosition.x -= damage;//倍ダメ
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
			const float posSpeed = 0.03f;
			const float posLimit = -4.0f;
			const float rotaSpeed = 0.03f;
			const float rotaLimit = 0.0f;
			Obj_[ i ]->wtf.rotation.z += rotaSpeed;
			if ( Obj_[ i ]->wtf.rotation.z >= rotaLimit )
			{
				Obj_[ i ]->wtf.rotation.z = rotaLimit;
			}
			Obj_[ i ]->wtf.position.x -= posSpeed;
			if ( Obj_[ i ]->wtf.position.x <= posLimit )
			{
				Obj_[ i ]->wtf.position.x = posLimit;
				isRushFlag_[ i ] = 5;
				isRushKnockbackFlag_[ i ] = 0;
				rushKnockbackTimerR_[ i ] = 0;
			}
		}
		//突進後クールタイム
		if ( isRushFlag_[ i ] == 5 )
		{
			const float rotaSpeed = 0.03f;
			const float rotaLimit = 0.0f;
			

			Obj_[ i ]->wtf.rotation.z += rotaSpeed;
			if ( Obj_[ i ]->wtf.rotation.z >= rotaLimit )
			{
				Obj_[ i ]->wtf.rotation.z = rotaLimit;
			}
			rushCoolTimer_[ i ]++;
		}

		const float rushLimit = 200.0f;
		if ( rushCoolTimer_[ i ] >= rushLimit )
		{
			isRushFlag_[ i ] = 0;
			rushCoolTimer_[ i ] = 0;
		}
	}

	//自機の加減速でバイク兵のZ軸移動
	for ( int i = 0; i < 9; i++ )
	{
		if ( isBackEntryFlag_[ i ] == 1 )
		{
			const float posSpeed = 0.06f;
			const float posSpeed2 = 0.07f;
			const float timerAdjust = 1.0f;
			const float timerMax = 29.0f;

			if ( input_->PushKey(DIK_W) || input_->StickInput(L_UP) )
			{
				Obj_[ i ]->wtf.position.z -= posSpeed;
			}
			else if ( input_->PushKey(DIK_S) || input_->StickInput(L_DOWN) )
			{
				Obj_[ i ]->wtf.position.z += posSpeed;
			}
			//自機が障害物と当たった時
			if ( player_->backTimer >= timerAdjust && player_->backTimer <= timerMax )
			{
				Obj_[ i ]->wtf.position.z += posSpeed2;
			}

		}
	}

	//当たり判定(自機とバイク兵の当たり判定)
	for ( int i = 0; i < 9; i++ )
	{
		//自機の左攻撃の当たり判定
		if ( HP_[ i ] >= 1 )
		{
			if ( isCollSWFlag == true )
			{
				if ( coll.CircleCollision(playerSWPos,collObj_[ i ]->wtf.position,0.6f,0.6f) )
				{
					HP_[ i ]--;
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
				isHit_[ i ]++;
				isDamageRightEffFlag_[ i ] = 1;
			}
		}
		//左にいるバイク兵のノックバック
		if ( limitRightmove_[ i ] == 1 )
		{
			knockbackTimer2_[ i ]++;
		}
		const float KbAdjust = 1.0f;
		const float KbMax = 10.0f;
		if ( knockbackTimer2_[ i ] >= KbAdjust && knockbackTimer2_[ i ] <= KbMax )
		{
			//右側にノックバック
			const float posSpeed = 0.04f;
			Obj_[ i ]->wtf.position.x -= posSpeed;
		}
		const float Kb2Over = 10.0f;
		if ( knockbackTimer2_[ i ] >= Kb2Over )
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
				isHit_[ i ]++;
				isDamageLeftEffFlag_[i] = 1;
			}
		}
		//右にいるバイク兵のノックバック
		if ( limitLeftmove_[ i ] == 1 )
		{
			knockbackTimer_[ i ]++;
		}
		if ( knockbackTimer_[ i ] >= 1 && knockbackTimer_[ i ] <= 10 )
		{
			//右側にノックバック
			const float posSpeed = 0.04f;
			Obj_[ i ]->wtf.position.x += posSpeed;
		}
		const float KbOver = 11.0f;
		if ( knockbackTimer_[ i ] >= KbOver )
		{
			player_->limitmove2 = false;
			limitLeftmove_[ i ] = 0;
			knockbackTimer_[ i ] = 0;
		}



	}

	//当たり判定(バイク兵同士の当たり判定)
	BiketoBikeColl();

	//バイクと障害物の当たり判定
	//木箱とバイク兵
	for ( int i = 0; i < 9; i++ )
	{
		if ( HP_[ i ] >= 1 )
		{
			for ( int j = 0; j < 3; j++ )
			{
				if ( coll.CircleCollision(collBoxObj_[j]->wtf.position,collFrontObj_[ i ]->wtf.position,0.6f,0.6f) )
				{
					isBoxFlag_[j] = 1;
				}
			}

			for ( int k = 0; k < 2; k++ )
			{
				if ( coll.CircleCollision(collKonObj_[k]->wtf.position,collFrontObj_[ i ]->wtf.position,0.6f,0.6f) )
				{
					isKonFlag_[k] = 1;
				}
			}

		}
	}


	//バイク兵のガス噴射
	for ( int i = 0; i < 9; i++ )
	{
		if ( HP_[ i ] <= 0 )
		{
			isBikclushFlag_[ i ] = 1;
			isbulletEffFlag_[ i ] = 0;
			bulletEffTimer_[ i ] = 0;
		}
		else
		{
			isbulletEffFlag_[ i ] = 1;
		}


		//残り体力によって出るパーティクルの変化
		if ( HP_[ i ] <= 0 )
		{
			isHeatEffFlag_[ i ] = 1;
		}


		if ( isHit_[ i ] >= 5 && isHit_[ i ] <= 10 )
		{
			isSmokeEffFlag_[ i ] = 1;
		}
		else if ( isHit_[ i ] >= 11 && isHit_[ i ] <= 19 )
		{
			isBlazeEffFlag_[ i ] = 1;
		}

	}

	//バイクが前進しながら倒れる
	for ( int i = 0; i < 9; i++ )
	{
		if ( isBackEntryFlag_[ i ] == 1 )
		{
			if ( isBikclushFlag_[ i ] == 1 )
			{
				const float posSpeed = 0.5f;
				const float rotaSpeed = 0.1f;
				
				Obj_[ i ]->wtf.position.z += posSpeed;
				bikclushObj_[ i ]->wtf.position.z += posSpeed;
				bikclushObj_[ i ]->wtf.rotation.z += rotaSpeed;
				deathTimer_[i]++;
			}
			const float rotaLimit = 1.5f;
			if ( bikclushObj_[ i ]->wtf.rotation.z >= rotaLimit )
			{
				bikclushObj_[ i ]->wtf.rotation.z = rotaLimit;
				isBikSpinFlag_[ i ] = 1;
			}
			//バイクが倒れて後ろに行く
			if ( isBikSpinFlag_[ i ] == 1 )
			{
				const float posSpeed = 0.8f;
				const float rotaSpeed = 0.1f;
				Obj_[ i ]->wtf.position.z -= posSpeed;
				bikclushObj_[ i ]->wtf.position.z -= posSpeed;
				bikclushObj_[ i ]->wtf.rotation.x -= rotaSpeed;
			}
		}
	}

	//デスタイマーの起動
	for ( int i = 0; i < 9; i++ )
	{
		if ( deathTimer_[ i ] == 1)
		{
			player_->isDeadEnemy++;
		}
		if ( deathTimer_[ i ] >=2 )
		{
			deathTimer_[ i ] = 2;
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
	ImGui::Text("isHit_[0]:%d",isHit_[0]);
	ImGui::Text("position_:%f,%f,%f",Obj_[ 1 ]->wtf.position.x,Obj_[ 1 ]->wtf.position.y,Obj_[ 1 ]->wtf.position.z);
	ImGui::End();
	/*ImGui::Text("HP:%d",HP_[ 0 ]);
	ImGui::Text("isHit_:%d",isHit_[1]);
	ImGui::Text("isBackEntryFlag:%d",isBackEntryFlag_[1]);*/



}

void BikeEnemy::Draw()
{
	if ( isGameStartTimer >= 200 )
	{
		for ( int i = 0; i < 9; i++ )
		{
			//バイク兵のモデル
			if ( isBikclushFlag_[ i ] == 0 )
			{
				Obj_[ i ]->Draw();
			/*	collObj_[i]->Draw();
				collRightObj_[ i ]->Draw();
				collLeftObj_[ i ]->Draw();*/
				collFrontObj_[ i ]->Draw();
				/*collBackObj_[ i ]->Draw(); */

			}

			//バイク兵の撃破モデル
			if ( isBikclushFlag_[ i ] == 1 )
			{
				if ( isBackEntryFlag_[ i ] == 1 )
				{
					if ( bikclushObj_[ i ]->wtf.position.z >= -20.0f )
					{
						bikclushObj_[ i ]->Draw();
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

		for ( int i = 0; i < 3; i++ )
		{
			collBoxObj_[ i ]->Draw();
		}

		//コーンモデル
		for ( int i = 0; i < 2; i++ )
		{
			collKonObj_[ i ]->Draw();
		}

	}

}

void BikeEnemy::EffUpdate()
{
	for ( int i = 0; i < 9; i++ )
	{
		//ガス
		if ( isbulletEffFlag_[ i ] == 1 )
		{
			bulletEffTimer_[ i ]++;
		}
		if ( bulletEffTimer_[ i ] <= 20 && bulletEffTimer_[ i ] >= 1 )
		{
			EffSummary(Vector3(Obj_[ i ]->wtf.position.x,Obj_[ i ]->wtf.position.y - 0.2f,Obj_[ i ]->wtf.position.z - 1.3f),i);
		}
		if ( bulletEffTimer_[ i ] >= 20 )
		{
			isbulletEffFlag_[ i ] = 0;
			bulletEffTimer_[ i ] = 0;
		}

		//火花(左側)
		if ( isDamageLeftEffFlag_[ i ] == 1 )
		{
			DamageLeftEffTimer_[ i ]++;
		}
		if ( DamageLeftEffTimer_[ i ] <= 10 && DamageLeftEffTimer_[ i ] >= 0 )
		{
			DamageLeftSummary(Vector3(collLeftObj_[ i ]->wtf.position.x - 0.1f,collLeftObj_[ i ]->wtf.position.y + 0.6f,collLeftObj_[ i ]->wtf.position.z - 0.2f),i);
		}
		if ( DamageLeftEffTimer_[ i ] >= 10 )
		{
			isDamageLeftEffFlag_[ i ] = 0;
			DamageLeftEffTimer_[ i ] = 0;
		}
		//火花(右側)
		if ( isDamageRightEffFlag_[ i ] == 1 )
		{
			DamageRightEffTimer_[ i ]++;
		}
		if ( DamageRightEffTimer_[ i ] <= 10 && DamageRightEffTimer_[ i ] >= 0 )
		{
			DamageRightSummary(Vector3(collRightObj_[ i ]->wtf.position.x + 0.1f,collRightObj_[ i ]->wtf.position.y + 0.6f,collRightObj_[ i ]->wtf.position.z - 0.2f),i);
		}
		if (DamageRightEffTimer_[ i ] >= 10 )
		{
			isDamageRightEffFlag_[ i ] = 0;
			DamageRightEffTimer_[ i ] = 0;
		}

			//煙
		if ( isSmokeEffFlag_[ i ] == 1 )
		{
			smokeEffTimer_[ i ]++;
		}
		if ( smokeEffTimer_[ i ] <= 10 && smokeEffTimer_[ i ] >= 0 )
		{

			if ( isRushFlag_[ i ] == 1 )
			{
				//右に突進
				smokePosX_[ i ] = collRightObj_[ i ]->wtf.position.x - 0.4f;
			}
			else if ( isRushFlag_[ i ] == 4 )
			{
				//左に突進
				smokePosX_[ i ] = collLeftObj_[ i ]->wtf.position.x + 0.4f;
			}
			else
			{
				smokePosX_[ i ] = Obj_[ i ]->wtf.position.x;
			}

			DamageSmokeSummary(Vector3(smokePosX_[ i ],Obj_[ i ]->wtf.position.y + 0.7f,Obj_[ i ]->wtf.position.z - 1.0f),i);
		}
		if ( smokeEffTimer_[ i ] >= 10 )
		{
			isSmokeEffFlag_[ i ] = 0;
			smokeEffTimer_[ i ] = 0;
		}

		//煙(体力ミリ)
		if ( isBlazeEffFlag_[ i ] == 1 )
		{
			blazeEffTimer_[ i ]++;
		}
		if ( blazeEffTimer_[ i ] <= 10 && blazeEffTimer_[ i ] >= 0 )
		{

			if ( isRushFlag_[ i ] == 1 )
			{
				//右に突進
				blazePosX_[ i ] = collRightObj_[ i ]->wtf.position.x - 0.4f;
			}
			else if ( isRushFlag_[ i ] == 4 )
			{
				//左に突進
				blazePosX_[ i ] = collLeftObj_[ i ]->wtf.position.x + 0.4f;
			}
			else
			{
				blazePosX_[ i ] = Obj_[ i ]->wtf.position.x;
			}

			DamageBlazeSummary(Vector3(blazePosX_[ i ],Obj_[ i ]->wtf.position.y + 0.7f,Obj_[ i ]->wtf.position.z - 1.0f),i);
			DamageBlazeSmokeSummary(Vector3(blazePosX_[ i ],Obj_[ i ]->wtf.position.y + 0.7f,Obj_[ i ]->wtf.position.z - 1.0f),i);
		}
		if ( blazeEffTimer_[ i ] >= 10 )
		{
			isBlazeEffFlag_[ i ] = 0;
			blazeEffTimer_[ i ] = 0;
		}

		//やられた時の炎
		if ( isHeatEffFlag_[ i ] == 1 )
		{
			heatEffTimer_[ i ]++;
		}
		if ( heatEffTimer_[ i ] <= 10 && heatEffTimer_[ i ] >= 0 )
		{

			if ( isRushFlag_[ i ] == 1 )
			{
				//右に突進
				heatPosX_[ i ] = collRightObj_[ i ]->wtf.position.x - 0.4f;
			}
			else if ( isRushFlag_[ i ] == 4 )
			{
				//左に突進
				heatPosX_[ i ] = collLeftObj_[ i ]->wtf.position.x + 0.4f;
			}
			else
			{
				heatPosX_[ i ] = Obj_[ i ]->wtf.position.x;
			}

			DamageHeatSummary(Vector3(heatPosX_[ i ],Obj_[ i ]->wtf.position.y,Obj_[ i ]->wtf.position.z),i);
		}
		if ( heatEffTimer_[ i ] >= 10 )
		{
			isHeatEffFlag_[ i ] = 0;
			heatEffTimer_[ i ] = 0;
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
		const float rnd_velGz = 0.03f;
		Vector3 velG{};
		velG.x = ( float ) rand() / RAND_MAX * rnd_velG - rnd_velG / 2.0f;
		velG.y = ( float ) rand() / RAND_MAX * rnd_velGy - rnd_velGy / 2.0f;
		velG.z = ( float ) rand() / RAND_MAX * rnd_velGz - rnd_velGz / 2.5f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		const float rnd_accG = 0.000001f;
		Vector3 accG{};
		accG.x = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;
		accG.y = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;

		//追加
		gasParticle_[ num ]->Add(60,posG,velG,accG,0.1f,0.0f);

		gasParticle_[ num ]->Update();

	}
}

void BikeEnemy::DamageLeftSummary(Vector3 firepos,int num)
{
	//パーティクル範囲
	for ( int i = 0; i < 5; i++ )
	{
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_pos = 0.0f;
		const float rnd_posy = 0.0f;
		const float rnd_posz = 0.1f;
		Vector3 pos{};
		pos.x += ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y += ( float ) rand() / RAND_MAX * rnd_posy - rnd_posy / 2.0f;
		pos.z += ( float ) rand() / RAND_MAX * rnd_posz - rnd_posz / 2.0f;
		pos += firepos;
		//速
		//X,,Z全て[-0.05f,+0.05f]でランダムに分布
		const float rnd_vel = 0.0f;
		const float rnd_vely = 0.0f;
		const float rnd_velz = 0.1f;
		Vector3 vel{};
		vel.x = ( float ) rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = ( float ) rand() / RAND_MAX * rnd_vely - rnd_vely / 2.0f;
		vel.z = ( float ) rand() / RAND_MAX * rnd_velz - rnd_velz / 2.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		const float rnd_acc = 0.000001f;
		Vector3 acc{};
		acc.x = ( float ) rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;
		acc.y = ( float ) rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;

		//追加
		DamageLeftParticle_[num]->Add(60,pos,vel,acc,0.15f,0.0f);

		DamageLeftParticle_[num]->Update();

	}
}

void BikeEnemy::DamageRightSummary(Vector3 firepos,int num)
{
	//パーティクル範囲
	for ( int i = 0; i < 5; i++ )
	{
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_pos2 = 0.0f;
		const float rnd_pos2y = 0.0f;
		const float rnd_pos2z = 0.1f;
		Vector3 pos2{};
		pos2.x += ( float ) rand() / RAND_MAX * rnd_pos2 -  rnd_pos2 / 2.0f;
		pos2.y += ( float ) rand() / RAND_MAX * rnd_pos2y - rnd_pos2y / 2.0f;
		pos2.z += ( float ) rand() / RAND_MAX * rnd_pos2z - rnd_pos2z / 2.0f;
		pos2 += firepos;
		//速
		//X,,Z全て[-0.05f,+0.05f]でランダムに分布
		const float rnd_vel2 = 0.0f;
		const float rnd_vel2y = 0.0f;
		const float rnd_vel2z = 0.1f;
		Vector3 vel2{};
		vel2.x = ( float ) rand() / RAND_MAX * rnd_vel2 -  rnd_vel2 / 2.0f;
		vel2.y = ( float ) rand() / RAND_MAX * rnd_vel2y - rnd_vel2y / 2.0f;
		vel2.z = ( float ) rand() / RAND_MAX * rnd_vel2z - rnd_vel2z / 2.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		const float rnd_acc2 = 0.000001f;
		Vector3 acc2{};
		acc2.x = ( float ) rand() / RAND_MAX * rnd_acc2 - rnd_acc2 / 2.0f;
		acc2.y = ( float ) rand() / RAND_MAX * rnd_acc2 - rnd_acc2 / 2.0f;

		//追加
		DamageRightParticle_[ num ]->Add(60,pos2,vel2,acc2,0.15f,0.0f);

		DamageRightParticle_[ num ]->Update();

	}
}

void BikeEnemy::DamageSmokeSummary(Vector3 smokepos,int num)
{
	//パーティクル範囲
	for ( int i = 0; i < 10; i++ )
	{
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_posG  = 0.1f;
		const float rnd_posGy = 0.1f;
		const float rnd_posGz = 0.1f;
		Vector3 posG{};
		posG.x += ( float ) rand() / RAND_MAX * rnd_posG - rnd_posG / 2.0f;
		posG.y += ( float ) rand() / RAND_MAX * rnd_posGy - rnd_posGy / 2.0f;
		posG.z += ( float ) rand() / RAND_MAX * rnd_posGz - rnd_posGz / 2.0f;
		posG += smokepos;
		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		const float rnd_velG = 0.0f;
		const float rnd_velGy = -0.03f;
		const float rnd_velGz = 0.0f;
		Vector3 velG{};
		velG.x = ( float ) rand() / RAND_MAX * rnd_velG - rnd_velG / 2.0f;
		velG.y = ( float ) rand() / RAND_MAX * rnd_velGy - rnd_velGy / 1.0f;
		velG.z = ( float ) rand() / RAND_MAX * rnd_velGz - rnd_velGz / 2.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		const float rnd_accG = 0.000001f;
		Vector3 accG{};
		accG.x = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;
		accG.y = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;

		//追加
		smokeParticle_[num]->Add(60,posG,velG,accG,0.1f,0.0f);

		smokeParticle_[ num ]->Update();

	}
}

void BikeEnemy::DamageBlazeSummary(Vector3 blazepos,int num)
{
	//パーティクル範囲
	for ( int i = 0; i < 10; i++ )
	{
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_posG = 0.05f;
		const float rnd_posGy = 0.05f;
		const float rnd_posGz = 0.05f;
		Vector3 posG{};
		posG.x += ( float ) rand() / RAND_MAX * rnd_posG - rnd_posG / 2.0f;
		posG.y += ( float ) rand() / RAND_MAX * rnd_posGy - rnd_posGy / 2.0f;
		posG.z += ( float ) rand() / RAND_MAX * rnd_posGz - rnd_posGz / 2.0f;
		posG += blazepos;
		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		const float rnd_velG = 0.0f;
		const float rnd_velGy = -0.03f;
		const float rnd_velGz = 0.0f;
		Vector3 velG{};
		velG.x = ( float ) rand() / RAND_MAX * rnd_velG - rnd_velG / 2.0f;
		velG.y = ( float ) rand() / RAND_MAX * rnd_velGy - rnd_velGy / 1.0f;
		velG.z = ( float ) rand() / RAND_MAX * rnd_velGz - rnd_velGz / 2.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		const float rnd_accG = 0.000001f;
		Vector3 accG{};
		accG.x = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;
		accG.y = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;

		//追加
		blazeParticle_[ num ]->Add(60,posG,velG,accG,0.1f,0.0f);

		blazeParticle_[ num ]->Update();

	}
}

void BikeEnemy::DamageBlazeSmokeSummary(Vector3 blazesmokepos,int num)
{
	//パーティクル範囲
	for ( int i = 0; i < 10; i++ )
	{
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_posG = 0.1f;
		const float rnd_posGy = 0.1f;
		const float rnd_posGz = 0.1f;
		Vector3 posG{};
		posG.x += ( float ) rand() / RAND_MAX * rnd_posG - rnd_posG / 2.0f;
		posG.y += ( float ) rand() / RAND_MAX * rnd_posGy - rnd_posGy / 2.0f;
		posG.z += ( float ) rand() / RAND_MAX * rnd_posGz - rnd_posGz / 2.0f;
		posG += blazesmokepos;
		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		const float rnd_velG = 0.0f;
		const float rnd_velGy = -0.04f;
		const float rnd_velGz = 0.0f;
		Vector3 velG{};
		velG.x = ( float ) rand() / RAND_MAX * rnd_velG - rnd_velG / 2.0f;
		velG.y = ( float ) rand() / RAND_MAX * rnd_velGy - rnd_velGy / 1.0f;
		velG.z = ( float ) rand() / RAND_MAX * rnd_velGz - rnd_velGz / 2.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		const float rnd_accG = 0.000001f;
		Vector3 accG{};
		accG.x = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;
		accG.y = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;

		//追加
		blazeSmokeParticle_[ num ]->Add(60,posG,velG,accG,0.2f,0.0f);

		blazeSmokeParticle_[ num ]->Update();

	}

}

void BikeEnemy::DamageHeatSummary(Vector3 heatpos,int num)
{
	//パーティクル範囲
	for ( int i = 0; i < 10; i++ )
	{
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_posG = 0.3f;
		const float rnd_posGy = 0.3f;
		const float rnd_posGz = 0.3f;
		Vector3 posG{};
		posG.x += ( float ) rand() / RAND_MAX * rnd_posG - rnd_posG / 2.0f;
		posG.y += ( float ) rand() / RAND_MAX * rnd_posGy - rnd_posGy / 2.0f;
		posG.z += ( float ) rand() / RAND_MAX * rnd_posGz - rnd_posGz / 2.0f;
		posG += heatpos;
		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		const float rnd_velG = 0.0f;
		const float rnd_velGy = -0.07f;
		const float rnd_velGz = 0.0f;
		Vector3 velG{};
		velG.x = ( float ) rand() / RAND_MAX * rnd_velG - rnd_velG / 2.0f;
		velG.y = ( float ) rand() / RAND_MAX * rnd_velGy - rnd_velGy / 1.0f;
		velG.z = ( float ) rand() / RAND_MAX * rnd_velGz - rnd_velGz / 2.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		const float rnd_accG = 0.000001f;
		Vector3 accG{};
		accG.x = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;
		accG.y = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;

		//追加
		heatParticle_[ num ]->Add(60,posG,velG,accG,0.4f,0.0f);

		heatParticle_[ num ]->Update();

	}

}

void BikeEnemy::EffDraw()
{
	for ( int i = 0; i < 9; i++ )
	{
		if ( isGameStartTimer >= 200 )
		{
			if ( isBikclushFlag_[ i ] == 0 )
			{
				if ( isbulletEffFlag_[ i ] == 1 )
				{
					gasParticle_[ i ]->Draw();
				}
				
			}
			//火花(左側)
			if ( isDamageLeftEffFlag_[ i ] == 1 && DamageLeftEffTimer_[ i ] <= 10 && DamageLeftEffTimer_[ i ] >= 1 )
			{
				DamageLeftParticle_[ i ]->Draw();
			}
			//火花(右側)
			if ( isDamageRightEffFlag_[ i ] == 1 && DamageRightEffTimer_[ i ] <= 10 && DamageRightEffTimer_[ i ] >= 1 )
			{
				DamageRightParticle_[ i ]->Draw();
			}

				//煙(体力半分)
			if ( isSmokeEffFlag_[ i ] == 1 && smokeEffTimer_[ i ] <= 10 && smokeEffTimer_[ i ] >= 1 )
			{
				smokeParticle_[ i ]->Draw();
			}

			//煙(体力ミリ)
			if ( isBlazeEffFlag_[ i ] == 1 && blazeEffTimer_[ i ] <= 10 && blazeEffTimer_[ i ] >= 1 )
			{
				blazeSmokeParticle_[ i ]->Draw();
				blazeParticle_[ i ]->Draw();
			}

			//やられた時の炎
			if ( isHeatEffFlag_[ i ] == 1 && heatEffTimer_[ i ] <= 10 && heatEffTimer_[ i ] >= 1 )
			{
				heatParticle_[ i ]->Draw();
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
		if ( isGameStartTimer >= 220)
		{
			Obj_[ 0 ]->wtf.position.z += 0.5f;
		}
		if ( Obj_[ 0 ]->wtf.position.z >= 2.0f )
		{
			Obj_[ 0 ]->wtf.position.z = 2.0f;
			isBackEntryFlag_[ 0 ] = 1;
		}
	}
	if ( isBackEntryFlag_[ 1 ] == 0 )
	{
		if ( isGameStartTimer >= 220)
		{
			Obj_[ 1 ]->wtf.position.z += 0.5f;
		}
		if ( Obj_[ 1 ]->wtf.position.z >= 7.0f )
		{
			Obj_[ 1 ]->wtf.position.z = 7.0f;
			isBackEntryFlag_[ 1 ] = 1;
		}
	}

	//バイク兵のエントリー(ラウンド3)
	if ( player_->isRoundFlag == 3 )
	{
		AliveR2Timer++;
		if ( AliveR2Timer >= 5 )
		{
			if ( isBackEntryFlag_[ 2 ] == 0 )
			{
				if ( isGameStartTimer >= 280 )
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
				if ( isGameStartTimer >= 280 )
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

	//バイク兵のエントリー(ラウンド4)
	if ( player_->isRoundFlag == 5 )
	{
		AliveR3Timer++;
		if ( AliveR3Timer >= 5 )
		{
			if ( isBackEntryFlag_[ 4 ] == 0 )
			{
				if ( isGameStartTimer >= 280 )
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
				if ( isGameStartTimer >= 280 )
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
				if ( isGameStartTimer >= 280 )
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

	//バイク兵のエントリー(ラウンド5)
	if ( player_->isRoundFlag == 7 )
	{
		AliveR4Timer++;
		if ( AliveR4Timer >= 5 )
		{
			if ( isBackEntryFlag_[ 7 ] == 0 )
			{
				if ( isGameStartTimer >= 280 )
				{
					Obj_[ 7 ]->wtf.position.z += 0.5f;
				}
				if ( Obj_[ 7 ]->wtf.position.z >= 3.0f )
				{
					Obj_[ 7 ]->wtf.position.z = 3.0f;
					isBackEntryFlag_[ 7 ] = 1;
				}
			}
		}

		if ( AliveR4Timer >= 5 )
		{
			if ( isBackEntryFlag_[ 8 ] == 0 )
			{
				if ( isGameStartTimer >= 280 )
				{
					Obj_[ 8 ]->wtf.position.z += 0.5f;
				}
				if ( Obj_[ 8 ]->wtf.position.z >= 8.0f )
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
		
		//移動制限(上下)
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
		//カメラが寄ってる時の移動制限(左右)
		if ( player_->standardCamera == 0 )
		{
			if ( Obj_[ i ]->wtf.position.x <= -5.0f )
			{
				Obj_[ i ]->wtf.position.x = -5.0f;
			}
			if ( Obj_[ i ]->wtf.position.x >= 5.0f )
			{
				Obj_[ i ]->wtf.position.x = 5.0f;
			}
		}
		//カメラが離れている時の移動制限(左右)
		if ( player_->standardCamera == 0 )
		{
			if ( Obj_[ i ]->wtf.position.x <= -8.0f )
			{
				Obj_[ i ]->wtf.position.x = -8.0f;
			}
			if ( Obj_[ i ]->wtf.position.x >= 8.0f )
			{
				Obj_[ i ]->wtf.position.x = 8.0f;
			}
		}
		
	}


}

void BikeEnemy::BiketoBikeColl()
{
	//////////////////////////////////////
	//--当たり判定(バイク兵同士の当たり判定)--//
	/////////////////////////////////////

	//ウェーブ1のバイク兵同士の当たり判定
	if ( isBackEntryFlag_[ 0 ] == 1 && isBikclushFlag_[ 0 ] == 0 && isBackEntryFlag_[ 1 ] == 1 && isBikclushFlag_[ 1 ] == 0 )
	{
		if ( HP_[ 0 ] >= 1 && HP_[ 1 ] >= 1 )
		{
			if ( isEachKnockbackFlag_ == 0 )
			{
				if ( coll.CircleCollision(collFrontObj_[ 0 ]->wtf.position,collBackObj_[ 1 ]->wtf.position,0.6f,0.6f) )
				{
					isEachKnockbackFlag_ = 1;
				}
			}
		}
	}
	if ( isEachKnockbackFlag_ == 1 )
	{
		eachKnockbackTimer_++;
	}
	if ( eachKnockbackTimer_ >= 1 && eachKnockbackTimer_ <= 20 )
	{
		Obj_[ 1 ]->wtf.position.z += 0.15f;
		Obj_[ 0 ]->wtf.position.z -= 0.15f;
	}
	if ( eachKnockbackTimer_ >= 21 )
	{
		isEachKnockbackFlag_ = 0;
		eachKnockbackTimer_ = 0;
	}
}
