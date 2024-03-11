#include "ArmorEnemy.h"
#include <imgui.h>
#include "Player/Player.h"

ArmorEnemy::ArmorEnemy()
{
}

ArmorEnemy::~ArmorEnemy()
{
	delete spriteCommon;
	for ( int i = 0; i < 4; i++ )
	{
		delete Obj_[ i ];
		delete Model_[ i ];
		delete Modelst_[ i ];
		delete collObj_[ i ];
		delete collModel_[ i ];
		delete bulletObj_[ i ];
		delete bulletModel_[ i ];
	}
	delete collObj3_;

}

void ArmorEnemy::AEReset()
{
	/*isGameStartTimer = 0;
	isAliveFlag_[ 4 ] = {1};
	AliveR2Timer = 0;
	AliveR3Timer = 0;
	AliveR4Timer = 0;
	deathTimer_[ 4 ] = { 0 };
	isRotFlag_[ 4 ] = { 0 };
	RotTimer_[ 4 ] = { 0 };
	entryTimer = 0;
	isExpolFlag_[ 4 ] = { 0 };
	ExpolTimer_[ 4 ] = { 0 };
	polygon_[ 4 ];
	isMoveFlag_[ 4 ] = { 0 };
	isbesideFlag_[ 4 ] = { 0 };

	for ( int i = 0; i < 4; i++ )
	{
		HP_[i] = 12;
	}
	Obj_[ 0 ]->wtf.position = { 10.0f,-2.0f,-10.0f };
	Obj_[ 1 ]->wtf.position = { -10.0f,-2.0f,-10.0f };
	Obj_[ 2 ]->wtf.position = { 10.0f,-2.0f,-20.0f };
	Obj_[ 3 ]->wtf.position = { -10.0f,-2.0f,-20.0f };*/
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


	for ( int i = 0; i < 4; i++ )
	{
//雑魚敵(攻撃状態)
		Model_[ i ] = Model::LoadFromOBJ("armorenemy");
		//雑魚敵(待機状態)
		Modelst_[ i ] = Model::LoadFromOBJ("armorenemyst");

		Obj_[ i ] = Object3d::Create();
		Obj_[ i ]->SetModel(Modelst_[ i ]);
		Obj_[ i ]->wtf.scale = { 0.4f,0.4f,0.4f };
		Obj_[ i ]->wtf.rotation.y = -3.0f;
		//ポリゴン爆散の情報                 大きさ 回転  飛ぶ量 
		Obj_[ i ]->SetPolygonExplosion({ 0.0f,-1.0f,6.28f,20.0f });
	}
	Obj_[ 0 ]->wtf.position = { 10.0f,-2.0f,-10.0f };
	Obj_[ 1 ]->wtf.position = { -10.0f,-2.0f,-10.0f };
	Obj_[ 2 ]->wtf.position = { 10.0f,-2.0f,-20.0f };
	Obj_[ 3 ]->wtf.position = { -10.0f,-2.0f,-20.0f };

	//ここでHP設定
	HP_[ 0 ] = 12;
	HP_[ 1 ] = 12;
	HP_[ 2 ] = 12;
	HP_[ 3 ] = 12;

	//大砲の弾
	for ( int i = 0; i < 4; i++ )
	{
		const float addPosY = 2.5f;
		const float addPosZ = 3.0f;
		bulletModel_[ i ] = Model::LoadFromOBJ("eneboll");
		bulletObj_[ i ] = Object3d::Create();
		bulletObj_[ i ]->SetModel(bulletModel_[ i ]);
		bulletObj_[ i ]->wtf.position = { Obj_[ i ]->wtf.position.x,Obj_[ i ]->wtf.position.y + addPosY,Obj_[ i ]->wtf.position.z - addPosZ };
	}



	//当たり判定のモデル
	for ( int i = 0; i < 4; i++ )
	{
		const float addPosY = 1.0f;
		collModel_[ i ] = Model::LoadFromOBJ("collboll");
		collObj_[ i ] = Object3d::Create();
		collObj_[ i ]->SetModel(collModel_[ i ]);
		collObj_[ i ]->wtf.scale = { 2.0f,2.0f,2.0f };
		collObj_[ i ]->wtf.position = { Obj_[ i ]->wtf.position.x,Obj_[ i ]->wtf.position.y + addPosY,Obj_[ i ]->wtf.position.z };
	}

	//自機の当たり判定
	collObj3_ = Object3d::Create();
	collObj3_->SetModel(collModel_[ 0 ]);

	//パーティクル生成
	for ( int i = 0; i < 4; i++ )
	{
//エフェクトの情報(地面のズサ)
		gasParticle_[ i ] = std::make_unique<ParticleManager>();
		gasParticle_[ i ].get()->Initialize();
		gasParticle_[ i ]->LoadTexture("gas.png");
		gasParticle_[ i ]->Update();

		gasParticle2_[ i ] = std::make_unique<ParticleManager>();
		gasParticle2_[ i ].get()->Initialize();
		gasParticle2_[ i ]->LoadTexture("gas.png");
		gasParticle2_[ i ]->Update();

		//エフェクトの情報(背中の噴射ガス)
		gasParticle3_[ i ] = std::make_unique<ParticleManager>();
		gasParticle3_[ i ].get()->Initialize();
		gasParticle3_[ i ]->LoadTexture("fire.png");
		gasParticle3_[ i ]->Update();

		gasParticle4_[ i ] = std::make_unique<ParticleManager>();
		gasParticle4_[ i ].get()->Initialize();
		gasParticle4_[ i ]->LoadTexture("fire.png");
		gasParticle4_[ i ]->Update();

		//攻撃受けた時の火花のパーティクル
		DamageParticle_[ i ] = std::make_unique<ParticleManager>();
		DamageParticle_[ i ].get()->Initialize();
		DamageParticle_[ i ]->LoadTexture("fire.png");
		DamageParticle_[ i ]->Update();

		//攻撃受けた時の火花のパーティクル
		smokeParticle_[ i ] = std::make_unique<ParticleManager>();
		smokeParticle_[ i ].get()->Initialize();
		smokeParticle_[ i ]->LoadTexture("smo.png");
		smokeParticle_[ i ]->Update();

		//たいりょく半分以下の煙
		fumeParticle_[ i ] = std::make_unique<ParticleManager>();
		fumeParticle_[ i ].get()->Initialize();
		fumeParticle_[ i ]->LoadTexture("blaze.png");
		fumeParticle_[ i ]->Update();

		//発砲時の弾速エフェクト
		ballisticParticle_[ i ] = std::make_unique<ParticleManager>();
		ballisticParticle_[ i ].get()->Initialize();
		ballisticParticle_[ i ]->LoadTexture("gas.png");
		ballisticParticle_[ i ]->Update();

	}



}

void ArmorEnemy::Update(Vector3 playerPos,Vector3 playerBpos,Vector3 playerB2pos,Vector3 playerB3pos,bool playerShootFlag)
{
	for ( int i = 0; i < 4; i++ )
	{
		if ( isAliveFlag_[ i ] == 0 )
		{
			const float addPosY = 1.0f;
			Obj_[ i ]->Update();
			collObj_[ i ]->Update();
			collObj_[ i ]->wtf.position = { Obj_[ i ]->wtf.position.x,Obj_[ i ]->wtf.position.y + addPosY,Obj_[ i ]->wtf.position.z };
			bulletObj_[ i ]->Update();
		}
	}
	collObj3_->Update();
	collObj3_->wtf.position = { playerPos };
	EffUpdate();
	isGameStartTimer++;

	//魔導兵が一定距離進むとこっちを向く
	for ( int i = 0; i < 4; i++ )
	{
		if ( Obj_[ i ]->wtf.position.z >= 20.0f )
		{
			RotTimer_[ i ]++;
		}
		if ( RotTimer_[ i ] >= 1 )
		{
			const float rotaSpeed = 0.3f;
			const float rotaLimit = 0.0f;
			Obj_[ i ]->wtf.rotation.y += rotaSpeed;
			if ( Obj_[ i ]->wtf.rotation.y >= rotaLimit )
			{
				Obj_[ i ]->wtf.rotation.y = rotaLimit;
			}
		}
	}

	//順番にエントリーさせる
	if ( player_->isRoundFlag >= 3 )
	{
		entryTimer++;
	}

	//魔導兵が後ろから登場(ラウンド2)
	for ( int i = 0; i < 2; i++ )
	{
		if ( player_->isRoundFlag == 0 )
		{
			BulletCoolTime_[ i ] = 0;
		}
		if ( player_->isRoundFlag == 1 )
		{
			//ここで描画させるタイマーの起動
			AliveR2Timer++;
			if ( AliveR2Timer >= 1 && AliveR2Timer <= 2 )
			{
				isAliveFlag_[ i ] = 0;
			}

			//後ろから登場して左右移動
			if ( isAliveFlag_[ i ] == 0 )
			{
				if ( isMoveFlag_[ i ] == 0 )
				{
					if ( isGameStartTimer >= 200 )
					{
						const float posSpeed = 1.0f;
						Obj_[ i ]->wtf.position.z += posSpeed;
					}
					const float posMax = 20.0f;
					const float posAdjust = 15.0f;
					if ( Obj_[ i ]->wtf.position.z <= posMax && Obj_[ i ]->wtf.position.z >= posAdjust && player_->isRoundFlag == 1 )
					{
						BulletCoolTime_[ i ] = -50;
					}
					const float posMax2 = 35.0f;
					if ( Obj_[ i ]->wtf.position.z >= posMax2 )
					{
						Obj_[ i ]->wtf.position.z = posMax2;
						isMoveFlag_[ i ] = 1;
					}
				}
			}
		}
	}

	//魔導兵が後ろから登場(ラウンド3)
	if ( player_->isRoundFlag <= 2 )
	{
		BulletCoolTime_[ 2 ] = 0;
	}
	if ( player_->isRoundFlag == 3 )
	{
//ここで描画させるタイマーの起動
		AliveR3Timer++;
		if ( AliveR3Timer >= 1 && AliveR3Timer <= 2 )
		{
			isAliveFlag_[ 2 ] = 0;
		}


		//後ろから登場して左右移動
		if ( isAliveFlag_[ 2 ] == 0 )
		{
			if ( isMoveFlag_[ 2 ] == 0 )
			{
				const float posAdjust = 15.0f;
				const float posMax = 20.0f;
				if ( isGameStartTimer >= 200 )
				{
					const float posSpeed = 1.0f;
					Obj_[ 2 ]->wtf.position.z += posSpeed;
				}
				if ( Obj_[ 2 ]->wtf.position.z <= posMax && Obj_[ 2 ]->wtf.position.z >= posAdjust && player_->isRoundFlag == 3 )
				{
					BulletCoolTime_[ 2 ] = -50;
				}
				const float posReset = 35.0f;
				if ( Obj_[ 2 ]->wtf.position.z >= posReset )
				{
					Obj_[ 2 ]->wtf.position.z = posReset;
					isMoveFlag_[ 2 ] = 1;
				}
			}
		}
	}

	if ( entryTimer <= 399 )
	{
		BulletCoolTime_[ 3 ] = 0;
	}
	if ( entryTimer >= 400 )
	{
		//ここで描画させるタイマーの起動
		AliveR4Timer++;
		if ( AliveR4Timer >= 1 && AliveR4Timer <= 2 )
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
					const float posSpeed = 1.0f;
					Obj_[ 3 ]->wtf.position.z += posSpeed;
				}
				const float posAdjust = 15.0f;
				const float posMax = 20.0f;
				if ( Obj_[ 3 ]->wtf.position.z <= posMax && Obj_[ 3 ]->wtf.position.z >= posAdjust && player_->isRoundFlag == 3 )
				{
					BulletCoolTime_[ 3 ] = -50;
				}
				const float posLimit = 35.0f;
				if ( Obj_[ 3 ]->wtf.position.z >= posLimit )
				{
					Obj_[ 3 ]->wtf.position.z = posLimit;
					isMoveFlag_[ 3 ] = 1;
				}
			}
		}
	}

	//自機の加減速でバイク兵のZ軸移動
	for ( int i = 0; i < 4; i++ )
	{
		if ( isAliveFlag_[ i ] == 0 )
		{

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

		}

	}

	//魔導兵のガス噴射
	for ( int i = 0; i < 4; i++ )
	{
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

			if ( HP_[ i ] >= 1 && HP_[ i ] <= 6 )
			{
				isFumeEffFlag_[ i ] = 1;
			}
			else
			{
				isFumeEffFlag_[ i ] = 0;
			}


		}
	}



	//魔導兵の射撃
	for ( int i = 0; i < 4; i++ )
	{
		if ( isAliveFlag_[ i ] == 0 )
		{
			if ( isGameStartTimer >= 280 && isShootFlag_[ i ] == 0 )
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
			//攻撃時硝煙がでる
			if ( BulletCoolTime_[ i ] >= 215 && BulletCoolTime_[ i ] <= 218 )
			{
				isSmoEffFlag_[ i ] = 1;
			}

			//誘導弾
			if ( isShootFlag_[ i ] == 1 )
			{
				isballisticEffFlag_[ i ] = 1;
				BulletdurationTime_[ i ]++;

				bulletObj_[ i ]->wtf.position += playerlen_[ i ];
				bitweenlen_[ i ] = playerlen_[ i ];
				bitweenlen_[ i ] *= 0.1f;
			}
			else
			{
				bulletObj_[ i ]->wtf.position = { Obj_[ i ]->wtf.position.x,Obj_[ i ]->wtf.position.y + 2.5f,Obj_[ i ]->wtf.position.z - 3.0f };
				isballisticEffFlag_[ i ] = 0;
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
	for ( int i = 0; i < 4; i++ )
	{
		if ( isAliveFlag_[ i ] == 0 )
		{
			if ( HP_[ i ] <= 0 )
			{
				isExpolFlag_[ i ] = 1;
				deathTimer_[ i ]++;
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
	//デスタイマーの起動
	for ( int i = 0; i < 4; i++ )
	{
		if ( deathTimer_[ i ] == 1 )
		{
			player_->isDeadEnemy++;
		}
		if ( deathTimer_[ i ] >= 2 )
		{
			deathTimer_[ i ] = 2;
		}
	}

	//当たり判定(プレイヤー弾から魔導兵)
	if ( playerShootFlag == true )
	{
		for ( int i = 0; i < 4; i++ )
		{
			if ( isAliveFlag_[ i ] == 0 )
			{
				if ( coll.CircleCollision(playerBpos,collObj_[ i ]->wtf.position,1.0f,1.0f) )
				{
					HP_[ i ]--;
					isdamEffFlag_[ i ] = 1;
				}

				if ( coll.CircleCollision(playerB2pos,collObj_[ i ]->wtf.position,1.0f,1.0f) )
				{
					HP_[ i ]--;
					isdamEffFlag_[ i ] = 1;
				}

				if ( coll.CircleCollision(playerB3pos,collObj_[ i ]->wtf.position,1.0f,1.0f) )
				{
					HP_[ i ]--;
					isdamEffFlag_[ i ] = 1;
				}

			}
		}
	}

	//当たり判定(魔導兵弾からプレイヤー)
	for ( int i = 0; i < 4; i++ )
	{
		if ( isAliveFlag_[ i ] == 0 )
		{
			if ( coll.CircleCollision(bulletObj_[ i ]->wtf.position,collObj3_->wtf.position,0.6f,0.6f) )
			{
				isShootFlag_[ i ] = 0;
				player_->isCamShake = 1;
				player_->camShakeTimer = player_->camShakeLimit;
				const float damage = 1.0f;
				player_->playerHP -= damage;//倍ダメ
			}
		}
	}

	ImGui::Begin("ArmorEnemy");
	ImGui::Text("entryTimer:%d",entryTimer);
	ImGui::Text("position_:%f,%f,%f",Obj_[ 0 ]->wtf.rotation.x,Obj_[ 0 ]->wtf.rotation.y,Obj_[ 0 ]->wtf.rotation.z);
	ImGui::Text("isGameStartTimer:%d",isGameStartTimer);
	ImGui::Text("BulletCoolTime:%d",BulletCoolTime_[ 2 ]);
	ImGui::Text("isSmoEffFlag_:%d",isSmoEffFlag_);

	ImGui::End();

}

void ArmorEnemy::Draw()
{

	//collObj3_->Draw();
	if ( isGameStartTimer >= 200 )
	{
		for ( int i = 0; i < 4; i++ )
		{
//モデル描画
			if ( isAliveFlag_[ i ] == 0 )
			{
				Obj_[ i ]->Draw();
				/*collObj_[ i ]->Draw();*/
				//魔導兵弾描画
				if ( isShootFlag_[ i ] == 1 )
				{
					bulletObj_[ i ]->Draw();
				}
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
		if ( isgasEffFlag_[ i ] == 1 )
		{
			gasEffTimer_[ i ]++;
		}
		if ( gasEffTimer_[ i ] <= 20 && gasEffTimer_[ i ] >= 1 )
		{
			const float addPosX = 1.0f;
			const float addPosY = 1.5f;
			const float addGasPosX = 0.3f;
			const float addGasPosY = 1.8f;
			const float addGasPosZ = 2.0f;
			EffSummary(Vector3(Obj_[ i ]->wtf.position.x + addPosX,Obj_[ i ]->wtf.position.y - addPosY,Obj_[ i ]->wtf.position.z),i);
			EffSummary2(Vector3(Obj_[ i ]->wtf.position.x - addPosX,Obj_[ i ]->wtf.position.y - addPosY,Obj_[ i ]->wtf.position.z),i);
			EffSummary3(Vector3(Obj_[ i ]->wtf.position.x + addGasPosX,Obj_[ i ]->wtf.position.y + addGasPosY,Obj_[ i ]->wtf.position.z + addGasPosZ),i);
			EffSummary4(Vector3(Obj_[ i ]->wtf.position.x - addGasPosX,Obj_[ i ]->wtf.position.y + addGasPosY,Obj_[ i ]->wtf.position.z + addGasPosZ),i);
		}
		if ( gasEffTimer_[ i ] >= 20 )
		{
			isgasEffFlag_[ i ] = 0;
			gasEffTimer_[ i ] = 0;
		}

		//体力半分以下の煙
		if ( isFumeEffFlag_[ i ] == 1 )
		{
			fumeEffTimer_[ i ]++;
		}
		if ( fumeEffTimer_[ i ] <= 10 && fumeEffTimer_[ i ] >= 0 )
		{
			const float addPosX = 0.5f;
			const float addPosY = 2.5f;
			DamagefumeSummary(Vector3(Obj_[ i ]->wtf.position.x + addPosX,Obj_[ i ]->wtf.position.y + addPosY,Obj_[ i ]->wtf.position.z),i);
		}
		if ( fumeEffTimer_[ i ] >= 10 )
		{
			isFumeEffFlag_[ i ] = 0;
			fumeEffTimer_[ i ] = 0;
		}


		//ダメージ
		if ( isdamEffFlag_[ i ] == 1 )
		{
			damEffTimer_[ i ]++;
		}
		if ( damEffTimer_[ i ] <= 10 && damEffTimer_[ i ] >= 1 )
		{
			DamageSummary(Vector3(Obj_[ i ]->wtf.position.x,Obj_[ i ]->wtf.position.y,Obj_[ i ]->wtf.position.z),i);
		}
		if ( damEffTimer_[ i ] >= 10 )
		{
			isdamEffFlag_[ i ] = 0;
			damEffTimer_[ i ] = 0;
		}
		//発砲時の硝煙
		if ( isSmoEffFlag_[ i ] == 1 )
		{
			smoEffTimer_[ i ]++;
		}
		if ( smoEffTimer_[ i ] <= 5 && smoEffTimer_[ i ] >= 0 )
		{
			const float addPosY = 3.0f;
			const float addPosZ = 4.0f;
			smokeSummary(Vector3(Obj_[ i ]->wtf.position.x,Obj_[ i ]->wtf.position.y + addPosY,Obj_[ i ]->wtf.position.z - addPosZ),i);
		}
		if ( smoEffTimer_[ i ] >= 6 )
		{
			isSmoEffFlag_[ i ] = 0;
			smoEffTimer_[ i ] = 0;
		}

		//発砲時の弾速エフェクト
		if ( isballisticEffFlag_[ i ] == 1 )
		{
			ballisticEffTimer_[ i ]++;
		}
		if ( ballisticEffTimer_[ i ] <= 10 && ballisticEffTimer_[ i ] >= 1 )
		{
			ballisticfumeSummary(Vector3(bulletObj_[ i ]->wtf.position.x,bulletObj_[ i ]->wtf.position.y + 0.5f,bulletObj_[ i ]->wtf.position.z),i);
		}
		if ( ballisticEffTimer_[ i ] >= 10 )
		{
			isballisticEffFlag_[ i ] = 0;
			ballisticEffTimer_[ i ] = 0;
		}


	}


}

void ArmorEnemy::EffSummary(Vector3 bulletpos,const int& num)
{
	//パーティクル範囲
	for ( int i = 0; i < 5; i++ )
	{
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		Vector3 posG{};
		posG.x += ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		posG.y += ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		posG.z += ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		posG += bulletpos;
		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		Vector3 velG{};
		velG.x = ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		velG.y = ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		velG.z = ( float ) rand() / RAND_MAX * rnd_velGas - rnd_velGas / 1.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		Vector3 accG{};
		accG.x = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;
		accG.y = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;
		//追加
		gasParticle_[ num ]->Add(60,posG,velG,accG,0.5f,0.0f);
		gasParticle_[ num ]->Update();
	}
}

void ArmorEnemy::EffSummary2(Vector3 bulletpos2,const int& num2)
{
	//パーティクル範囲
	for ( int i = 0; i < 5; i++ )
	{
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		Vector3 pos2{};
		pos2.x += ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		pos2.y += ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		pos2.z += ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		pos2 += bulletpos2;
		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		Vector3 vel2{};
		vel2.x = ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		vel2.y = ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		vel2.z = ( float ) rand() / RAND_MAX * rnd_velGas - rnd_velGas / 1.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		Vector3 acc2{};
		acc2.x = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;
		acc2.y = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;
		//追加
		gasParticle2_[ num2 ]->Add(60,pos2,vel2,acc2,0.5f,0.0f);
		gasParticle2_[ num2 ]->Update();

	}

}

void ArmorEnemy::EffSummary3(Vector3 bulletpos3,const int& num3)
{
	//パーティクル範囲
	for ( int i = 0; i < 5; i++ )
	{
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		Vector3 pos3{};
		pos3.x += ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		pos3.y += ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		pos3.z += ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		pos3 += bulletpos3;
		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		Vector3 vel3{};
		vel3.x = ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		vel3.y = ( float ) rand() / RAND_MAX * rnd_velBGas - rnd_velBGas / 2.0f;
		vel3.z = ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 1.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		Vector3 acc3{};
		acc3.x = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;
		acc3.y = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;
		//追加
		gasParticle3_[ num3 ]->Add(60,pos3,vel3,acc3,0.5f,0.0f);
		gasParticle3_[ num3 ]->Update();
	}
}

void ArmorEnemy::EffSummary4(Vector3 bulletpos4,const int& num4)
{
	//パーティクル範囲
	for ( int i = 0; i < 5; i++ )
	{
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		Vector3 pos4{};
		pos4.x += ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		pos4.y += ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		pos4.z += ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		pos4 += bulletpos4;
		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		Vector3 vel4{};
		vel4.x = ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		vel4.y = ( float ) rand() / RAND_MAX * rnd_velBGas - rnd_velBGas / 2.0f;
		vel4.z = ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		Vector3 acc4{};
		acc4.x = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;
		acc4.y = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;
		//追加
		gasParticle4_[ num4 ]->Add(60,pos4,vel4,acc4,0.5f,0.0f);
		gasParticle4_[ num4 ]->Update();
	}
}

void ArmorEnemy::DamageSummary(Vector3 EnePos,const int& eneNum)
{
	//パーティクル範囲
	for ( int i = 0; i < 5; i++ )
	{
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		Vector3 pos{};
		pos.x += ( float ) rand() / RAND_MAX * rnd_posDamage - rnd_posDamage / 2.0f;
		pos.y += ( float ) rand() / RAND_MAX * rnd_posDamage - rnd_posDamage / 2.0f;
		pos.z += ( float ) rand() / RAND_MAX * rnd_posDamage - rnd_posDamage / 2.0f;
		pos += EnePos;
		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		Vector3 vel{};
		vel.x = ( float ) rand() / RAND_MAX * rnd_velDamage - rnd_velDamage / 2.0f;
		vel.y = ( float ) rand() / RAND_MAX * rnd_velDamage - rnd_velDamage / 2.0f;
		vel.z = ( float ) rand() / RAND_MAX * rnd_velDamage - rnd_velDamage / 2.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		Vector3 acc{};
		acc.x = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;
		acc.y = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;

		//追加
		DamageParticle_[ eneNum ]->Add(60,pos,vel,acc,2.0f,1.0f);

		DamageParticle_[ eneNum ]->Update();
	}
}

void ArmorEnemy::smokeSummary(Vector3 EnePos,const int& eneNum)
{
	//パーティクル範囲
	for ( int i = 0; i < 5; i++ )
	{
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		Vector3 poss{};
		poss.x += ( float ) rand() / RAND_MAX * rnd_smokepos - rnd_smokepos / 2.0f;
		poss.y += ( float ) rand() / RAND_MAX * rnd_smokepos - rnd_smokepos / 2.0f;
		poss.z += ( float ) rand() / RAND_MAX * rnd_smokepos - rnd_smokepos / 2.0f;
		poss += EnePos;
		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		Vector3 vels{};
		vels.x = ( float ) rand() / RAND_MAX * rnd_smokevel - rnd_smokevel / 2.0f;
		vels.y = ( float ) rand() / RAND_MAX * rnd_smokevel - rnd_smokevel / 2.0f;
		vels.z = ( float ) rand() / RAND_MAX * rnd_smokevel - rnd_smokevel / 2.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		Vector3 accs{};
		accs.x = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;
		accs.y = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;
		//追加
		smokeParticle_[ eneNum ]->Add(60,poss,vels,accs,0.8f,0.0f);
		smokeParticle_[ eneNum ]->Update();
	}
}

void ArmorEnemy::DamagefumeSummary(Vector3 fumepos,const int& num)
{
	//パーティクル範囲
	for ( int i = 0; i < 10; i++ )
	{
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		Vector3 posG{};
		posG.x += ( float ) rand() / RAND_MAX * rnd_posDamagefume - rnd_posDamagefume / 2.0f;
		posG.y += ( float ) rand() / RAND_MAX * rnd_posDamagefume - rnd_posDamagefume / 2.0f;
		posG.z += ( float ) rand() / RAND_MAX * rnd_posDamagefume - rnd_posDamagefume / 2.0f;
		posG += fumepos;
		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		Vector3 velG{};
		velG.x = ( float ) rand() / RAND_MAX * rnd_velDamagefume - rnd_velDamagefume / 2.0f;
		velG.y = ( float ) rand() / RAND_MAX * rnd_velGy - rnd_velGy / 1.0f;
		velG.z = ( float ) rand() / RAND_MAX * rnd_velDamagefume - rnd_velDamagefume / 2.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		Vector3 accG{};
		accG.x = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;
		accG.y = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;

		//追加
		fumeParticle_[ num ]->Add(60,posG,velG,accG,0.4f,0.0f);

		fumeParticle_[ num ]->Update();

	}
}

void ArmorEnemy::ballisticfumeSummary(Vector3 bulletpos,const int& num)
{
	//パーティクル範囲
	for ( int i = 0; i < 5; i++ )
	{
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		Vector3 posG{};
		posG.x += ( float ) rand() / RAND_MAX * rnd_posBallistic - rnd_posBallistic / 2.0f;
		posG.y += ( float ) rand() / RAND_MAX * rnd_posBallistic - rnd_posBallistic / 2.0f;
		posG.z += ( float ) rand() / RAND_MAX * rnd_posBallistic - rnd_posBallistic / 2.0f;
		posG += bulletpos;
		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		Vector3 velG{};
		velG.x = ( float ) rand() / RAND_MAX * rnd_velBallistic - rnd_velBallistic / 2.0f;
		velG.y = ( float ) rand() / RAND_MAX * rnd_velBallistic - rnd_velBallistic / 2.0f;
		velG.z = ( float ) rand() / RAND_MAX * rnd_velBallistic - rnd_velBallistic / 2.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		Vector3 accG{};
		accG.x = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;
		accG.y = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;

		//追加
		ballisticParticle_[ num ]->Add(60,posG,velG,accG,0.3f,0.0f);

		ballisticParticle_[ num ]->Update();

	}
}

void ArmorEnemy::EffDraw()
{
	if ( isGameStartTimer >= 200 )
	{
		for ( int i = 0; i < 4; i++ )
		{
			//背中の噴射ガス
			if ( isgasEffFlag_[ i ] == 1 )
			{
				if ( isAliveFlag_[ i ] == 0 )
				{
					gasParticle_[ i ]->Draw();
					gasParticle2_[ i ]->Draw();
					if ( Obj_[ i ]->wtf.position.z >= 35.0f )
					{
						gasParticle3_[ i ]->Draw();
						gasParticle4_[ i ]->Draw();
					}
				}
			}
			//攻撃受けた時の火花のパーティクル
			if ( isdamEffFlag_[ i ] == 1 )
			{
				DamageParticle_[ i ]->Draw();
			}
			//発砲時の硝煙パーティクル
			if ( isSmoEffFlag_[ i ] == 1 && smoEffTimer_[ i ] <= 5 && smoEffTimer_[ i ] >= 1 )
			{
				smokeParticle_[ i ]->Draw();
			}

			//体力半分以下の煙
			if ( isFumeEffFlag_[ i ] == 1 && fumeEffTimer_[ i ] <= 10 && fumeEffTimer_[ i ] >= 1 )
			{
				fumeParticle_[ i ]->Draw();
			}

			//発砲時の弾速エフェクト
			if ( isballisticEffFlag_[ i ] == 1 )
			{
				ballisticParticle_[ i ]->Draw();
			}

		}

	}

}

