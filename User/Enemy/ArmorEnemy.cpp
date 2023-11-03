#include "ArmorEnemy.h"
#include <imgui.h>
#include "Player.h"

ArmorEnemy::ArmorEnemy()
{
}

ArmorEnemy::~ArmorEnemy()
{
	delete spriteCommon;
	for ( int i = 0; i < 2; i++ )
	{
		delete Obj_[i];
	}
	delete Obj2_;
	delete Model_;
	delete Modelst_;

	for ( int i = 0; i < 2; i++ )
	{
		delete bulletObj_[i];
	}
	delete bulletObj2_;
	delete bulletModel_;

	for ( int i = 0; i < 2; i++ )
	{
		delete collObj_[i];
	}
	delete collObj2_;
	delete collObj3_;
	delete collModel_;

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
	for ( int i = 0; i < 2; i++ )
	{
		Obj_[i] = Object3d::Create();
		Obj_[i]->SetModel(Modelst_);
		Obj_[i]->wtf.scale = { 0.4f,0.4f,0.4f };

		Obj_[ 0 ]->wtf.position = { 10.0f,-2.0f,-10.0f };
		Obj_[ 1 ]->wtf.position = { 10.0f,-2.0f,-100.0f };

		//ポリゴン爆散の情報                 大きさ 回転  飛ぶ量 
		Obj_[i]->SetPolygonExplosion({0.0f,-1.0f,6.28f,20.0f});
	}

	

	Obj2_ = Object3d::Create();
	Obj2_->SetModel(Modelst_);
	Obj2_->wtf.scale = { 0.4f,0.4f,0.4f };
	Obj2_->wtf.position = { -10.0f,-2.0f,-10.0f };
	//ポリゴン爆散の情報                 大きさ 回転  飛ぶ量 
	Obj2_->SetPolygonExplosion({ 0.0f,-1.0f,6.28f,20.0f });


	//大砲の弾
	bulletModel_ = Model::LoadFromOBJ("eneboll");
	for ( int i = 0; i < 2; i++ )
	{
		bulletObj_[i] = Object3d::Create();
		bulletObj_[i]->SetModel(bulletModel_);
		bulletObj_[i]->wtf.position = { Obj_[i]->wtf.position.x,Obj_[i]->wtf.position.y + 2.5f,Obj_[i]->wtf.position.z - 3.0f};
	}

	bulletObj2_ = Object3d::Create();
	bulletObj2_->SetModel(bulletModel_);
	bulletObj2_->wtf.position = { Obj2_->wtf.position.x,Obj2_->wtf.position.y + 2.5f,Obj2_->wtf.position.z - 3.0f };

	//当たり判定のモデル
	collModel_ = Model::LoadFromOBJ("collboll");
	for ( int i = 0; i < 2; i++ )
	{
		collObj_[i] = Object3d::Create();
		collObj_[i]->SetModel(collModel_);
		collObj_[i]->wtf.scale = { 2.0f,2.0f,2.0f };
		collObj_[i]->wtf.position = { Obj_[i]->wtf.position.x,Obj_[i]->wtf.position.y + 1.0f,Obj_[i]->wtf.position.z};
	}
	collObj2_ = Object3d::Create();
	collObj2_->SetModel(collModel_);
	collObj2_->wtf.scale = { 2.0f,2.0f,2.0f };
	collObj2_->wtf.position = { Obj2_->wtf.position.x,Obj2_->wtf.position.y + 1.0f,Obj2_->wtf.position.z };

	//自機の当たり判定
	collObj3_ = Object3d::Create();
	collObj3_->SetModel(collModel_);

	//パーティクル生成
	//エフェクトの情報(地面のズサ)
	for ( int i = 0; i < 2; i++ )
	{
		gasParticle_[i] = std::make_unique<ParticleManager>();
		gasParticle_[i].get()->Initialize();
		gasParticle_[i]->LoadTexture("gas.png");
		gasParticle_[i]->Update();

		gasParticle2_[ i ] = std::make_unique<ParticleManager>();
		gasParticle2_[ i ].get()->Initialize();
		gasParticle2_[ i ]->LoadTexture("gas.png");
		gasParticle2_[ i ]->Update();

		//エフェクトの情報(背中の噴射ガス)
		gasParticle3_[i] = std::make_unique<ParticleManager>();
		gasParticle3_[i].get()->Initialize();
		gasParticle3_[i]->LoadTexture("gas.png");
		gasParticle3_[i]->Update();

		gasParticle4_[i] = std::make_unique<ParticleManager>();
		gasParticle4_[i].get()->Initialize();
		gasParticle4_[i]->LoadTexture("gas.png");
		gasParticle4_[i]->Update();
	}
	//パーティクル生成
	//エフェクトの情報(地面のズサ)
	gasParticle_2 = std::make_unique<ParticleManager>();
	gasParticle_2.get()->Initialize();
	gasParticle_2->LoadTexture("gas.png");
	gasParticle_2->Update();

	gasParticle2_2 = std::make_unique<ParticleManager>();
	gasParticle2_2.get()->Initialize();
	gasParticle2_2->LoadTexture("gas.png");
	gasParticle2_2->Update();

	//エフェクトの情報(背中の噴射ガス)
	gasParticle3_2 = std::make_unique<ParticleManager>();
	gasParticle3_2.get()->Initialize();
	gasParticle3_2->LoadTexture("gas.png");
	gasParticle3_2->Update();
				
	gasParticle4_2 = std::make_unique<ParticleManager>();
	gasParticle4_2.get()->Initialize();
	gasParticle4_2->LoadTexture("gas.png");
	gasParticle4_2->Update();

}

void ArmorEnemy::Update(Vector3 playerPos,Vector3 playerBpos,bool playerShootFlag)
{
	for ( int i = 0; i < 2; i++ )
	{
		Obj_[i]->Update();
		collObj_[i]->Update();
		collObj_[i]->wtf.position = { Obj_[i]->wtf.position.x,Obj_[i]->wtf.position.y + 1.0f,Obj_[i]->wtf.position.z};
		bulletObj_[i]->Update();
	}
	Obj2_->Update();
	
	collObj2_->Update();
	collObj2_->wtf.position = { Obj2_->wtf.position.x,Obj2_->wtf.position.y + 1.0f,Obj2_->wtf.position.z };
	bulletObj2_->Update();
	EffUpdate();

	collObj3_->Update();
	collObj3_->wtf.position = { playerPos };
	isGameStartTimer++;

	isAliveFlag_[1] = 1;

	//魔導兵が後ろから登場
	for ( int i = 0; i < 2; i++ )
	{
		if ( isGameStartTimer >= 200 )
		{
			Obj_[i]->wtf.position.z += 0.7f;
		}
		if ( Obj_[i]->wtf.position.z >= 25.0f )
		{
			Obj_[i]->wtf.position.z = 25.0f;
			isMoveFlag_[i] = true;
		}
	}


	if ( isGameStartTimer >= 200 ){Obj2_->wtf.position.z += 0.7f;}
	if ( Obj2_->wtf.position.z >= 25.0f ){
		Obj2_->wtf.position.z = 25.0f;
		isMoveFlag2 = true;
	}

	//定位置についたら魔導兵が左右に少しづつ動く
	for ( int i = 0; i < 2; i++ )
	{
		if ( isMoveFlag_[i] == true ){
			if ( isbesideFlag_[ i] == false ){
				Obj_[ i ]->wtf.position.x -= 0.02f;
			}
			else{
				Obj_[ i ]->wtf.position.x += 0.01f;
			}
		}
		if ( isMoveFlag_[ i ] == true && Obj_[ i ]->wtf.position.x <= 3.0f )
		{
			isbesideFlag_[ i ] = true;
		}
		else if ( isMoveFlag_[ i ] == true && Obj_[ i ]->wtf.position.x >= 10.0f )
		{
			isbesideFlag_[ i ] = false;
		}
	}

	if ( isMoveFlag2 == true )
	{
		if ( isbesideFlag2 == false )
		{
			Obj2_->wtf.position.x += 0.01f;
		}
		else
		{
			Obj2_->wtf.position.x -= 0.01f;
		}
	}
	if ( isMoveFlag2 == true && Obj2_->wtf.position.x >= -1.0f )
	{
		isbesideFlag2 = true;
	}
	else if ( isMoveFlag2 == true && Obj2_->wtf.position.x <= -5.0f )
	{
		isbesideFlag2 = false;
	}


	for ( int i = 0; i < 2; i++ )
	{
		if ( HP_[i] >= 1 )
		{
			isbulletEffFlag_[ i ] = 1;
		}
		else
		{
			isbulletEffFlag_[ i ] = 0;
			bulletEffTimer_[ i ] = 0;
		}
	}

	if ( HP2 >= 1 ){
		isbulletEffFlag2_ = 1;
	}
	else
	{
		isbulletEffFlag2_ = 0;
		bulletEffTimer2_ = 0;
	}

	//魔導兵の射撃
	for ( int i = 0; i < 2; i++ )
	{
		if ( isGameStartTimer >= 200 && isShootFlag_[i] == false )
		{
			if ( isAliveFlag_[ i ] == 1 )
			{
				BulletCoolTime_[ i ]++;
			}
		}
		if ( BulletCoolTime_[ i ] == 119 )
		{
			playerlen_[ i ] = collObj3_->wtf.position - bulletObj_[ i ]->wtf.position;
			playerlen_[ i ].nomalize();
		}
		if ( BulletCoolTime_[ i ] >= 120 )
		{
			BulletCoolTime_[ i ] = 120;
			isShootFlag_[ i ] = true;
		}
		//攻撃時モデルが変わる
		if ( BulletCoolTime_[ i ] >= 110 )
		{
			Obj_[ i ]->SetModel(Model_);
		}
		else
		{
			Obj_[ i ]->SetModel(Modelst_);
		}

		//誘導弾
		if ( isShootFlag_[ i ] == true )
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
		if ( BulletdurationTime_[ i ] >= 40.0f )
		{
			BulletdurationTime_[ i ] = 0;
			isShootFlag_[ i ] = false;
			BulletCoolTime_[ i ] = 0;
		}
	}
	//魔導兵の射撃
	if ( isGameStartTimer >= 200 && isShootFlag2 == false )
	{
		if ( isAliveFlag2 == true )
		{
			BulletCoolTime2++;
		}
	}
	if ( BulletCoolTime2 == 159 )
	{
		playerlen2 = playerPos - bulletObj2_->wtf.position;
		playerlen2.nomalize();
	}
	if ( BulletCoolTime2 >= 160 )
	{
		BulletCoolTime2 = 160;
		isShootFlag2 = true;
	}
	//攻撃時モデルが変わる
	if ( BulletCoolTime2 >= 150 ){Obj2_->SetModel(Model_);}
	else{Obj2_->SetModel(Modelst_);}

	//誘導弾
	if ( isShootFlag2 == true )
	{
		BulletdurationTime2++;

		bulletObj2_->wtf.position += playerlen2;
		bitweenlen2 = playerlen2;
		bitweenlen2 *= 0.1f;
	}
	else
	{
		bulletObj2_->wtf.position = { Obj2_->wtf.position.x,Obj2_->wtf.position.y + 2.5f,Obj2_->wtf.position.z - 3.0f };
	}
	if ( BulletdurationTime2 >= 40.0f )
	{
		BulletdurationTime2 = 0;
		isShootFlag2 = false;
		BulletCoolTime2 = 0;
	}

	//ポリゴン爆散
	for ( int i = 0; i < 2; i++ )
	{
		if ( HP_[i] <= 0 )
		{
			isExpolFlag_[ i ] = true;
		}
		if ( isExpolFlag_[ i ] == true )
		{
			ExpolTimer_[ i ]++;

			polygon_[ i ] = ExpolTimer_[ i ] / ExpolMT_[ i ];

			Obj_[ i ]->SetDestruction(polygon_[ i ]);
			if ( ExpolTimer_[ i ] >= ExpolMT_[ i ] )
			{
				bulletEffTimer_[ i ] = 0;
				/*isAliveFlag_[ i ] = 0;*/
				isShootFlag_[ i ] = false;
			}
		}
	}

	if ( HP2 <= 0 )
	{
		isExpolFlag2 = true;
	}
	if ( isExpolFlag2 == true )
	{
		ExpolTimer2++;

		float polygon2 = ExpolTimer2 / ExpolMT2;

		Obj2_->SetDestruction(polygon2);
		if ( ExpolTimer2 >= ExpolMT2 )
		{
			bulletEffTimer2_ = 0;
			isAliveFlag2 = false;
			isShootFlag2 = false;
		}
	}

	//当たり判定
	if ( playerShootFlag == true){
		for ( int i = 0; i < 2; i++ )
		{
			if ( HP_[i] >= 1 )
			{
				if ( coll.CircleCollision(playerBpos,collObj_[i]->wtf.position,1.0f,1.0f) )
				{
					HP_[ i ]--;
					playerShootFlag = false;
				}
			}
		}
		if ( HP2 >= 1 )
		{
			if ( coll.CircleCollision(playerBpos,GetWorldPosition2(),1.0f,1.0f) )
			{
				HP2--;
				playerShootFlag = false;
			}
		}

	}

	if ( coll.CircleCollision(GetWorldBulletPosition(),collObj3_->wtf.position,0.6f,0.6f) )
	{
		player_->isCamShake = 1;
		player_->camShakeTimer = player_->camShakeLimit;
	}

	if ( coll.CircleCollision(GetWorldBulletPosition2(),collObj3_->wtf.position,0.6f,0.6f) )
	{
		player_->isCamShake = 1;
		player_->camShakeTimer = player_->camShakeLimit;
	}

	ImGui::Begin("ArmorEnemy");

	ImGui::Text("isGameStartTimer:%d",isGameStartTimer);
	ImGui::Text("position:%f,%f,%f",Obj_[1]->wtf.position.x,Obj_[1]->wtf.position.y,Obj_[1]->wtf.position.z);
	ImGui::Text("isAliveFlag_0:%d",isAliveFlag_[0]);
	ImGui::Text("isAliveFlag_1:%d",isAliveFlag_[1]);
	//ImGui::Text("BulletCoolTime:%d",BulletCoolTime);
	//ImGui::Text("HP:%d",HP);

	ImGui::End();

}

void ArmorEnemy::Draw()
{
	collObj3_->Draw();
	if ( isGameStartTimer >= 200 ){
		for ( int i = 0; i < 2; i++ ){
			if ( isAliveFlag_[i] == 1 ){
				Obj_[ i ]->Draw();
				if ( isShootFlag_[ i ] == true ){
					bulletObj_[ i ]->Draw();
				}
				collObj_[i]->Draw();
			}
		}
		if ( isAliveFlag2 == true )
		{
			Obj2_->Draw();
			if ( isShootFlag2 == true )
			{
				bulletObj2_->Draw();
			}
			/*collObj_->Draw();*/
		}

	}
}

void ArmorEnemy::EffUpdate()
{
	for ( int i = 0; i < 2; i++ )
	{
		if ( isbulletEffFlag_[i] == 1 )
		{
			bulletEffTimer_[ i ]++;
		}
		if ( bulletEffTimer_[ i ] <= 20 && bulletEffTimer_[ i ] >= 1 )
		{
			EffSummary(Vector3(Obj_[ i ]->wtf.position.x + 1.0f, Obj_[i]->wtf.position.y - 1.5f,Obj_[i]->wtf.position.z),i);
			EffSummary2(Vector3(Obj_[ i ]->wtf.position.x - 1.0f,Obj_[i]->wtf.position.y - 1.5f,Obj_[i]->wtf.position.z),i);
			EffSummary3(Vector3(Obj_[ i ]->wtf.position.x + 0.3f,Obj_[i]->wtf.position.y + 2.0f,Obj_[i]->wtf.position.z + 2.0f),i);
			EffSummary4(Vector3(Obj_[ i ]->wtf.position.x - 0.3f,Obj_[i]->wtf.position.y + 2.0f,Obj_[i]->wtf.position.z + 2.0f),i);
		}
		if ( bulletEffTimer_[ i ] >= 20 )
		{
			isbulletEffFlag_[ i ] = 0;
			bulletEffTimer_[ i ] = 0;
		}
	}

	if ( isbulletEffFlag2_ == 1 )
	{
		bulletEffTimer2_++;
	}
	if ( bulletEffTimer2_ <= 20 && bulletEffTimer2_ >= 1 )
	{
		EffSummary_2(Vector3( Obj2_->wtf.position.x + 1.0f,Obj2_->wtf.position.y - 1.5f,Obj2_->wtf.position.z));
		EffSummary2_2(Vector3(Obj2_->wtf.position.x - 1.0f,Obj2_->wtf.position.y - 1.5f,Obj2_->wtf.position.z));
		EffSummary3_2(Vector3(Obj2_->wtf.position.x + 0.3f,Obj2_->wtf.position.y + 2.0f,Obj2_->wtf.position.z + 2.0f));
		EffSummary4_2(Vector3(Obj2_->wtf.position.x - 0.3f,Obj2_->wtf.position.y + 2.0f,Obj2_->wtf.position.z + 2.0f));
	}
	if ( bulletEffTimer2_ >= 20 )
	{
		isbulletEffFlag2_ = 0;
		bulletEffTimer2_ = 0;
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
		gasParticle_[num]->Add(60,posG,velG,accG,0.5f,0.0f);

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

void ArmorEnemy::EffSummary_2(Vector3 bulletpos)
{
	//パーティクル範囲
	for ( int i = 0; i < 5; i++ )
	{
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_posG_2 = 0.0f;
		const float rnd_posG_2y = 0.0f;
		const float rnd_posG_2z = 0.0f;
		Vector3 posG_2{};
		posG_2.x += ( float ) rand() / RAND_MAX * rnd_posG_2 - rnd_posG_2 / 2.0f;
		posG_2.y += ( float ) rand() / RAND_MAX * rnd_posG_2y - rnd_posG_2y / 2.0f;
		posG_2.z += ( float ) rand() / RAND_MAX * rnd_posG_2z - rnd_posG_2z / 2.0f;
		posG_2 += bulletpos;
		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		const float rnd_velG_2 = 0.0f;
		const float rnd_velGy_2 = 0.0f;
		const float rnd_velGz_2 = 0.1f;
		Vector3 velG_2{};
		velG_2.x = ( float ) rand() / RAND_MAX * rnd_velG_2 - rnd_velG_2 / 2.0f;
		velG_2.y = ( float ) rand() / RAND_MAX * rnd_velGy_2 - rnd_velGy_2 / 2.0f;
		velG_2.z = ( float ) rand() / RAND_MAX * rnd_velGz_2 - rnd_velGz_2 / 1.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		const float rnd_accG_2 = 0.000001f;
		Vector3 accG_2{};
		accG_2.x = ( float ) rand() / RAND_MAX * rnd_accG_2 - rnd_accG_2 / 2.0f;
		accG_2.y = ( float ) rand() / RAND_MAX * rnd_accG_2 - rnd_accG_2 / 2.0f;

		//追加
		gasParticle_2->Add(60,posG_2,velG_2,accG_2,0.5f,0.0f);

		gasParticle_2->Update();
	}
}

void ArmorEnemy::EffSummary2_2(Vector3 bulletpos2)
{
	//パーティクル範囲
	for ( int i = 0; i < 5; i++ )
	{
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_pos2_2 = 0.0f;
		const float rnd_posy2_2 = 0.0f;
		const float rnd_posz2_2 = 0.0f;
		Vector3 pos2_2{};
		pos2_2.x += ( float ) rand() / RAND_MAX * rnd_pos2_2 - rnd_pos2_2 / 2.0f;
		pos2_2.y += ( float ) rand() / RAND_MAX * rnd_posy2_2 - rnd_posy2_2 / 2.0f;
		pos2_2.z += ( float ) rand() / RAND_MAX * rnd_posz2_2 - rnd_posz2_2 / 2.0f;
		pos2_2 += bulletpos2;
		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		const float rnd_vel2_2 = 0.0f;
		const float rnd_vely2_2 = 0.0f;
		const float rnd_velz2_2 = 0.1f;
		Vector3 vel2_2{};
		vel2_2.x = ( float ) rand() / RAND_MAX * rnd_vel2_2 - rnd_vel2_2 / 2.0f;
		vel2_2.y = ( float ) rand() / RAND_MAX * rnd_vely2_2 - rnd_vely2_2 / 2.0f;
		vel2_2.z = ( float ) rand() / RAND_MAX * rnd_velz2_2 - rnd_velz2_2 / 1.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		const float rnd_acc2_2 = 0.000001f;
		Vector3 acc2_2{};
		acc2_2.x = ( float ) rand() / RAND_MAX * rnd_acc2_2 - rnd_acc2_2 / 2.0f;
		acc2_2.y = ( float ) rand() / RAND_MAX * rnd_acc2_2 - rnd_acc2_2 / 2.0f;

		//追加
		gasParticle2_2->Add(60,pos2_2,vel2_2,acc2_2,0.5f,0.0f);

		gasParticle2_2->Update();

	}
}

void ArmorEnemy::EffSummary3_2(Vector3 bulletpos3)
{
	//パーティクル範囲
	for ( int i = 0; i < 5; i++ )
	{
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_pos3_2 = 0.0f;
		const float rnd_posy3_2 = 0.0f;
		const float rnd_posz3_2 = 0.0f;
		Vector3 pos3_2{};
		pos3_2.x += ( float ) rand() / RAND_MAX *  rnd_pos3_2 - rnd_pos3_2 / 2.0f;
		pos3_2.y += ( float ) rand() / RAND_MAX * rnd_posy3_2 - rnd_posy3_2 / 2.0f;
		pos3_2.z += ( float ) rand() / RAND_MAX * rnd_posz3_2 - rnd_posz3_2 / 2.0f;
		pos3_2 += bulletpos3;
		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		const float  rnd_vel3_2 = 0.0f;
		const float rnd_vely3_2 = -0.05f;
		const float rnd_velz3_2 = 0.0f;
		Vector3 vel3_2{};
		vel3_2.x = ( float ) rand() / RAND_MAX *  rnd_vel3_2 - rnd_vel3_2/ 2.0f;
		vel3_2.y = ( float ) rand() / RAND_MAX * rnd_vely3_2 - rnd_vely3_2 / 2.0f;
		vel3_2.z = ( float ) rand() / RAND_MAX * rnd_velz3_2 - rnd_velz3_2 / 1.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		const float rnd_acc3_2 = 0.000001f;
		Vector3 acc3_2{};
		acc3_2.x = ( float ) rand() / RAND_MAX * rnd_acc3_2 - rnd_acc3_2 / 2.0f;
		acc3_2.y = ( float ) rand() / RAND_MAX * rnd_acc3_2 - rnd_acc3_2 / 2.0f;

		//追加
		gasParticle3_2->Add(60,pos3_2,vel3_2,acc3_2,0.5f,0.0f);

		gasParticle3_2->Update();

	}
}

void ArmorEnemy::EffSummary4_2(Vector3 bulletpos4)
{
	//パーティクル範囲
	for ( int i = 0; i < 5; i++ )
	{
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float  rnd_pos4_2 = 0.0f;
		const float rnd_posy4_2 = 0.0f;
		const float rnd_posz4_2 = 0.0f;
		Vector3 pos4_2{};
		pos4_2.x += ( float ) rand() / RAND_MAX *  rnd_pos4_2 -  rnd_pos4_2 / 2.0f;
		pos4_2.y += ( float ) rand() / RAND_MAX * rnd_posy4_2 - rnd_posy4_2 / 2.0f;
		pos4_2.z += ( float ) rand() / RAND_MAX * rnd_posz4_2 - rnd_posz4_2 / 2.0f;
		pos4_2 += bulletpos4;
		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		const float  rnd_vel4_2 = 0.0f;
		const float rnd_vely4_2 = -0.05f;
		const float rnd_velz4_2 = 0.0f;
		Vector3 vel4_2{};
		vel4_2.x = ( float ) rand() / RAND_MAX *  rnd_vel4_2 -  rnd_vel4_2 / 2.0f;
		vel4_2.y = ( float ) rand() / RAND_MAX * rnd_vely4_2 - rnd_vely4_2 / 2.0f;
		vel4_2.z = ( float ) rand() / RAND_MAX * rnd_velz4_2 - rnd_velz4_2 / 2.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		const float rnd_acc4_2 = 0.000001f;
		Vector3 acc4_2{};
		acc4_2.x = ( float ) rand() / RAND_MAX * rnd_acc4_2 - rnd_acc4_2 / 2.0f;
		acc4_2.y = ( float ) rand() / RAND_MAX * rnd_acc4_2 - rnd_acc4_2 / 2.0f;

		//追加
		gasParticle4_2->Add(60,pos4_2,vel4_2,acc4_2,0.5f,0.0f);

		gasParticle4_2->Update();

	}
}

void ArmorEnemy::EffDraw()
{
	if ( isGameStartTimer >= 200 ){
		for ( int i = 0; i < 2; i++ )
		{
			if ( isbulletEffFlag_[i] == 1 )
			{
				if ( isAliveFlag_[ i ] == 1 )
				{
					gasParticle_[ i ]->Draw();
					gasParticle2_[ i ]->Draw();
					if ( Obj_[ i ]->wtf.position.z >= 25.0f )
					{
						gasParticle3_[ i ]->Draw();
						gasParticle4_[ i ]->Draw();
					}
				}
			}
		}
		if ( isbulletEffFlag2_ == 1 )
		{
			if ( isAliveFlag2 == true )
			{
				gasParticle_2->Draw();
				gasParticle2_2->Draw();
				if ( Obj2_->wtf.position.z >= 25.0f )
				{
					gasParticle3_2->Draw();
					gasParticle4_2->Draw();
				}
			}
		}

	}

}

Vector3 ArmorEnemy::GetWorldPosition()
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

Vector3 ArmorEnemy::GetWorldPosition2()
{
	//ワールド座標を入れる変数
	Vector3 worldPos2;

	collObj2_->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	worldPos2.x = collObj2_->wtf.matWorld.m[ 3 ][ 0 ];
	worldPos2.y = collObj2_->wtf.matWorld.m[ 3 ][ 1 ];
	worldPos2.z = collObj2_->wtf.matWorld.m[ 3 ][ 2 ];

	return worldPos2;
}

Vector3 ArmorEnemy::GetWorldBulletPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldbulletPos;

	//bulletObj_->wtf.UpdateMat();
	////ワールド行列の平行移動成分
	//worldbulletPos.x = bulletObj_->wtf.matWorld.m[ 3 ][ 0 ];
	//worldbulletPos.y = bulletObj_->wtf.matWorld.m[ 3 ][ 1 ];
	//worldbulletPos.z = bulletObj_->wtf.matWorld.m[ 3 ][ 2 ];

	return worldbulletPos;
}

Vector3 ArmorEnemy::GetWorldBulletPosition2()
{
	//ワールド座標を入れる変数
	Vector3 worldbulletPos2;

	bulletObj2_->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	worldbulletPos2.x = bulletObj2_->wtf.matWorld.m[ 3 ][ 0 ];
	worldbulletPos2.y = bulletObj2_->wtf.matWorld.m[ 3 ][ 1 ];
	worldbulletPos2.z = bulletObj2_->wtf.matWorld.m[ 3 ][ 2 ];

	return worldbulletPos2;
}


