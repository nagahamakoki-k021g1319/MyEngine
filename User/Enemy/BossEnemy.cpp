//#include "BossEnemy.h"
//
//BossEnemy::BossEnemy()
//{
//}
//
//BossEnemy::~BossEnemy()
//{
//	delete spriteCommon;
//	delete Obj_;
//	delete Model_;
//
//	delete collObj_;
//	delete collModel_;
//
//	delete bikclushObj_;
//	delete bikclushModel_;
//
//	delete blowenemyObj_;
//	delete blowenemyModel_;
//}
//
//void BossEnemy::Initialize(DirectXCommon* dxCommon,Input* input)
//{
//	// nullptrチェック
//	assert(dxCommon);
//	assert(input);
//
//	this->dxCommon_ = dxCommon;
//	input_ = input;
//	//スプライト共通部分の初期化
//	spriteCommon = new SpriteCommon;
//	spriteCommon->Initialize(dxCommon);
//
//
//	//雑魚敵(攻撃状態)
//	Model_ = Model::LoadFromOBJ("bikenemy");
//	Obj_ = Object3d::Create();
//	Obj_->SetModel(Model_);
//	Obj_->wtf.scale = { 0.4f,0.4f,0.4f };
//	Obj_->wtf.position = { -3.0f,-2.0f,0.0f };
//
//	//当たり判定のモデル
//	collModel_ = Model::LoadFromOBJ("collboll");
//	collObj_ = Object3d::Create();
//	collObj_->SetModel(collModel_);
//	collObj_->wtf.position = { Obj_->wtf.position.x,Obj_->wtf.position.y,Obj_->wtf.position.z - 1.0f };
//
//
//	//雑魚敵(死んだときのバイクスピン)
//	bikclushModel_ = Model::LoadFromOBJ("bikclush");
//	bikclushObj_ = Object3d::Create();
//	bikclushObj_->SetModel(bikclushModel_);
//	bikclushObj_->wtf.scale = { 0.4f,0.4f,0.4f };
//	bikclushObj_->wtf.position = { Obj_->wtf.position.x,Obj_->wtf.position.y,Obj_->wtf.position.z };
//
//	//雑魚敵(死んだときの敵が吹っ飛ぶ)
//	blowenemyModel_ = Model::LoadFromOBJ("blowenemy");
//	blowenemyObj_ = Object3d::Create();
//	blowenemyObj_->SetModel(blowenemyModel_);
//	blowenemyObj_->wtf.scale = { 0.4f,0.4f,0.4f };
//	blowenemyObj_->wtf.position = { Obj_->wtf.position.x,Obj_->wtf.position.y,Obj_->wtf.position.z };
//
//	//パーティクル生成
//	gasParticle = std::make_unique<ParticleManager>();
//	gasParticle.get()->Initialize();
//	gasParticle->LoadTexture("gas.png");
//	gasParticle->Update();
//
//}
//
//void BossEnemy::Update()
//{
//	Obj_->Update();
//	collObj_->Update();
//	collObj_->wtf.position = { Obj_->wtf.position.x,Obj_->wtf.position.y + 0.5f,Obj_->wtf.position.z - 1.0f };
//	bikclushObj_->Update();
//	blowenemyObj_->Update();
//	EffUpdate();
//	isGameStartTimer++;
//
//	/*if ( isGameStartTimer >= 220 ){
//		Obj_->wtf.position.z += 0.5f;
//	}
//	if ( Obj_->wtf.position.z >= 0.0f){
//		Obj_->wtf.position.z = 0.0f;
//	}*/
//
//
//	//バイクスピン
//	if ( input_->TriggerKey(DIK_4) ){
//		isBikclushFlag = true;
//		isbulletEffFlag_ = 0;
//		bulletEffTimer_ = 0;
//	}
//	else{isbulletEffFlag_ = 1;}
//
//	//バイクが前進しながら倒れる
//	if( isBikclushFlag == true){
//		bikclushObj_->wtf.rotation.z += 0.1f;
//		bikclushObj_->wtf.position.z += 0.3f;
//		blowenemyObj_->wtf.position.y -= 0.1f;
//		blowenemyObj_->wtf.position.z -= 0.2f;
//	}
//	if ( bikclushObj_->wtf.rotation.z >= 1.5f){
//		bikclushObj_->wtf.rotation.z = 1.5f;
//		isBikSpinFlag = true;
//	}
//	//バイクが倒れて後ろに行く
//	if ( isBikSpinFlag == true){
//		bikclushObj_->wtf.position.z -= 0.6f;
//		bikclushObj_->wtf.rotation.x -= 0.05f;
//	}
//
//
//}
//
//void BossEnemy::Draw()
//{
//	if ( isGameStartTimer >= 200 ){
//		if ( isBikclushFlag == false ){
//			Obj_->Draw();
//			/*collObj_->Draw();*/
//		}
//		else if ( isBikclushFlag == true ){
//			bikclushObj_->Draw();
//			blowenemyObj_->Draw();
//		}
//	}
//
//}
//
//void BossEnemy::EffUpdate()
//{
//	if ( isbulletEffFlag_ == 1 )
//	{
//		bulletEffTimer_++;
//	}
//	if ( bulletEffTimer_ <= 20 && bulletEffTimer_ >= 1 )
//	{
//		EffSummary(Vector3(Obj_->wtf.position.x - 0.3f,Obj_->wtf.position.y - 0.5f,Obj_->wtf.position.z + 0.5f));
//	}
//	if ( bulletEffTimer_ >= 20 )
//	{
//		isbulletEffFlag_ = 0;
//		bulletEffTimer_ = 0;
//	}
//}
//
//void BossEnemy::EffSummary(Vector3 bulletpos)
//{
//	//パーティクル範囲
//	for ( int i = 0; i < 5; i++ )
//	{
//		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
//		const float rnd_posG = 0.0f;
//		const float rnd_posGy = 0.0f;
//		const float rnd_posGz = 0.0f;
//		Vector3 posG{};
//		posG.x += ( float ) rand() / RAND_MAX * rnd_posG - rnd_posG / 2.0f;
//		posG.y += ( float ) rand() / RAND_MAX * rnd_posGy - rnd_posGy / 2.0f;
//		posG.z += ( float ) rand() / RAND_MAX * rnd_posGz - rnd_posGz / 2.0f;
//		posG += bulletpos;
//		//速度
//		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
//		const float rnd_velG = 0.0f;
//		const float rnd_velGy = 0.0f;
//		const float rnd_velGz = 0.5f;
//		Vector3 velG{};
//		velG.x = ( float ) rand() / RAND_MAX * rnd_velG - rnd_velG / 2.0f;
//		velG.y = ( float ) rand() / RAND_MAX * rnd_velGy - rnd_velGy / 2.0f;
//		velG.z = ( float ) rand() / RAND_MAX * rnd_velGz - rnd_velGz / 0.5f;
//		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
//		const float rnd_accG = 0.000001f;
//		Vector3 accG{};
//		accG.x = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;
//		accG.y = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;
//
//		//追加
//		gasParticle->Add(60,posG,velG,accG,0.1f,0.0f);
//
//		gasParticle->Update();
//
//	}
//}
//
//void BossEnemy::EffDraw()
//{
//	if ( isGameStartTimer >= 200 ){
//		if ( isBikclushFlag == false ){
//			if ( isbulletEffFlag_ == 1 ){
//				gasParticle->Draw();
//			}
//		}
//	}
//}
//
//Vector3 BossEnemy::GetWorldPosition()
//{
//	return Vector3();
//}
