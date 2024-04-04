#include"Player.h"
#include <imgui.h>
#include "Enemy/ArmorEnemy.h"

Player::Player() {

}

Player::~Player() {
	delete spriteCommon;
	delete playerUI_;

	for ( int i = 0; i < 3; i++ ){delete shootObj_[i];}
	delete shootModel_;
	delete retObj_;
	delete retModel_;

	delete retVisualObj_;
	delete retVisualModel_;
	delete Obj_;
	for ( int i = 0; i < MaxModel; i++ ){delete Model_[i];}
	for ( int i = 0; i < MaxColl; i++ ){delete collObj_[i];}
	delete collModel_;

	for ( int i = 0; i < MaxSprite; i++ ){delete sprite_[ i ];}

	delete debrisModel_;
	delete debrisObj_;
}

void Player::Reset()
{
	isGameStartFlag = false;
	isGameStartTimer = 0;
	acflag = false;
	rotaflag = false;
	camerasetFlag = false;

	//自機の納刀モデルから抜刀モデルに切り替え
	isBikswordstyFlag = 0;
	BikswordstyTimer = 0;
	//自機の生存フラグ
	isAliveFlag = true;

	//遠距離攻撃のUIとゲージ
	isgageUPFlag = false;
	isgageStopFlag = false;
	gageCount = 1;
	BulletCount = 1;
	retResetTimer = 0;
	retdisplay = false;
	EffTimer = 0;
	isEffFlag = 0;
	//自機のHP表示
	playerHP = 20.0f;

	//バイクの残骸が後ろに散らばる
	isScatterFlag = false;

	isShootFlag = false;
	BulletCoolTime = 0;
	//ゲームクリアするときのバイク移動
	isClearFlag = false;
	isclearFlagTimer = 0;

	//ボス登場でカメラの向きをかえるフラグ
	isCameraBehavior = 0;
	CameraBehaviorTimer = 0;
	CameraBehaviorTimer2 = 0;

	//画面シェイク
	isCamShake = 0;

	//ラウンド制御(プレイヤー側で設定する)
	isRoundFlag = 0;
	isDeadEnemy = 0;
	//ラウンドが変わるたびカメラが一旦引く
	incidenceCamera = 0;
	incidenceCamera2 = 0;
	incidenceCamera3 = 0;

	//障害物に当たって減速
	isDecelerationFlag = false;
	DecelerationTimer = 0;
	backTimer = 0;

	//敵が自機より後ろにいるときカメラを少し下げる
	standardCamera = 0;
	moveCamera = 0;
	cameraResetTimer = 0;
	BossCameraResetTimer = 0;
	//操作説明
	//1ウェーブ
	isOperationFlag = false;
	OperationbbTimer = 0;
	//2ウェーブ
	isOperationFlag2 = false;
	isOperationFlag3 = false;
	waveTimer2 = 0;
	OperationbbTimer2 = 0;

	Obj_->SetModel(Model_[ ActionState::IDEL ]);
	Obj_->wtf.position = { 0.0f,-2.0f,-20.0f };
}

void Player::Initialize(DirectXCommon* dxCommon,Input* input) {
	// nullptrチェック
	assert(dxCommon);
	assert(input);

	this->dxCommon_ = dxCommon;
	input_ = input;
	//スプライト共通部分の初期化
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon);

	camTransForm = new Transform();
	camera = new Camera(WinApp::window_width,WinApp::window_height);

	//自機
	Model_[ActionState::IDELBlade] = Model::LoadFromOBJ("bik");
	Model_[ ActionState::IDELBladeBefore ] = Model::LoadFromOBJ("bik2");
	Model_[ ActionState::IDEL ] = Model::LoadFromOBJ("bikst");
	Model_[ ActionState::IDELBefore ] = Model::LoadFromOBJ("bikst2");
	//加速
	Model_[ ActionState::Accel] = Model::LoadFromOBJ("bikac");
	Model_[ ActionState::AccelBefore] = Model::LoadFromOBJ("bikac2");
	//攻撃
	Model_[ ActionState::BulletAttackMid] = Model::LoadFromOBJ("bikmid");
	Model_[ ActionState::BulletAttack] = Model::LoadFromOBJ("bikAt");
	//自機納刀から抜刀へ
	Model_[ ActionState::ModeChangeMid] = Model::LoadFromOBJ("bikswordsty");
	Model_[ ActionState::ModeChange] = Model::LoadFromOBJ("bikswordsty2");
	//近接攻撃(左側)
	Model_[ ActionState::LSpinAttack] = Model::LoadFromOBJ("bikswordAt");
	Model_[ ActionState::RSpinAttack] = Model::LoadFromOBJ("bikswordAt2");

	//自機
	Obj_ = Object3d::Create();
	Obj_->SetModel(Model_[ ActionState::IDEL]);
	Obj_->wtf.scale = { 0.4f,0.4f,0.4f };
	Obj_->wtf.position = { 0.0f,-2.0f,-20.0f };

	const float addPos = 0.5f;
	const float addPosZ = -1.0f;
	//自機の当たり判定のモデル
	collModel_ = Model::LoadFromOBJ("collboll");
	collObj_[CollisionObject::PlayerColl] = Object3d::Create();
	collObj_[ CollisionObject::PlayerColl ]->SetModel(collModel_);
	collObj_[ CollisionObject::PlayerColl ]->wtf.position = { Obj_->wtf.position.x,Obj_->wtf.position.y + addPos,addPosZ };

	//自機の近接攻撃判定のモデル(左)
	collObj_[ CollisionObject::LAttackColl] = Object3d::Create();
	collObj_[ CollisionObject::LAttackColl]->SetModel(collModel_);
	collObj_[ CollisionObject::LAttackColl ]->wtf.position = { Obj_->wtf.position.x - addPos,Obj_->wtf.position.y + addPos,addPosZ };

	//自機の近接攻撃判定のモデル(右)
	collObj_[ CollisionObject::RAttackColl ] = Object3d::Create();
	collObj_[ CollisionObject::RAttackColl ]->SetModel(collModel_);
	collObj_[ CollisionObject::RAttackColl ]->wtf.position = { Obj_->wtf.position.x + addPos,Obj_->wtf.position.y + addPos,addPosZ };

	const float addPosX = 0.1f;
	//自機が衝突した時用のモデル
	//右
	collObj_[ CollisionObject::RPlayerColl] = Object3d::Create();
	collObj_[ CollisionObject::RPlayerColl ]->SetModel(collModel_);
	collObj_[ CollisionObject::RPlayerColl ]->wtf.position = { Obj_->wtf.position.x + addPosX,Obj_->wtf.position.y + addPos,addPosZ };
	//左
	collObj_[ CollisionObject::LPlayerColl ] = Object3d::Create();
	collObj_[ CollisionObject::LPlayerColl ]->SetModel(collModel_);
	collObj_[ CollisionObject::LPlayerColl ]->wtf.position = { Obj_->wtf.position.x - addPosX,Obj_->wtf.position.y + addPos,addPosZ };

	//自機が回転攻撃したときに動くモデル(エフェクト用)
	//右
	collObj_[ CollisionObject::RSparkColl ] = Object3d::Create();
	collObj_[ CollisionObject::RSparkColl ]->SetModel(collModel_);
	collObj_[ CollisionObject::RSparkColl ]->wtf.scale = { 0.3f,0.3f,0.3f };
	collObj_[ CollisionObject::RSparkColl ]->wtf.position = { 1.0f,-2.5f,0.0f };
	//左
	collObj_[ CollisionObject::LSparkColl ] = Object3d::Create();
	collObj_[ CollisionObject::LSparkColl ]->SetModel(collModel_);
	collObj_[ CollisionObject::LSparkColl ]->wtf.scale = { 0.3f,0.3f,0.3f };
	collObj_[ CollisionObject::LSparkColl ]->wtf.position = { 1.0f,-2.5f,0.0f };

	//自機の弾(弱)
	shootModel_ = Model::LoadFromOBJ("boll2");
	for ( int i = 0; i < 3; i++ ){
		shootObj_[i] = Object3d::Create();
		shootObj_[i]->SetModel(shootModel_);
		shootObj_[i]->wtf.scale = { 5.0f,5.0f,5.0f };
	}
	shootObj_[ BulletNumber::FirstBullet]->wtf.position = {Obj_->wtf.position.x,Obj_->wtf.position.y, Obj_->wtf.position.z};
	shootObj_[ BulletNumber::SecondBullet ]->wtf.position = {Obj_->wtf.position.x - 0.5f,Obj_->wtf.position.y - 0.2f, Obj_->wtf.position.z - 0.5f};
	shootObj_[ BulletNumber::ThirdBullet]->wtf.position = {Obj_->wtf.position.x + 0.5f,Obj_->wtf.position.y - 0.4f, Obj_->wtf.position.z - 1.0f};

	//レティクル(見えないレティクル)
	retModel_ = Model::LoadFromOBJ("ster");
	retObj_ = Object3d::Create();
	retObj_->SetModel(retModel_);
	retObj_->wtf.scale = { 0.6f,0.6f,0.6f };
	retObj_->wtf.position = { 0.0f,0.0f,50.0f };

	//レティクル(可視化)
	retVisualModel_ = Model::LoadFromOBJ("ret");
	retVisualObj_ = Object3d::Create();
	retVisualObj_->SetModel(retVisualModel_);
	retVisualObj_->wtf.scale = { 0.3f,0.3f,0.3f };
	retVisualObj_->wtf.position = { 0.0f,-0.3f,15.0f };

	//バイクの残骸
	debrisModel_ = Model::LoadFromOBJ("debris");
	debrisObj_ = Object3d::Create();
	debrisObj_->SetModel(debrisModel_);
	debrisObj_->wtf.scale = { 0.4f,0.4f,0.4f };

	//パーティクル生成
	//ガス(通常)
	gasParticle = std::make_unique<ParticleManager>();
	gasParticle->Initialize();
	gasParticle->LoadTexture("gas.png");
	gasParticle2 = std::make_unique<ParticleManager>();
	gasParticle2->Initialize();
	gasParticle2->LoadTexture("gas.png");

	//ガス(加速)
	gasParticleAccelR = std::make_unique<ParticleManager>();
	gasParticleAccelR->Initialize();
	gasParticleAccelR->LoadTexture("gas1.png");
	gasParticleAccelL = std::make_unique<ParticleManager>();
	gasParticleAccelL->Initialize();
	gasParticleAccelL->LoadTexture("gas1.png");

	//ガス(減速)
	gasParticleDecelR = std::make_unique<ParticleManager>();
	gasParticleDecelR->Initialize();
	gasParticleDecelR->LoadTexture("gas.png");
	gasParticleDecelL = std::make_unique<ParticleManager>();
	gasParticleDecelL->Initialize();
	gasParticleDecelL->LoadTexture("gas.png");

	//弾
	for ( int i = 0; i < 3; i++ ){
		ballisticParticle_[i] = std::make_unique<ParticleManager>();
		ballisticParticle_[i]->Initialize();
		ballisticParticle_[i]->LoadTexture("bulletchage.png");
	}

	//右スピンエフェクト
	RSpinParticle = std::make_unique<ParticleManager>();
	RSpinParticle->Initialize();
	RSpinParticle->LoadTexture("fire.png");

	//左スピンエフェクト
	LSpinParticle = std::make_unique<ParticleManager>();
	LSpinParticle->Initialize();
	LSpinParticle->LoadTexture("fire.png");

	playerEffect = std::make_unique<PlayerEffect>();
	playerEffect->Initialize();
	//UIの初期化(枚数が多いため)
	UIInitialize();
}

void Player::Update() {
	camera->Update();
	for ( int i = 0; i < 3; i++ ){shootObj_[i]->Update();}
	const float addRetPos = 50.0f;
	retObj_->Update();
	retObj_->wtf.position.z = Obj_->wtf.position.z + addRetPos;

	const float addRetVPos = 15.0f;
	retVisualObj_->Update();
	retVisualObj_->wtf.position.z = Obj_->wtf.position.z + addRetVPos;
	Obj_->Update();
	EffUpdate();
	collObj_[ CollisionObject::PlayerColl ]->Update();
	collObj_[ CollisionObject::RSparkColl ]->Update();
	if ( spineRffflag == false ){collObj_[ CollisionObject::RSparkColl ]->wtf.position.z = Obj_->wtf.position.z - 1.0f;}
	collObj_[ CollisionObject::LSparkColl ]->Update();
	if ( spineLffflag == false ){collObj_[ CollisionObject::LSparkColl ]->wtf.position.z = Obj_->wtf.position.z - 1.0f;}
	for ( int i = LSparkColl; i <= RSparkColl; i++ ){collObj_[ i ]->wtf.position.y = Obj_->wtf.position.y;}
	collObj_[ CollisionObject::RSparkColl ]->wtf.position.x = Obj_->wtf.position.x + 1.0f;
	collObj_[ CollisionObject::LSparkColl ]->wtf.position.x = Obj_->wtf.position.x - 1.0f;

	const float addPosX = 0.1f;
	const float addPos = 0.5f;
	const float addPosZ = 1.0f;
	if ( isGameStartTimer >= 180 ){collObj_[ CollisionObject::PlayerColl ]->wtf.position = { Obj_->wtf.position.x,Obj_->wtf.position.y + addPos,Obj_->wtf.position.z - addPosZ };}
	collObj_[ CollisionObject::LAttackColl ]->Update();
	collObj_[ CollisionObject::LAttackColl ]->wtf.position = { Obj_->wtf.position.x - addPosZ,Obj_->wtf.position.y + addPos,Obj_->wtf.position.z - addPosZ };
	collObj_[ CollisionObject::RAttackColl ]->Update();
	collObj_[ CollisionObject::RAttackColl ]->wtf.position = { Obj_->wtf.position.x + addPosZ,Obj_->wtf.position.y + addPos,Obj_->wtf.position.z - addPosZ };
	collObj_[ CollisionObject::RPlayerColl]->Update();
	collObj_[ CollisionObject::RPlayerColl ]->wtf.position = { Obj_->wtf.position.x + addPosX,Obj_->wtf.position.y + addPos,Obj_->wtf.position.z - addPosZ };
	collObj_[ CollisionObject::LPlayerColl ]->Update();
	collObj_[ CollisionObject::LPlayerColl ]->wtf.position = { Obj_->wtf.position.x - addPosX,Obj_->wtf.position.y + addPos,Obj_->wtf.position.z - addPosZ };
	debrisObj_->Update();
	if ( isScatterFlag == false ){debrisObj_->wtf.position = { Obj_->wtf.position.x,Obj_->wtf.position.y + 0.7f,Obj_->wtf.position.z };}

	isGameStartFlag = true;

	//ゲームが始まる
	GameStartMovie();

	//操作説明(1ステの視線誘導)
	if ( isGameStartTimer >= 310 ){OperationbbTimer++;}
	if ( OperationbbTimer >= 1 && OperationbbTimer <= 60 ){isOperationFlag = true;}
	else{isOperationFlag = false;}
	//操作説明(2ステの視線誘導)
	if ( waveTimer2 >= 100 ){
		OperationbbTimer2++;
		isOperationFlag2 = true;
	}
	if ( OperationbbTimer2 >= 1 && OperationbbTimer2 <= 60 ){isOperationFlag3 = true;}
	else{isOperationFlag3 = false;}

	//破片
	if ( isScatterFlag == true ){
		debrisObj_->wtf.rotation.y -= 0.1f;
		debrisObj_->wtf.position.z -= 0.2f;
		debrisObj_->wtf.position.y -= 0.1f;
	}

	//プレイヤーの行動一覧
	PlayerAction();
	if ( isbikslidFlag == true || isLeftAtFlag == true || isRightAtFlag == true ){
		isbulletEffFlag_ = 0;
		bulletEffTimer_ = 0;
	}
	else{isbulletEffFlag_ = 1;}

	if ( isBikswordstyFlag == 2 ){
		bikSpinTimer++;
		//バイクの車輪が動き出す(抜刀)
		if ( bikSpinTimer > 10 ){bikSpinTimer = 0;}
		//通常
		if ( isAtTimerFlag == false && isLeftAtFlag == false && isRightAtFlag == false && isAccelFlag == false ){
			if ( bikSpinTimer >= 1 && bikSpinTimer <= 5 ){Obj_->SetModel(Model_[ ActionState::IDELBladeBefore]);}
			else if ( bikSpinTimer >= 6 && bikSpinTimer <= 10 ){Obj_->SetModel(Model_[ ActionState::IDELBlade ]);}
		}
		//加速
		if ( isAccelFlag == true ){
			if ( bikSpinTimer >= 1 && bikSpinTimer <= 5 ){Obj_->SetModel(Model_[ ActionState::Accel]);}
			else if ( bikSpinTimer >= 6 && bikSpinTimer <= 10 ){Obj_->SetModel(Model_[ ActionState::AccelBefore]);}
		}
	}
	else if ( isBikswordstyFlag == 0 ){
		bikstSpinTimer++;
		//バイクの車輪が動き出す(納刀)
		if ( bikstSpinTimer > 10 ){bikstSpinTimer = 0;}
		if ( isAtTimerFlag == false && isLeftAtFlag == false && isRightAtFlag == false ){
			if ( bikstSpinTimer >= 1 && bikstSpinTimer <= 5 ){Obj_->SetModel(Model_[ ActionState::IDEL]);}
			else if ( bikstSpinTimer >= 6 && bikstSpinTimer <= 10 ){Obj_->SetModel(Model_[ ActionState::IDELBefore]);}
		}
	}

	//当たり判定
	if ( isCamShake == 1 ){DamageCamShake();}

	//ゲームクリア時に自機が前に進む
	if ( isClearFlag == true ){
		const float addObjPosZ = 0.5f;
		Obj_->wtf.position.z += addObjPosZ;
		isclearFlagTimer++;
	}
	if ( isclearFlagTimer >= 100 ){isclearFlagTimer = 100;}

	//ボス登場時のカメラ
	if ( BossCameraResetTimer == 80 ){
		if ( isCameraBehavior == 0 ){isCameraBehavior = 1;}
	}
	if ( isCameraBehavior == 1 ){CameraBehaviorTimer++;}
	if ( isCameraBehavior == 1 ){
		if ( CameraBehaviorTimer <= 70 ){
			const float addcameraPosZ = 0.2f;
			camera->wtf.position.z -= addcameraPosZ;
		}
		if ( CameraBehaviorTimer >= 20 ){
			const float addcameraPosY = 0.05f;
			camera->wtf.rotation.y += addcameraPosY;
		}
		if ( camera->wtf.position.z <= -14.0f ){camera->wtf.position.z = -14.0f;}
		if ( camera->wtf.rotation.y >= 2.5f ){
			camera->wtf.rotation.y = 2.5f;
			isCameraBehavior = 2;
		}
	}
	if ( isCameraBehavior == 2 ){
		CameraBehaviorTimer2++;
		if ( CameraBehaviorTimer2 >= 80 ){
			const float addcameraPosZ = 0.2f;
			camera->wtf.position.z += addcameraPosZ;
		}
		if ( CameraBehaviorTimer2 >= 100 ){
			const float addcameraPosY = 0.05f;
			camera->wtf.rotation.y -= addcameraPosY;
		}
		if ( camera->wtf.position.z >= 10.0f ){camera->wtf.position.z = 10.0f;}
		if ( camera->wtf.rotation.y <= 0.0f ){
			camera->wtf.rotation.y = 0.0f;
			isCameraBehavior = 3;
		}
	}

	//カメラリセット(最初のチュートリアル)
	if ( isDeadEnemy >= 2 ){
		cameraResetTimer++;
		if ( cameraResetTimer >= 1 && cameraResetTimer <= 2 ){
			if ( standardCamera == 1 ){standardCamera = 0;}
		}
		if ( cameraResetTimer <= 3 ){cameraResetTimer = 3;}
	}
	//カメラリセット(ボス前)
	if ( isRoundFlag == 9 ){
		BossCameraResetTimer++;
		if ( BossCameraResetTimer >= 1 && BossCameraResetTimer <= 2 ){
			if ( standardCamera == 1 ){standardCamera = 0;}
		}
		if ( BossCameraResetTimer >= 80 ){BossCameraResetTimer = 80;}
	}

	//ラウンド変化(2ラウンド目)
	if ( isDeadEnemy == 2 ){
		isRoundFlag = 1;
		waveTimer2++;
	}
	//ラウンド変化(3ラウンド目)ここから実践
	if ( isDeadEnemy == 4 ){isRoundFlag = 3;}
	//ラウンド変化(4ラウンド目)
	if ( isDeadEnemy == 6 ){isRoundFlag = 5;}
	//ラウンド変化(5ラウンド目)
	if ( isDeadEnemy == 9 ){isRoundFlag = 7;}
	//ラウンド変化(6ラウンド目)
	if ( isDeadEnemy == 13 ){isRoundFlag = 9;}

	//敵が自機より後ろにいるときカメラを少し下げる
	if ( isGameStartTimer >= 250 ){
		if ( standardCamera == 0 ){
			const float addcameraPosZ = 0.1f;
			camera->wtf.position.z += addcameraPosZ;
			if ( camera->wtf.position.z >= 0.0f ){camera->wtf.position.z = 0.0f;}
		}
		if ( standardCamera == 1 ){
			const float addcameraPosZ = 0.1f;
			camera->wtf.position.z -= addcameraPosZ;
			if ( camera->wtf.position.z <= -10.0f ){camera->wtf.position.z = -10.0f;}
		}
	}

	ImGui::Begin("Player");
	ImGui::Text("bulletgagePosition.x:%f",bulletgagePosition.x);
	ImGui::Text("playerHP:%f",playerHP);
	ImGui::Text("CameraBehaviorTimer:%d",CameraBehaviorTimer);
	ImGui::Text("CameraBehaviorTimer2:%d",CameraBehaviorTimer2);
	ImGui::Text("isDeadEnemy:%d",isDeadEnemy);
	ImGui::Text("isboostFlag:%d",isboostFlag);
	ImGui::End();
}

void Player::Draw() {

	if ( isAliveFlag == true ){Obj_->Draw();}

	if ( isGameStartTimer >= 180 ){
		/*extrusionRightObj_->Draw();
		extrusionLeftObj_->Draw();
		collObj_->Draw();*/
		if ( isCollSWFlag == true ){/*collSWObj_->Draw();*/}
		if ( isCollSWRightFlag == true ){/*collSWRightObj_->Draw();*/}
	}

	if ( isShootFlag == true ){/*shootObj_->Draw();*/}
	for ( int i = 0; i < 3; i++ ){/*shootObj_[i]->Draw();*/}

	if ( retdisplay == true && isClearFlag == false && OperationbbTimer2 >= 60 ){
		/*retObj_->Draw();*/
		retVisualObj_->Draw();
	}
	if ( isScatterFlag == true ){debrisObj_->Draw();}
	/*collObj_[ CollisionObject::LSparkColl ]->Draw();
	collObj_[ CollisionObject::RSparkColl ]->Draw();*/
	/*slashRObj_->Draw();
	slashLObj_->Draw();*/
}

void Player::UIInitialize()
{
	//enpty
	sprite_[Spritekinds::bulletenp] = new Sprite();
	sprite_[ Spritekinds::bulletenp ]->Initialize(spriteCommon);
	sprite_[ Spritekinds::bulletenp ]->SetPozition({ 0,0 });
	sprite_[ Spritekinds::bulletenp ]->SetSize({ 1280.0f, 720.0f });

	//タコメータ
	sprite_[ Spritekinds::meta] = new Sprite();
	sprite_[ Spritekinds::meta]->Initialize(spriteCommon);
	sprite_[ Spritekinds::meta]->SetPozition({ 0,0 });
	sprite_[ Spritekinds::meta]->SetSize({ 1280.0f, 720.0f });
	//メーター矢印
	sprite_[ Spritekinds::arrow] = new Sprite();
	sprite_[ Spritekinds::arrow ]->Initialize(spriteCommon);
	arrowPosition = sprite_[ Spritekinds::arrow ]->GetPosition();
	arrowPosition.x = 1245;
	arrowPosition.y = 700;
	sprite_[ Spritekinds::arrow ]->SetPozition(arrowPosition);
	arrowRotation.x = sprite_[ Spritekinds::arrow ]->GetRotation();
	arrowRotation.x = -145;
	sprite_[ Spritekinds::arrow ]->SetRotation(arrowRotation.x);
	sprite_[ Spritekinds::arrow ]->SetSize({ 80.0f, 20.0f });

	//自機の弾倉
	for ( int i = bullet1; i <= bullet3; i++ ){
		sprite_[ i ] = new Sprite();
		sprite_[ i ]->Initialize(spriteCommon);
		sprite_[ i ]->SetPozition({ 0,0 });
		sprite_[ i ]->SetSize({ 1280.0f, 720.0f });
	}

	//ゲージマックス文字
	sprite_[ Spritekinds::gagemax ] = new Sprite();
	sprite_[ Spritekinds::gagemax ]->Initialize(spriteCommon);
	sprite_[ Spritekinds::gagemax ]->SetPozition({ 0,0 });
	sprite_[ Spritekinds::gagemax ]->SetSize({ 1280.0f, 720.0f });

	//下地の黒
	sprite_[ Spritekinds::gageblack ] = new Sprite();
	sprite_[ Spritekinds::gageblack ]->Initialize(spriteCommon);
	sprite_[ Spritekinds::gageblack ]->SetPozition({ 0,0 });
	sprite_[ Spritekinds::gageblack ]->SetSize({ 1280.0f, 720.0f });

	//被弾時エフェクト
	sprite_[ Spritekinds::blood ] = new Sprite();
	sprite_[ Spritekinds::blood ]->Initialize(spriteCommon);
	sprite_[ Spritekinds::blood ]->SetPozition({ 0,0 });
	sprite_[ Spritekinds::blood ]->SetSize({ 1280.0f, 720.0f });

	//遠距離攻撃のUI
	sprite_[ Spritekinds::bulletgage ] = new Sprite();
	sprite_[ Spritekinds::bulletgage ]->Initialize(spriteCommon);
	bulletPosition = sprite_[ Spritekinds::bulletgage ]->GetPosition();
	sprite_[ Spritekinds::bulletgage ]->SetPozition(bulletPosition);
	sprite_[ Spritekinds::bulletgage ]->SetSize({ 1280.0f, 720.0f });

	//遠距離攻撃のゲージ
	sprite_[ Spritekinds::bluegage ] = new Sprite();
	sprite_[ Spritekinds::bluegage ]->Initialize(spriteCommon);
	bulletgagePosition = sprite_[ Spritekinds::bluegage ]->GetPosition();
	bulletgagePosition.x = -187.0f;
	sprite_[ Spritekinds::bluegage ]->SetPozition(bulletgagePosition);
	sprite_[ Spritekinds::bluegage ]->SetSize({ 1280.0f, 720.0f });

	//操作説明
	for ( int i = operation; i <= operation2bb; i++ ){
		sprite_[i] = new Sprite();
		sprite_[i]->Initialize(spriteCommon);
		sprite_[i]->SetPozition({ 0,0 });
		sprite_[i]->SetSize({ 1280.0f, 720.0f });
	}
	
	//画像読み込み
	//メーター
	spriteCommon->LoadTexture(meta,"meta.png");
	sprite_[ Spritekinds::meta ]->SetTextureIndex(meta);
	//矢印
	spriteCommon->LoadTexture(arrow,"arrow.png");
	sprite_[ Spritekinds::arrow ]->SetTextureIndex(arrow);
	//被弾エフェクト
	spriteCommon->LoadTexture(blood,"blood.png");
	sprite_[ Spritekinds::blood ]->SetTextureIndex(blood);

	//遠距離攻撃の弾表示
	spriteCommon->LoadTexture(bulletenp,"bulletenp.png");
	sprite_[ Spritekinds::bulletenp ]->SetTextureIndex(bulletenp);
	spriteCommon->LoadTexture(bullet1,"bullet1.png");
	sprite_[ Spritekinds::bullet1 ]->SetTextureIndex(bullet1);
	spriteCommon->LoadTexture(bullet2,"bullet2.png");
	sprite_[ Spritekinds::bullet2 ]->SetTextureIndex(bullet2);
	spriteCommon->LoadTexture(bullet3,"bullet3.png");
	sprite_[ Spritekinds::bullet3 ]->SetTextureIndex(bullet3);

	//ゲージMAXの時のUI
	spriteCommon->LoadTexture(gagemax,"gagemax.png");
	sprite_[ Spritekinds::gagemax ]->SetTextureIndex(gagemax);
	spriteCommon->LoadTexture(gageblack,"gageblack.png");
	sprite_[ Spritekinds::gageblack ]->SetTextureIndex(gageblack);

	//遠距離攻撃のUI
	spriteCommon->LoadTexture(bulletgage,"bulletgage.png");
	sprite_[ Spritekinds::bulletgage ]->SetTextureIndex(bulletgage);
	//遠距離攻撃のゲージ
	spriteCommon->LoadTexture(bluegage,"bluegage.png");
	sprite_[ Spritekinds::bluegage ]->SetTextureIndex(bluegage);

	//操作説明
	spriteCommon->LoadTexture(operation,"operation.png");
	sprite_[ Spritekinds::operation ]->SetTextureIndex(operation);
	spriteCommon->LoadTexture(operationbb,"operationbb.png");
	sprite_[ Spritekinds::operationbb ]->SetTextureIndex(operationbb);
	spriteCommon->LoadTexture(operation2,"operation2.png");
	sprite_[ Spritekinds::operation2 ]->SetTextureIndex(operation2);
	spriteCommon->LoadTexture(operation2bb,"operation2bb.png");
	sprite_[ Spritekinds::operation2bb ]->SetTextureIndex(operation2bb);
}

void Player::UIDraw()
{
	//被弾時の赤いエフェクト
	if ( isCamShake == 1 ){sprite_[ Spritekinds::blood]->Draw();}

	//HP関連
	if ( isGameStartTimer >= 180 ){
		if ( OperationbbTimer2 >= 60 ){
			sprite_[ Spritekinds::gageblack]->Draw();
			sprite_[ Spritekinds::bluegage ]->Draw();
			sprite_[ Spritekinds::bulletgage ]->Draw();

			if ( gageCount == 0 ){sprite_[ Spritekinds::bulletenp ]->Draw();}
			if ( gageCount == 1 ){sprite_[ Spritekinds::bullet1 ]->Draw();}
			if ( gageCount == 2 ){sprite_[ Spritekinds::bullet2 ]->Draw();}
			if ( gageCount >= 3 ){
				sprite_[ Spritekinds::bullet3 ]->Draw();
				sprite_[ Spritekinds::gagemax ]->Draw();
			}
		}
		sprite_[ Spritekinds::meta ]->Draw();
		sprite_[ Spritekinds::arrow]->Draw();
	}

	//操作説明関連
	if ( isGameStartTimer >= 310 ){
		sprite_[ Spritekinds::operation ]->Draw();
		if ( isOperationFlag == true ){sprite_[ Spritekinds::operationbb ]->Draw();}
		if ( isOperationFlag2 == true ){sprite_[ Spritekinds::operation2 ]->Draw();}
		if ( isOperationFlag3 == true ){sprite_[ Spritekinds::operation2bb ]->Draw();}
	}
}

void Player::PlayerAction()
{
	//自機とレティクルの速度
	const float playerSpeed = 0.08f;
	const float playerSpeed2 = 0.06f;
	const float retSpeed = 0.08f;
	const float retSpeed2 = 0.20f;

	//減速
	if ( isDecelerationFlag == true ){
		DecelerationTimer++;
		backTimer++;
		//減速で自機も少し後ろに下がる
		if ( backTimer >= 1 && backTimer <= 30 ){
			const float backSpeed = 0.07f;
			Obj_->wtf.position.z -= backSpeed;
		}
		if ( backTimer >= 31 && backTimer <= 90 ){
			const float backSpeedZ = 0.03f;
			Obj_->wtf.position.z += backSpeedZ;
			if ( Obj_->wtf.position.z >= -0.3f ){Obj_->wtf.position.z = -0.3f;}
		}
	}
	if ( DecelerationTimer >= 100 ){
		isDecelerationFlag = false;
		backTimer = 0;
		DecelerationTimer = 0;
	}

	//自機の攻撃モーション(射撃)
	if ( isAtTimerFlag == false ){
		if ( input_->TriggerKey(DIK_SPACE) || input_->PButtonTrigger(RT) ){isAtTimerFlag = true;}
	}
	if ( isAtTimerFlag == true ){
		AtTimer++;
		bikSpinTimer = 6;
	}
	if ( AtTimer >= 1 && AtTimer <= 5 ){
		Obj_->SetModel(Model_[ ActionState::BulletAttackMid]);
	}
	else if ( AtTimer >= 6 && AtTimer <= 20 ){
		Obj_->SetModel(Model_[ ActionState::BulletAttack]);
	}
	else if ( AtTimer >= 21 ){
		AtTimer = 0;
		bikSpinTimer++;
		isAtTimerFlag = false;
	}

	//自機の攻撃モーション(左近接攻撃)
	if ( isLeftAtFlag == false ){
		if ( input_->TriggerKey(DIK_Q) || input_->PButtonTrigger(LB) ){
			isLeftAtFlag = true;
			isCollSWFlag = true;
			if ( spineLffflag == false ){spineLffflag = true;}
		}
	}
	if ( isLeftAtFlag == true ){
		Obj_->wtf.rotation.z = 0.0f;
		leftAtTimer++;
		bikSpinTimer = 6;
	}
	if ( leftAtTimer >= 1 && leftAtTimer < 30 ){
		Obj_->SetModel(Model_[ ActionState::LSpinAttack]);
		const float LeftAtSpeed = 0.32f;
		Obj_->wtf.rotation.y += LeftAtSpeed;
	}
	else if ( leftAtTimer >= 30 ){
		leftAtTimer = 0;
		bikSpinTimer++;
		Obj_->wtf.rotation.y = 0.0f;
		isCollSWFlag = false;
		isLeftAtFlag = false;
		spineLffflag = false;
		isLSpinEffFlag_ = 0;
	}
	if ( leftAtTimer >= 1 && leftAtTimer < 30 ){collObj_[ CollisionObject::LSparkColl ]->wtf.position.z += 0.4f;}

	//自機の攻撃モーション(右近接攻撃)
	if ( isRightAtFlag == false ){
		if ( input_->TriggerKey(DIK_E) || input_->PButtonTrigger(RB) ){
			isRightAtFlag = true;
			isCollSWRightFlag = true;
			if ( spineRffflag == false ){spineRffflag = true;}
		}
	}
	if ( isRightAtFlag == true ){
		Obj_->wtf.rotation.z = 0.0f;
		rightAtTimer++;
		bikSpinTimer = 6;
	}
	if ( rightAtTimer >= 1 && rightAtTimer < 30 ){
		Obj_->SetModel(Model_[ ActionState::RSpinAttack]);
		const float rightAtSpeed = 0.32f;
		Obj_->wtf.rotation.y -= rightAtSpeed;
	}
	else if ( rightAtTimer >= 30 ){
		rightAtTimer = 0;
		bikSpinTimer++;
		Obj_->wtf.rotation.y = 0.0f;
		isCollSWRightFlag = false;
		isRightAtFlag = false;
		spineRffflag = false;
		isRSpinEffFlag_ = 0;
	}
	if ( rightAtTimer >= 1 && rightAtTimer < 10 ){collObj_[ CollisionObject::RSparkColl ]->wtf.position.z += 0.8f;}

	//自機が左右に動いたらモデルも傾く
	if ( input_->PushKey(DIK_D) || input_->StickInput(L_RIGHT) ){
		const float Speed = 0.03f;
		Obj_->wtf.rotation.z -= Speed;
		if ( Obj_->wtf.rotation.z <= -0.4f ){Obj_->wtf.rotation.z = -0.4f;}
	}
	else if ( input_->PushKey(DIK_A) || input_->StickInput(L_LEFT) ){
		const float Speed = 0.03f;
		Obj_->wtf.rotation.z += Speed;
		if ( Obj_->wtf.rotation.z >= 0.4f ){Obj_->wtf.rotation.z = 0.4f;}
	}
	else{Obj_->wtf.rotation.z = 0.0f;}

	//移動(自機)
	if ( isJumpFlag == false && isVertFlag == false ){
		if ( input_->TriggerKey(DIK_Z) ){isJumpFlag = true;}
	}
	if ( input_->PushKey(DIK_A) || input_->StickInput(L_LEFT) ){
		if ( limitmove == true ){
			Obj_->wtf.position.x -= 0.0f;
			collObj_[ CollisionObject::PlayerColl ]->wtf.position.x -= 0.0f;
			retObj_->wtf.position.x += 0.0f;
			camera->wtf.position.x -= 0.0f;
		}
		else{
			limitmove2 = false;
			Obj_->wtf.position.x -= playerSpeed;
			collObj_[ CollisionObject::PlayerColl ]->wtf.position.x -= playerSpeed;
			retObj_->wtf.position.x += playerSpeed2;
		}
	}
	if ( input_->PushKey(DIK_D) || input_->StickInput(L_RIGHT) ){
		if ( limitmove2 == true ){
			Obj_->wtf.position.x -= 0.0f;
			collObj_[ CollisionObject::PlayerColl ]->wtf.position.x -= 0.0f;
			retObj_->wtf.position.x += 0.0f;
			camera->wtf.position.x -= 0.0f;
		}
		else{
			limitmove = false;
			Obj_->wtf.position.x += playerSpeed;
			collObj_[ CollisionObject::PlayerColl ]->wtf.position.x += playerSpeed;
			retObj_->wtf.position.x -= playerSpeed2;
		}
	}

	if ( input_->PushKey(DIK_W) || input_->StickInput(L_UP) ){
		limitmove = false;
		limitmove2 = false;
		isboostFlag = 1;
		isArrowUpFlag = true;
	}
	else if ( input_->PushKey(DIK_S) || input_->StickInput(L_DOWN) ){
		limitmove = false;
		limitmove2 = false;
		isboostFlag = 2;
		isArrowDwonFlag = true;
	}
	else{
		isboostFlag = 0;
		isArrowUpFlag = false;
		isArrowDwonFlag = false;
	}

	if ( input_->StickInput(L_UP) && isRightAtFlag == false && isLeftAtFlag == false && isAtTimerFlag == false ){isAccelFlag = true;}
	else{isAccelFlag = false;}

	//移動(レティクル)
	if ( input_->PushKey(DIK_UP) || input_->StickInput(R_UP) ){
		retObj_->wtf.position.y += retSpeed2;
		retVisualObj_->wtf.position.y += retSpeed;
	}
	if ( input_->PushKey(DIK_DOWN) || input_->StickInput(R_DOWN) ){
		retObj_->wtf.position.y -= retSpeed2;
		retVisualObj_->wtf.position.y -= retSpeed;
	}
	if ( input_->PushKey(DIK_LEFT) || input_->StickInput(R_LEFT) ){
		retObj_->wtf.position.x -= retSpeed2;
		retVisualObj_->wtf.position.x -= retSpeed;
	}
	if ( input_->PushKey(DIK_RIGHT) || input_->StickInput(R_RIGHT) ){
		retObj_->wtf.position.x += retSpeed2;
		retVisualObj_->wtf.position.x += retSpeed;
	}

	//弾発射(弱)
	float ShortSpeed = 0.01f;
	if ( input_->TriggerKey(DIK_SPACE) || input_->PButtonTrigger(RT) ){
		if ( isShootFlag == false ){
			isShootFlag = true;
			isgageStopFlag = true;
		}
	}
	if ( BulletCoolTime == 0 ){
		bulletlen_[FirstBullet] = retObj_->wtf.position - shootObj_[ FirstBullet ]->wtf.position;
		bulletlen_[ FirstBullet ].nomalize();

		bulletlen_[SecondBullet] = retObj_->wtf.position - shootObj_[ SecondBullet ]->wtf.position;
		bulletlen_[ SecondBullet ].nomalize();

		bulletlen_[ThirdBullet] = retObj_->wtf.position - shootObj_[ ThirdBullet ]->wtf.position;
		bulletlen_[ ThirdBullet ].nomalize();
	}
	if ( isShootFlag == true ){
		BulletCoolTime++;
		if ( BulletCount >= 1 ){
			shootObj_[ FirstBullet ]->wtf.rotation.z += 0.2f;
			shootObj_[ FirstBullet ]->wtf.position += bulletlen_[ FirstBullet ];
			len_[ FirstBullet ] = bulletlen_[ FirstBullet ];
			len_[ FirstBullet ] *= ShortSpeed;
		}

		if ( BulletCoolTime >= 10){
			if ( BulletCount >= 2 ){
				shootObj_[ SecondBullet ]->wtf.rotation.z += 0.2f;
				shootObj_[ SecondBullet ]->wtf.position += bulletlen_[ SecondBullet ];
				len_[ SecondBullet ] = bulletlen_[ SecondBullet ];
				len_[ SecondBullet ] *= ShortSpeed;
			}
		}

		if ( BulletCoolTime >= 20 ){
			if ( BulletCount >= 3 ){
				shootObj_[ ThirdBullet ]->wtf.rotation.z += 0.2f;
				shootObj_[ ThirdBullet ]->wtf.position += bulletlen_[ ThirdBullet ];
				len_[ ThirdBullet ] = bulletlen_[ ThirdBullet ];
				len_[ ThirdBullet ] *= ShortSpeed;
			}
		}
		isBallisticEffFlag_ = 1;
	}
	else{ 
		shootObj_[FirstBullet]->wtf.position = {Obj_->wtf.position.x,Obj_->wtf.position.y, Obj_->wtf.position.z +1.0f};
		shootObj_[SecondBullet]->wtf.position = { Obj_->wtf.position.x - 1.0f,Obj_->wtf.position.y - 0.4f, Obj_->wtf.position.z+1.0f};
		shootObj_[ThirdBullet]->wtf.position = { Obj_->wtf.position.x + 1.0f,Obj_->wtf.position.y - 0.8f, Obj_->wtf.position.z+1.0f};
		isBallisticEffFlag_ = 0;
	}
	if ( BulletCoolTime >= 55.0f ){
		BulletCoolTime = 0;
		isShootFlag = false;
		isBallisticEffFlag_ = 0;
		isgageStopFlag = false;
		gageCount = 1;
		BulletCount = 1;
	}

	//残り残数のUI表示
	if ( isgageStopFlag == true ){
		gageCount = 0;
		bulletgagePosition.x = -187.0f;
		sprite_[ Spritekinds::bluegage ]->SetPozition(bulletgagePosition);
	}
	else{
		if ( OperationbbTimer2 >= 60 ){
			bulletgagePosition.x += 0.8f;
			sprite_[ Spritekinds::bluegage ]->SetPozition(bulletgagePosition);
		}
	}
	if ( bulletgagePosition.x >= 0.0f ){
		gageCount++;
		BulletCount++;
		bulletgagePosition.x = -187.0f;
		sprite_[ Spritekinds::bluegage ]->SetPozition(bulletgagePosition);
	}
	if ( gageCount >= 3 ){
		gageCount = 3;
		bulletgagePosition.x = 0.0f;
		sprite_[ Spritekinds::bluegage ]->SetPozition(bulletgagePosition);
	}

	//突進を受けた時にノックバック(右からの突進)
	if ( isKnockbackFlag == true ){knockbackTimer++;}
	if ( knockbackTimer >= 1 ){Obj_->wtf.position.x -= 0.03f;}
	if ( knockbackTimer >= 21 ){
		knockbackTimer = 0;
		isKnockbackFlag = false;
	}
	//突進を受けた時にノックバック(左からの突進)
	if ( isKnockbackFlagL == true ){knockbackTimerL++;}
	if ( knockbackTimerL >= 1 ){Obj_->wtf.position.x += 0.03f;}
	if ( knockbackTimerL >= 21 ){
		knockbackTimerL = 0;
		isKnockbackFlagL = false;
	}

	//エンジンブルブルする
	arrowTimer++;
	if ( arrowTimer >= 6 ){arrowTimer = 0;}
	if ( arrowTimer >= 0 && arrowTimer <= 3 ){
		arrowRotation.x += 1.0f;
		sprite_[ Spritekinds::arrow ]->SetRotation(arrowRotation.x);
	}
	if ( arrowTimer >= 4 && arrowTimer <= 6 ){
		arrowRotation.x -= 2.0f;
		sprite_[ Spritekinds::arrow ]->SetRotation(arrowRotation.x);
	}
	//エンジン動く
	if ( isArrowUpFlag == true ){
		arrowRotation.x += 1.0f;
		sprite_[ Spritekinds::arrow ]->SetRotation(arrowRotation.x);
		if ( arrowRotation.x >= -85.0f ){arrowRotation.x = -85.0f;}
	}
	if ( isArrowDwonFlag == true ){
		arrowRotation.x -= 1.0f;
		sprite_[ Spritekinds::arrow ]->SetRotation(arrowRotation.x);
		if ( arrowRotation.x <= -190.0f ){arrowRotation.x = -190.0f;}
	}
	if ( isArrowUpFlag == false && isArrowDwonFlag == false ){
		if ( arrowRotation.x >= -146.0f ){
			arrowRotation.x -= 1.4f;
			sprite_[ Spritekinds::arrow ]->SetRotation(arrowRotation.x);
			if ( arrowRotation.x <= -145.0f ){arrowRotation.x = -145.0f;}
		}
		if ( arrowRotation.x <= -144.0f ){
			arrowRotation.x += 1.4f;
			sprite_[ Spritekinds::arrow ]->SetRotation(arrowRotation.x);
			if ( arrowRotation.x >= -145.0f ){arrowRotation.x = -145.0f;}
		}
	}
}

void Player::EffUpdate()
{
	//バイクのエンジン
	if ( isbulletEffFlag_ == 1 ){bulletEffTimer_++;}
	if ( bulletEffTimer_ <= 20 && bulletEffTimer_ >= 1 ){
		const float gasLPosX = 0.15f;
		const float gasRPosX = 0.13f;
		const float gasAddPosY = 0.2f;
		const float gasLPosZ = 1.5f;
		const float gasRPosZ = 2.5f;
		//通常
		EffSummary(Vector3(Obj_->wtf.position.x - gasLPosX,Obj_->wtf.position.y + gasAddPosY,Obj_->wtf.position.z - gasLPosZ));
		EffSummary2(Vector3(Obj_->wtf.position.x + gasRPosX,Obj_->wtf.position.y + gasAddPosY,Obj_->wtf.position.z - gasLPosZ));
		//加速
		EffSummaryAccelR(Vector3(Obj_->wtf.position.x - gasLPosX,Obj_->wtf.position.y + gasAddPosY,Obj_->wtf.position.z - gasRPosZ));
		EffSummaryAccelL(Vector3(Obj_->wtf.position.x + gasRPosX,Obj_->wtf.position.y + gasAddPosY,Obj_->wtf.position.z - gasRPosZ));
		//減速
		EffSummaryDecelR(Vector3(Obj_->wtf.position.x,Obj_->wtf.position.y,Obj_->wtf.position.z));
		EffSummaryDecelL(Vector3(Obj_->wtf.position.x,Obj_->wtf.position.y,Obj_->wtf.position.z));
	}
	if ( bulletEffTimer_ >= 20 ){
		isbulletEffFlag_ = 0;
		bulletEffTimer_ = 0;
	}

	//自機の弾
	if ( isBallisticEffFlag_ == 1 ){ballisticEffTimer_++;}
	if ( ballisticEffTimer_ <= 10 && ballisticEffTimer_ >= 0 ){
		EffSummaryBullet(Vector3(shootObj_[0]->wtf.position.x,shootObj_[0]->wtf.position.y + 0.5f,shootObj_[0]->wtf.position.z));
		EffSummaryBullet2(Vector3(shootObj_[1]->wtf.position.x,shootObj_[1]->wtf.position.y + 0.5f,shootObj_[1]->wtf.position.z));
		EffSummaryBullet3(Vector3(shootObj_[2]->wtf.position.x,shootObj_[2]->wtf.position.y + 0.5f,shootObj_[2]->wtf.position.z));
	}
	if ( ballisticEffTimer_ >= 10 ){
		isBallisticEffFlag_ = 0;
		ballisticEffTimer_ = 0;
	}

	//スピン
	if ( isRSpinEffFlag_ == 1 ){RSpinEffTimer_++;}
	if ( RSpinEffTimer_ <= 10 && RSpinEffTimer_ >= 0 ){
		EffSummaryRSpin(Vector3(collObj_[ CollisionObject::RSparkColl ]->wtf.position.x,collObj_[ CollisionObject::RSparkColl ]->wtf.position.y + 0.5f,collObj_[ CollisionObject::RSparkColl ]->wtf.position.z));
	}
	if ( RSpinEffTimer_ >= 10 ){
		isRSpinEffFlag_ = 0;
		RSpinEffTimer_ = 0;
	}

	if ( isLSpinEffFlag_ == 1 ){LSpinEffTimer_++;}
	if ( LSpinEffTimer_ <= 10 && LSpinEffTimer_ >= 0 ){
		EffSummaryLSpin(Vector3(collObj_[ CollisionObject::LSparkColl ]->wtf.position.x,collObj_[ CollisionObject::LSparkColl ]->wtf.position.y + 0.5f,collObj_[ CollisionObject::LSparkColl ]->wtf.position.z));
	}
	if ( LSpinEffTimer_ >= 10 ){
		isLSpinEffFlag_ = 0;
		LSpinEffTimer_ = 0;
	}
}

void Player::EffSummary(Vector3 bulletpos)
{
	//パーティクル範囲
	for ( int i = 0; i < 20; i++ ){
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		Vector3 posGas{};
		posGas.x += ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		posGas.y += ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		posGas.z += ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		posGas += bulletpos;
		//速度
		Vector3 velGas{};
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		velGas.x = ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		velGas.y = ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		velGas.z = ( float ) rand() / RAND_MAX * rnd_velGasz - rnd_velGasz / 0.2f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		Vector3 accGas{};
		accGas.x = ( float ) rand() / RAND_MAX * rnd_accGas - rnd_accGas / 2.0f;
		accGas.y = ( float ) rand() / RAND_MAX * rnd_accGas - rnd_accGas / 2.0f;
		//追加
		gasParticle->Add(60,posGas,velGas,accGas,0.05f,0.0f);
		gasParticle->Update();
	}
}

void Player::EffSummary2(Vector3 bulletpos2)
{
	//パーティクル範囲
	for ( int i = 0; i < 20; i++ ){
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
		vel2.z = ( float ) rand() / RAND_MAX * rnd_velGasz - rnd_velGasz / 0.2f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		Vector3 acc2{};
		acc2.x = ( float ) rand() / RAND_MAX * rnd_accGas - rnd_accGas / 2.0f;
		acc2.y = ( float ) rand() / RAND_MAX * rnd_accGas - rnd_accGas / 2.0f;
		//追加
		gasParticle2->Add(60,pos2,vel2,acc2,0.05f,0.0f);
		gasParticle2->Update();
	}
}

void Player::EffSummaryAccelR(Vector3 bulletpos3)
{
	//パーティクル範囲
	for ( int i = 0; i < 50; i++ )
	{
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		Vector3 pos3G{};
		pos3G.x += ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		pos3G.y += ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		pos3G.z += ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		pos3G += bulletpos3;
		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		Vector3 vel3G{};
		vel3G.x = ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		vel3G.y = ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		vel3G.z = ( float ) rand() / RAND_MAX * rnd_velGasz - rnd_velGasz / 2.5f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		Vector3 acc3G{};
		acc3G.x = ( float ) rand() / RAND_MAX * rnd_accGas - rnd_accGas / 2.0f;
		acc3G.y = ( float ) rand() / RAND_MAX * rnd_accGas - rnd_accGas / 2.0f;
		//追加
		gasParticleAccelR->Add(60,pos3G,vel3G,acc3G,0.07f,0.0f);
		gasParticleAccelR->Update();
	}
}

void Player::EffSummaryAccelL(Vector3 bulletpos4)
{
	//パーティクル範囲
	for ( int i = 0; i < 50; i++ ){
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		Vector3 pos4G{};
		pos4G.x += ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		pos4G.y += ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		pos4G.z += ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		pos4G += bulletpos4;
		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		Vector3 vel4G{};
		vel4G.x = ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		vel4G.y = ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		vel4G.z = ( float ) rand() / RAND_MAX * rnd_velGasz - rnd_velGasz / 2.5f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		Vector3 acc4G{};
		acc4G.x = ( float ) rand() / RAND_MAX * rnd_accGas - rnd_accGas / 2.0f;
		acc4G.y = ( float ) rand() / RAND_MAX * rnd_accGas - rnd_accGas / 2.0f;
		//追加
		gasParticleAccelL->Add(60,pos4G,vel4G,acc4G,0.07f,0.0f);
		gasParticleAccelL->Update();
	}
}

void Player::EffSummaryDecelR(Vector3 bulletpos3)
{
	//パーティクル範囲
	for ( int i = 0; i < 5; i++ ){
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		Vector3 posD{};
		posD.x += ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		posD.y += ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		posD.z += ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		posD += bulletpos3;
		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		Vector3 velD{};
		velD.x = ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		velD.y = ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		velD.z = ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 0.05f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		Vector3 accD{};
		accD.x = ( float ) rand() / RAND_MAX * rnd_accGas - rnd_accGas / 2.0f;
		accD.y = ( float ) rand() / RAND_MAX * rnd_accGas - rnd_accGas / 2.0f;
		//追加
		gasParticleDecelR->Add(60,posD,velD,accD,0.05f,0.0f);
		gasParticleDecelR->Update();
	}
}

void Player::EffSummaryDecelL(Vector3 bulletpos4)
{
	//パーティクル範囲
	for ( int i = 0; i < 5; i++ ){
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		Vector3 posD2{};
		posD2.x += ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		posD2.y += ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		posD2.z += ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		posD2 += bulletpos4;
		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		Vector3 velD2{};
		velD2.x = ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		velD2.y = ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 2.0f;
		velD2.z = ( float ) rand() / RAND_MAX * rnd_posGas - rnd_posGas / 0.05f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		Vector3 accD2{};
		accD2.x = ( float ) rand() / RAND_MAX * rnd_accGas - rnd_accGas / 2.0f;
		accD2.y = ( float ) rand() / RAND_MAX * rnd_accGas - rnd_accGas / 2.0f;
		//追加
		gasParticleDecelL->Add(60,posD2,velD2,accD2,0.05f,0.0f);
		gasParticleDecelL->Update();
	}
}

void Player::EffSummaryBullet(Vector3 bulletpos)
{
	//パーティクル範囲
	for ( int i = 0; i < 5; i++ ){
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		Vector3 posG{};
		posG.x += ( float ) rand() / RAND_MAX * rnd_posBullet - rnd_posBullet / 2.0f;
		posG.y += ( float ) rand() / RAND_MAX * rnd_posBullet - rnd_posBullet / 2.0f;
		posG.z += ( float ) rand() / RAND_MAX * rnd_posBullet - rnd_posBullet / 2.0f;
		posG += bulletpos;
		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		Vector3 velG{};
		velG.x = ( float ) rand() / RAND_MAX * rnd_velBullet - rnd_velBullet / 2.0f;
		velG.y = ( float ) rand() / RAND_MAX * rnd_velBullet - rnd_velBullet / 2.0f;
		velG.z = ( float ) rand() / RAND_MAX * rnd_velBullet - rnd_velBullet / 2.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		Vector3 accG{};
		accG.x = ( float ) rand() / RAND_MAX * rnd_accGas - rnd_accGas / 2.0f;
		accG.y = ( float ) rand() / RAND_MAX * rnd_accGas - rnd_accGas / 2.0f;
		//追加
		ballisticParticle_[0]->Add(60,posG,velG,accG,0.5f,0.0f);
		ballisticParticle_[ 0 ]->Update();
	}
}

void Player::EffSummaryBullet2(Vector3 bulletpos)
{
	//パーティクル範囲
	for ( int i = 0; i < 5; i++ ){
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		Vector3 posG{};
		posG.x += ( float ) rand() / RAND_MAX * rnd_posBullet - rnd_posBullet / 2.0f;
		posG.y += ( float ) rand() / RAND_MAX * rnd_posBullet - rnd_posBullet / 2.0f;
		posG.z += ( float ) rand() / RAND_MAX * rnd_posBullet - rnd_posBullet / 2.0f;
		posG += bulletpos;
		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		Vector3 velG{};
		velG.x = ( float ) rand() / RAND_MAX * rnd_velBullet - rnd_velBullet / 2.0f;
		velG.y = ( float ) rand() / RAND_MAX * rnd_velBullet - rnd_velBullet / 2.0f;
		velG.z = ( float ) rand() / RAND_MAX * rnd_velBullet - rnd_velBullet / 2.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		Vector3 accG{};
		accG.x = ( float ) rand() / RAND_MAX * rnd_accGas - rnd_accGas / 2.0f;
		accG.y = ( float ) rand() / RAND_MAX * rnd_accGas - rnd_accGas / 2.0f;
		//追加
		ballisticParticle_[ 1 ]->Add(60,posG,velG,accG,0.5f,0.0f);
		ballisticParticle_[ 1 ]->Update();
	}
}

void Player::EffSummaryBullet3(Vector3 bulletpos)
{
	//パーティクル範囲
	for ( int i = 0; i < 5; i++ ){
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		Vector3 posG{};
		posG.x += ( float ) rand() / RAND_MAX * rnd_posBullet - rnd_posBullet / 2.0f;
		posG.y += ( float ) rand() / RAND_MAX * rnd_posBullet - rnd_posBullet / 2.0f;
		posG.z += ( float ) rand() / RAND_MAX * rnd_posBullet - rnd_posBullet / 2.0f;
		posG += bulletpos;
		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		Vector3 velG{};
		velG.x = ( float ) rand() / RAND_MAX * rnd_velBullet - rnd_velBullet / 2.0f;
		velG.y = ( float ) rand() / RAND_MAX * rnd_velBullet - rnd_velBullet / 2.0f;
		velG.z = ( float ) rand() / RAND_MAX * rnd_velBullet - rnd_velBullet / 2.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		Vector3 accG{};
		accG.x = ( float ) rand() / RAND_MAX * rnd_accGas - rnd_accGas / 2.0f;
		accG.y = ( float ) rand() / RAND_MAX * rnd_accGas - rnd_accGas / 2.0f;
		//追加
		ballisticParticle_[ 2 ]->Add(60,posG,velG,accG,0.5f,0.0f);
		ballisticParticle_[ 2 ]->Update();
	}
}

void Player::EffSummaryRSpin(Vector3 pos)
{
	//パーティクル範囲
	for ( int i = 0; i < 5; i++ ){
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		Vector3 posG{};
		posG.x += ( float ) rand() / RAND_MAX * rnd_posBullet - rnd_posBullet / 2.0f;
		posG.y += ( float ) rand() / RAND_MAX * rnd_posBullet - rnd_posBullet / 2.0f;
		posG.z += ( float ) rand() / RAND_MAX * rnd_posBullet - rnd_posBullet / 2.0f;
		posG += pos;
		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		Vector3 velG{};
		velG.x = ( float ) rand() / RAND_MAX * rnd_posBullet - rnd_posBullet / 2.0f;
		velG.y = ( float ) rand() / RAND_MAX * rnd_posBullet - rnd_posBullet / 2.0f;
		velG.z = ( float ) rand() / RAND_MAX * rnd_posBullet - rnd_posBullet / 2.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		Vector3 accG{};
		accG.x = ( float ) rand() / RAND_MAX * rnd_accGas - rnd_accGas / 2.0f;
		accG.y = ( float ) rand() / RAND_MAX * rnd_accGas - rnd_accGas / 2.0f;
		//追加
		RSpinParticle->Add(60,posG,velG,accG,0.5f,0.0f);
		RSpinParticle->Update();
	}
}

void Player::EffSummaryLSpin(Vector3 pos)
{
	//パーティクル範囲
	for ( int i = 0; i < 5; i++ ){
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		Vector3 posG{};
		posG.x += ( float ) rand() / RAND_MAX * rnd_posBullet - rnd_posBullet / 2.0f;
		posG.y += ( float ) rand() / RAND_MAX * rnd_posBullet - rnd_posBullet / 2.0f;
		posG.z += ( float ) rand() / RAND_MAX * rnd_posBullet - rnd_posBullet / 2.0f;
		posG += pos;
		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		Vector3 velG{};
		velG.x = ( float ) rand() / RAND_MAX * rnd_posBullet - rnd_posBullet / 2.0f;
		velG.y = ( float ) rand() / RAND_MAX * rnd_posBullet - rnd_posBullet / 2.0f;
		velG.z = ( float ) rand() / RAND_MAX * rnd_posBullet - rnd_posBullet / 2.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		Vector3 accG{};
		accG.x = ( float ) rand() / RAND_MAX * rnd_accGas - rnd_accGas / 2.0f;
		accG.y = ( float ) rand() / RAND_MAX * rnd_accGas - rnd_accGas / 2.0f;
		//追加
		LSpinParticle->Add(60,posG,velG,accG,0.5f,0.0f);
		LSpinParticle->Update();
	}
}

void Player::EffDraw()
{
	if ( isbulletEffFlag_ == 1 ){
		if ( isboostFlag == 0 ){
			gasParticle->Draw();
			gasParticle2->Draw();
		}
		else if ( isboostFlag == 1 ){
			gasParticleAccelR->Draw();
			gasParticleAccelL->Draw();
		}
		else if ( isboostFlag == 2 ){
			/*gasParticleDecelR->Draw();
			gasParticleDecelL->Draw();*/
		}
	}

	//遠距離攻撃
	if ( isBallisticEffFlag_ == 1 ){
		ballisticParticle_[ 0 ]->Draw();
		if ( BulletCount >= 2 ){ballisticParticle_[ 1 ]->Draw();}
		if ( BulletCount >= 3 ){ballisticParticle_[ 2 ]->Draw();}
	}
	playerEffect->Draw();
	//左右のスピンの火花
	if ( isRSpinEffFlag_ == 1 && RSpinEffTimer_ <= 10 && RSpinEffTimer_ >= 1 ){RSpinParticle->Draw();}
	if ( isLSpinEffFlag_ == 1 && LSpinEffTimer_ <= 10 && LSpinEffTimer_ >= 1 ){LSpinParticle->Draw();}
}

Vector3 Player::GetWorldPosition(int CollNumber) {
	//ワールド座標を入れる変数
	Vector3 worldPos;

	collObj_[ CollNumber ]->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	worldPos.x = collObj_[ CollNumber ]->wtf.matWorld.m[ 3 ][ 0 ];
	worldPos.y = collObj_[ CollNumber ]->wtf.matWorld.m[ 3 ][ 1 ];
	worldPos.z = collObj_[ CollNumber ]->wtf.matWorld.m[ 3 ][ 2 ];

	return worldPos;
}

Vector3 Player::GetBulletWorldPosition(int BulletNumber)
{
	//ワールド座標を入れる変数
	Vector3 BulletWorldPos;

	shootObj_[ BulletNumber ]->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	BulletWorldPos.x = shootObj_[ BulletNumber ]->wtf.matWorld.m[ 3 ][ 0 ];
	BulletWorldPos.y = shootObj_[ BulletNumber ]->wtf.matWorld.m[ 3 ][ 1 ];
	BulletWorldPos.z = shootObj_[ BulletNumber ]->wtf.matWorld.m[ 3 ][ 2 ];

	return BulletWorldPos;
}

void Player::GameStartMovie()
{
	if ( isGameStartFlag == true ){
		acflag = true;
		camerasetFlag = true;
		isGameStartTimer++;
		bikstSpinTimer++;
	}

	if ( camerasetFlag == true ){camerasetFlag = false;}

	if ( acflag == true ){
		const float PosSpeed = 0.3f;
		Obj_->wtf.position.z += PosSpeed;
	}

	if ( isGameStartTimer >= 60 ){rotaflag = true;}

	if ( isGameStartTimer <= 180 && rotaflag == true ){
		const float RotSpeed = 0.05f;
		camera->wtf.rotation.y -= RotSpeed;
	}

	if ( isGameStartTimer <= 180 && camera->wtf.rotation.y <= 0.2f && rotaflag == true ){
		const float RotSpeed = 0.2f;
		camera->wtf.rotation.y = RotSpeed;
		rotaflag = false;
	}

	if ( isGameStartTimer >= 179 && isGameStartTimer <= 180 ){Obj_->wtf.position.z = 0.0f;}
	if ( isGameStartTimer >= 180 ){
		acflag = false;
		const float PosSpeed = 0.3f;
		Obj_->wtf.position.z -= PosSpeed;
		retdisplay = true;
	}

	if ( isGameStartTimer >= 180 && isGameStartTimer <= 220 ){camera->wtf.rotation.y = 0.0f;}
	if ( isGameStartTimer >= 220 ){isBikswordstyFlag = 1;}

	if ( isBikswordstyFlag == 1 ){BikswordstyTimer++;}

	if ( BikswordstyTimer >= 1 && BikswordstyTimer <= 5 ){Obj_->SetModel(Model_[ ActionState::ModeChangeMid]);}
	else if ( BikswordstyTimer >= 6 && BikswordstyTimer <= 11 ){Obj_->SetModel(Model_[ ActionState::ModeChange]);}

	if ( BikswordstyTimer >= 12 ){isBikswordstyFlag = 2;}
}

void Player::DamageCamShake()
{
	//画面シェイク
	if ( isCamShake == 1 ){
		const float shakePos = 0.2f;
		const float shakePosY = 0.1f;
		camShakeTimer--;
		if ( camShakeTimer <= camShakeLimit && camShakeTimer > camShakeLimit * 3 / 4 ){camera->wtf.position.y += shakePosY;}
		else if ( camShakeTimer <= camShakeLimit * 3 / 4 && camShakeTimer > camShakeLimit * 2 / 4 ){camera->wtf.position.y -= shakePos;}
		else if ( camShakeTimer <= camShakeLimit * 2 / 4 && camShakeTimer > camShakeLimit * 1 / 4 ){camera->wtf.position.y += shakePos;}
		else if ( camShakeTimer <= camShakeLimit * 1 / 4 && camShakeTimer > 0 ){camera->wtf.position.y -= shakePos;}
		else if ( camShakeTimer <= 0 ){
			isCamShake = 0;
			camera->wtf.position.y = 0.0f;
		}
	}
}