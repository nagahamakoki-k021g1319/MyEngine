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

	delete collObj_;
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
	Obj_ = Object3d::Create();
	Obj_->SetModel(Modelst_);
	Obj_->wtf.scale = { 0.4f,0.4f,0.4f };
	Obj_->wtf.position = { 10.0f,-2.0f,-10.0f };
	//ポリゴン爆散の情報                 大きさ 回転  飛ぶ量 
	Obj_->SetPolygonExplosion({ 0.0f,-1.0f,6.28f,20.0f });

	//大砲の弾
	bulletModel_ = Model::LoadFromOBJ("eneboll");
	bulletObj_ = Object3d::Create();
	bulletObj_->SetModel(bulletModel_);
	bulletObj_->wtf.position = { Obj_->wtf.position.x,Obj_->wtf.position.y + 2.5f,Obj_->wtf.position.z - 3.0f };

	//当たり判定のモデル
	collModel_ = Model::LoadFromOBJ("collboll");
	collObj_ = Object3d::Create();
	collObj_->SetModel(collModel_);
	collObj_->wtf.scale = { 2.0f,2.0f,2.0f };
	collObj_->wtf.position = { Obj_->wtf.position.x,Obj_->wtf.position.y + 1.0f,Obj_->wtf.position.z };

	//パーティクル生成
	//エフェクトの情報(地面のズサ)
	gasParticle = std::make_unique<ParticleManager>();
	gasParticle.get()->Initialize();
	gasParticle->LoadTexture("gas.png");
	gasParticle->Update();

	gasParticle2 = std::make_unique<ParticleManager>();
	gasParticle2.get()->Initialize();
	gasParticle2->LoadTexture("gas.png");
	gasParticle2->Update();

	//エフェクトの情報(背中の噴射ガス)
	gasParticle3 = std::make_unique<ParticleManager>();
	gasParticle3.get()->Initialize();
	gasParticle3->LoadTexture("gas.png");
	gasParticle3->Update();

	gasParticle4 = std::make_unique<ParticleManager>();
	gasParticle4.get()->Initialize();
	gasParticle4->LoadTexture("gas.png");
	gasParticle4->Update();

}

void ArmorEnemy::Update(Vector3 playerPos,Vector3 playerBpos,bool playerShootFlag)
{
	Obj_->Update();
	collObj_->Update();
	collObj_->wtf.position = { Obj_->wtf.position.x,Obj_->wtf.position.y + 1.0f,Obj_->wtf.position.z };
	bulletObj_->Update();
	EffUpdate();
	isGameStartTimer++;
	isbulletEffFlag_ = 1;

	//魔導兵が後ろから登場
	if ( isGameStartTimer >= 200 ){Obj_->wtf.position.z += 0.7f;}
	if ( Obj_->wtf.position.z >= 25.0f ){Obj_->wtf.position.z = 25.0f;}

	//魔導兵の射撃
	if ( isGameStartTimer >= 200 && isShootFlag == false ){
		if ( isAliveFlag == true){BulletCoolTime++;}}
	if ( BulletCoolTime == 79 ){
		playerlen = playerPos - bulletObj_->wtf.position;
		playerlen.nomalize();
	}
	if ( BulletCoolTime >= 80 ){
		BulletCoolTime = 80;
		isShootFlag = true;
	}
	//攻撃時モデルが変わる
	if ( BulletCoolTime >= 70 ){Obj_->SetModel(Model_);}
	else{Obj_->SetModel(Modelst_);}

	//誘導弾
	if ( isShootFlag == true ){
		BulletdurationTime++;

		bulletObj_->wtf.position += playerlen;
		bitweenlen = playerlen;
		bitweenlen *= 0.1f;
	}
	else{bulletObj_->wtf.position = { Obj_->wtf.position.x,Obj_->wtf.position.y + 2.5f,Obj_->wtf.position.z - 3.0f};}
	if ( BulletdurationTime >= 40.0f ){
		BulletdurationTime = 0;
		isShootFlag = false;
		BulletCoolTime = 0;
	}

	

	//ポリゴン爆散
	if (HP <= 0 ){isExpolFlag = true;}
	if ( isExpolFlag == true ){
		ExpolTimer++;

		float polygon = ExpolTimer / ExpolMT;

		Obj_->SetDestruction(polygon);
		if ( ExpolTimer >= ExpolMT )
		{
			bulletEffTimer_ = 0;
			isAliveFlag = false;
			isShootFlag = false;
		}
	}

	//当たり判定
	if ( playerShootFlag == true){
		if (HP >= 1){
			if ( coll.CircleCollision(playerBpos,GetWorldPosition(),1.0f,1.0f) ){
				HP--;
				playerShootFlag = false;
			}
		}
	}
	ImGui::Begin("ArmorEnemy");

	ImGui::Text("isGameStartTimer:%d",isGameStartTimer);
	ImGui::Text("BulletCoolTime:%d",BulletCoolTime);
	ImGui::Text("HP:%d",HP);

	ImGui::End();

}

void ArmorEnemy::Draw()
{
	if ( isGameStartTimer >= 200 ){
		if ( isAliveFlag == true ){
			Obj_->Draw();
			if ( isShootFlag == true ){
				bulletObj_->Draw();
			}
			/*collObj_->Draw();*/
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
		EffSummary3(Vector3(Obj_->wtf.position.x + 0.3f,Obj_->wtf.position.y + 2.0f,Obj_->wtf.position.z + 2.0f));
		EffSummary4(Vector3(Obj_->wtf.position.x - 0.3f,Obj_->wtf.position.y + 2.0f,Obj_->wtf.position.z + 2.0f));
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
		gasParticle2->Add(60,pos2,vel2,acc2,0.5f,0.0f);

		gasParticle2->Update();

	}

}

void ArmorEnemy::EffSummary3(Vector3 bulletpos3)
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
		gasParticle3->Add(60,pos3,vel3,acc3,0.5f,0.0f);

		gasParticle3->Update();

	}
}

void ArmorEnemy::EffSummary4(Vector3 bulletpos4)
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
		gasParticle4->Add(60,pos4,vel4,acc4,0.5f,0.0f);

		gasParticle4->Update();

	}
}

void ArmorEnemy::EffDraw()
{
	if ( isGameStartTimer >= 200 ){
		if ( isbulletEffFlag_ == 1 ){
			if ( isAliveFlag == true ){
				gasParticle->Draw();
				gasParticle2->Draw();
				if ( Obj_->wtf.position.z >= 25.0f ){
					gasParticle3->Draw();
					gasParticle4->Draw();
				}
			}
		}
	}

}

Vector3 ArmorEnemy::GetWorldPosition()
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


