#include"Player.h"
#include <imgui.h>
#include "Enemy/ArmorEnemy.h"

Player::Player() {

}

Player::~Player() {
	delete spriteCommon;
	delete playerUI_;
	for ( int i = 0; i < 3; i++ )
	{
		delete shootObj_[i];
	}
	delete shootModel_;
	delete shootStObj_;
	delete shootStModel_;
	delete retObj_;
	delete retModel_;
	delete ret1Model_;
	delete ret2Model_;

	delete retVisualObj_;
	delete retVisualModel_;

	delete BloodUI;

	delete bullet1UI;
	delete bullet2UI;
	delete bullet3UI;
	delete gageMaxUI;
	delete underBlackUI;

	delete bulletEnptyUI;
	delete metaUI;
	delete arrowUI;

	delete Obj_;
	for ( int i = 0; i < 12; i++ )
	{
		delete Model_[i];
	}

	delete collObj_;
	delete collModel_;

	delete collSWObj_;
	delete collSWModel_;
	delete collSWRightObj_;
	delete collSWRightModel_;

	delete extrusionRightModel_;
	delete extrusionRightObj_;
	delete extrusionLeftModel_;
	delete extrusionLeftObj_;

	delete bulletUI;
	delete bulletgageUI;

	delete operationUI;
	delete operationbbUI;

	delete operation2UI;
	delete operationbb2UI;

	delete debrisModel_;
	delete debrisObj_;

	delete slashModel_;
	delete slashRObj_;
	delete slashLObj_;
}

void Player::Reset()
{
	/*isGameStartFlag = false;
	isGameStartTimer = 0;
	acflag = false;
	rotaflag = false;
	camerasetFlag = false;

	isClearFlag = false;
	isclearFlagTimer = 0;

	gageCount = 1;
	BulletCount = 1;

	isCameraBehavior = 0;
	CameraBehaviorTimer = 0;
	CameraBehaviorTimer2 = 0;
	
	isRoundFlag = 0;
	isDeadEnemy = 0;
	
	standardCamera = 0;
	moveCamera = 0;
	cameraResetTimer = 0;
	BossCameraResetTimer = 0;

	isOperationFlag = false;
	OperationbbTimer = 0;

	isOperationFlag2 = false;
	isOperationFlag3 = false;
	waveTimer2 = 0;
	OperationbbTimer2 = 0;

	isScatterFlag = false;

	Obj_->wtf.position = { 0.0f,-2.0f,-20.0f };*/

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
	collObj_ = Object3d::Create();
	collObj_->SetModel(collModel_);
	collObj_->wtf.position = { Obj_->wtf.position.x,Obj_->wtf.position.y + addPos,addPosZ };

	//自機の近接攻撃判定のモデル(左)
	collSWModel_ = Model::LoadFromOBJ("collboll");
	collSWObj_ = Object3d::Create();
	collSWObj_->SetModel(collSWModel_);
	collSWObj_->wtf.position = { Obj_->wtf.position.x - addPos,Obj_->wtf.position.y + addPos,addPosZ };

	//自機の近接攻撃判定のモデル(左)
	collSWRightModel_ = Model::LoadFromOBJ("collboll");
	collSWRightObj_ = Object3d::Create();
	collSWRightObj_->SetModel(collSWRightModel_);
	collSWRightObj_->wtf.position = { Obj_->wtf.position.x + addPos,Obj_->wtf.position.y + addPos,addPosZ };

	const float addPosX = 0.1f;
	//自機が衝突した時用のモデル
	//右
	extrusionRightModel_ = Model::LoadFromOBJ("collboll");
	extrusionRightObj_ = Object3d::Create();
	extrusionRightObj_->SetModel(extrusionRightModel_);
	extrusionRightObj_->wtf.position = { Obj_->wtf.position.x + addPosX,Obj_->wtf.position.y + addPos,addPosZ };
	//左
	extrusionLeftModel_ = Model::LoadFromOBJ("collboll");
	extrusionLeftObj_ = Object3d::Create();
	extrusionLeftObj_->SetModel(extrusionLeftModel_);
	extrusionLeftObj_->wtf.position = { Obj_->wtf.position.x - addPosX,Obj_->wtf.position.y + addPos,addPosZ };

	//自機が回転攻撃したときに動くモデル(エフェクト用)
	//右
	slashModel_ = Model::LoadFromOBJ("collboll");
	slashRObj_ = Object3d::Create();
	slashRObj_->SetModel(slashModel_);
	slashRObj_->wtf.scale = { 0.3f,0.3f,0.3f };
	slashRObj_->wtf.position = { 1.0f,-2.5f,0.0f };
	//左
	slashLObj_ = Object3d::Create();
	slashLObj_->SetModel(slashModel_);
	slashLObj_->wtf.scale = { 0.3f,0.3f,0.3f };
	slashLObj_->wtf.position = { 1.0f,-2.5f,0.0f };

	//自機の弾(弱)
	shootModel_ = Model::LoadFromOBJ("boll2");
	for ( int i = 0; i < 3; i++ )
	{
		shootObj_[i] = Object3d::Create();
		shootObj_[i]->SetModel(shootModel_);
		shootObj_[i]->wtf.scale = { 5.0f,5.0f,5.0f };
	}
	shootObj_[0]->wtf.position = {Obj_->wtf.position.x,Obj_->wtf.position.y, Obj_->wtf.position.z};
	shootObj_[1]->wtf.position = {Obj_->wtf.position.x - 0.5f,Obj_->wtf.position.y - 0.2f, Obj_->wtf.position.z - 0.5f};
	shootObj_[2]->wtf.position = {Obj_->wtf.position.x + 0.5f,Obj_->wtf.position.y - 0.4f, Obj_->wtf.position.z - 1.0f};


	//自機の弾(強)
	shootStModel_ = Model::LoadFromOBJ("boll");
	shootStObj_ = Object3d::Create();
	shootStObj_->SetModel(shootStModel_);
	shootStObj_->wtf.position = { Obj_->wtf.position.x,Obj_->wtf.position.y, Obj_->wtf.position.z };
	shootStObj_->wtf.scale = { 0.8f,0.8f,0.8f };

	//レティクル(見えないレティクル)
	retModel_ = Model::LoadFromOBJ("ster");
	ret1Model_ = Model::LoadFromOBJ("ster1");
	ret2Model_ = Model::LoadFromOBJ("ster2");
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

	//剣チャージ
	swordchageParticle = std::make_unique<ParticleManager>();
	swordchageParticle->Initialize();
	swordchageParticle->LoadTexture("swordchage.png");

	//弾
	for ( int i = 0; i < 3; i++ )
	{
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
	for ( int i = 0; i < 3; i++ )
	{
		shootObj_[i]->Update();
	}
	shootStObj_->Update();
	const float addRetPos = 50.0f;
	retObj_->Update();
	retObj_->wtf.position.z = Obj_->wtf.position.z + addRetPos;

	const float addRetVPos = 15.0f;
	retVisualObj_->Update();
	retVisualObj_->wtf.position.z = Obj_->wtf.position.z + addRetVPos;
	Obj_->Update();
	EffUpdate();
	collObj_->Update();
	slashRObj_->Update();
	if ( spineRffflag == false )
	{
		slashRObj_->wtf.position = { Obj_->wtf.position.x + 0.5f,Obj_->wtf.position.y,Obj_->wtf.position.z - 1.0f };
	}

	slashLObj_->Update();
	if ( spineLffflag == false )
	{
		slashLObj_->wtf.position = { Obj_->wtf.position.x - 0.5f,Obj_->wtf.position.y,Obj_->wtf.position.z - 1.0f };
	}
	const float addPosX = 0.1f;
	const float addPos = 0.5f;
	const float addPosZ = 1.0f;
	if ( isGameStartTimer >= 180 )
	{
		collObj_->wtf.position = { Obj_->wtf.position.x,Obj_->wtf.position.y + addPos,Obj_->wtf.position.z - addPosZ };
	}
	collSWObj_->Update();
	collSWObj_->wtf.position = { Obj_->wtf.position.x - addPosZ,Obj_->wtf.position.y + addPos,Obj_->wtf.position.z - addPosZ };
	collSWRightObj_->Update();
	collSWRightObj_->wtf.position = { Obj_->wtf.position.x + addPosZ,Obj_->wtf.position.y + addPos,Obj_->wtf.position.z - addPosZ };
	extrusionRightObj_->Update();
	extrusionRightObj_->wtf.position = { Obj_->wtf.position.x + addPosX,Obj_->wtf.position.y + addPos,Obj_->wtf.position.z - addPosZ };
	extrusionLeftObj_->Update();
	extrusionLeftObj_->wtf.position = { Obj_->wtf.position.x - addPosX,Obj_->wtf.position.y + addPos,Obj_->wtf.position.z - addPosZ };
	debrisObj_->Update();
	if ( isScatterFlag == false )
	{
		debrisObj_->wtf.position = { Obj_->wtf.position.x,Obj_->wtf.position.y + 0.7f,Obj_->wtf.position.z };
	}


	

	isGameStartFlag = true;

	//ゲームが始まる
	GameStartMovie();

	//操作説明(1ステの視線誘導)
	if ( isGameStartTimer >= 310 )
	{
		OperationbbTimer++;
	}
	if ( OperationbbTimer >= 1 && OperationbbTimer <= 60 )
	{
		isOperationFlag = true;
	}
	else
	{
		isOperationFlag = false;
	}
	//操作説明(2ステの視線誘導)
	if ( waveTimer2 >= 100 )
	{
		OperationbbTimer2++;
		isOperationFlag2 = true;
	}
	if ( OperationbbTimer2 >= 1 && OperationbbTimer2 <= 60 )
	{
		isOperationFlag3 = true;
	}
	else
	{
		isOperationFlag3 = false;
	}


	if ( isScatterFlag == true )
	{
		debrisObj_->wtf.rotation.y -= 0.1f;
		debrisObj_->wtf.position.z -= 0.2f;
		debrisObj_->wtf.position.y -= 0.1f;
	}


	isswordchageEffFlag_ = 1;

	//プレイヤーの行動一覧
	PlayerAction();
	if ( isbikslidFlag == true || isLeftAtFlag == true || isRightAtFlag == true )
	{
		isbulletEffFlag_ = 0;
		bulletEffTimer_ = 0;
	}
	else
	{
		isbulletEffFlag_ = 1;
	}

	if ( isBikswordstyFlag == 2 )
	{
		bikSpinTimer++;
		//バイクの車輪が動き出す(抜刀)
		if ( bikSpinTimer > 10 )
		{
			bikSpinTimer = 0;
		}
		//通常
		if ( isAtTimerFlag == false && isLeftAtFlag == false && isRightAtFlag == false && isAccelFlag == false )
		{
			if ( bikSpinTimer >= 1 && bikSpinTimer <= 5 )
			{
				Obj_->SetModel(Model_[ ActionState::IDELBladeBefore]);
			}
			else if ( bikSpinTimer >= 6 && bikSpinTimer <= 10 )
			{
				Obj_->SetModel(Model_[ ActionState::IDELBlade ]);
			}
		}

		//加速
		if ( isAccelFlag == true )
		{
			if ( bikSpinTimer >= 1 && bikSpinTimer <= 5 )
			{
				Obj_->SetModel(Model_[ ActionState::Accel]);
			}
			else if ( bikSpinTimer >= 6 && bikSpinTimer <= 10 )
			{
				Obj_->SetModel(Model_[ ActionState::AccelBefore]);
			}
		}
	}
	else if ( isBikswordstyFlag == 0 )
	{
		bikstSpinTimer++;
		//バイクの車輪が動き出す(納刀)
		if ( bikstSpinTimer > 10 )
		{
			bikstSpinTimer = 0;
		}
		if ( isAtTimerFlag == false && isLeftAtFlag == false && isRightAtFlag == false )
		{
			if ( bikstSpinTimer >= 1 && bikstSpinTimer <= 5 )
			{
				Obj_->SetModel(Model_[ ActionState::IDEL]);
			}
			else if ( bikstSpinTimer >= 6 && bikstSpinTimer <= 10 )
			{
				Obj_->SetModel(Model_[ ActionState::IDELBefore]);
			}
		}
	}

	//当たり判定
	if ( isCamShake == 1 )
	{
		DamageCamShake();
	}

	//ゲームクリア時に自機が前に進む
	if ( isClearFlag == true )
	{
		const float addObjPosZ = 0.5f;
		Obj_->wtf.position.z += addObjPosZ;
		isclearFlagTimer++;
	}
	if ( isclearFlagTimer >= 100 )
	{
		isclearFlagTimer = 100;
	}

//ボス登場時のカメラ
	if ( BossCameraResetTimer == 80 )
	{
		if ( isCameraBehavior == 0 )
		{
			isCameraBehavior = 1;
		}
	}
	if ( isCameraBehavior == 1 )
	{
		CameraBehaviorTimer++;
	}
	if ( isCameraBehavior == 1 )
	{
		if ( CameraBehaviorTimer <= 70 )
		{
			const float addcameraPosZ = 0.2f;
			camera->wtf.position.z -= addcameraPosZ;
		}
		if ( CameraBehaviorTimer >= 20 )
		{
			const float addcameraPosY = 0.05f;
			camera->wtf.rotation.y += addcameraPosY;
		}

		if ( camera->wtf.position.z <= -14.0f )
		{
			camera->wtf.position.z = -14.0f;
		}
		if ( camera->wtf.rotation.y >= 2.5f )
		{
			camera->wtf.rotation.y = 2.5f;
			isCameraBehavior = 2;
		}
	}
	if ( isCameraBehavior == 2 )
	{
		CameraBehaviorTimer2++;
		if ( CameraBehaviorTimer2 >= 80 )
		{
			const float addcameraPosZ = 0.2f;
			camera->wtf.position.z += addcameraPosZ;
		}
		if ( CameraBehaviorTimer2 >= 100 )
		{
			const float addcameraPosY = 0.05f;
			camera->wtf.rotation.y -= addcameraPosY;
		}

		if ( camera->wtf.position.z >= 10.0f )
		{
			camera->wtf.position.z = 10.0f;
		}
		if ( camera->wtf.rotation.y <= 0.0f )
		{
			camera->wtf.rotation.y = 0.0f;
			isCameraBehavior = 3;
		}
	}

	//カメラリセット(最初のチュートリアル)
	if ( isDeadEnemy >= 2 )
	{
		cameraResetTimer++;
		if ( cameraResetTimer >= 1 && cameraResetTimer <= 2 )
		{
			if ( standardCamera == 1 )
			{
				standardCamera = 0;
			}
		}
		if ( cameraResetTimer <= 3 )
		{
			cameraResetTimer = 3;
		}
	}
	//カメラリセット(ボス前)
	if ( isRoundFlag == 9 )
	{
		BossCameraResetTimer++;
		if ( BossCameraResetTimer >= 1 && BossCameraResetTimer <= 2 )
		{
			if ( standardCamera == 1 )
			{
				standardCamera = 0;
			}
		}
		if ( BossCameraResetTimer >= 80 )
		{
			BossCameraResetTimer = 80;
		}
	}

	//ラウンド変化(2ラウンド目)
	if ( isDeadEnemy == 2 )
	{
		isRoundFlag = 1;
		waveTimer2++;
	}

	//ラウンド変化(3ラウンド目)
	if ( isDeadEnemy == 4 )
	{
		isRoundFlag = 3;
	}

	//ラウンド変化(4ラウンド目)
	if ( isDeadEnemy == 6 )
	{
		isRoundFlag = 5;
	}

	//ラウンド変化(5ラウンド目)
	if ( isDeadEnemy == 9 )
	{
		isRoundFlag = 7;
	}

	//ラウンド変化(6ラウンド目)
	if ( isDeadEnemy == 13 )
	{
		isRoundFlag = 9;
	}

	//敵が自機より後ろにいるときカメラを少し下げる
	if ( isGameStartTimer >= 250 )
	{
		if ( standardCamera == 0 )
		{
			const float addcameraPosZ = 0.1f;
			camera->wtf.position.z += addcameraPosZ;
			if ( camera->wtf.position.z >= 0.0f )
			{
				camera->wtf.position.z = 0.0f;
			}
		}
		if ( standardCamera == 1 )
		{
			const float addcameraPosZ = 0.1f;
			camera->wtf.position.z -= addcameraPosZ;
			if ( camera->wtf.position.z <= -10.0f )
			{
				camera->wtf.position.z = -10.0f;
			}
		}
	}


	ImGui::Begin("Player");
	ImGui::Text("Position:%f,%f,%f",slashRObj_->wtf.position.x,slashRObj_->wtf.position.y,slashRObj_->wtf.position.z);
	ImGui::Text("bulletgagePosition.x:%f",bulletgagePosition.x);
	ImGui::Text("playerHP:%f",playerHP);
	ImGui::Text("CameraBehaviorTimer:%d",CameraBehaviorTimer);
	ImGui::Text("CameraBehaviorTimer2:%d",CameraBehaviorTimer2);
	ImGui::Text("isDeadEnemy:%d",isDeadEnemy);
	ImGui::Text("isboostFlag:%d",isboostFlag);
	ImGui::End();


	//ImGui::Text("Rotation:%f,%f,%f",Obj_->wtf.rotation.x,Obj_->wtf.rotation.y,Obj_->wtf.rotation.z);
	//ImGui::Text("Position:%f,%f,%f",Obj_->wtf.position.x,Obj_->wtf.position.y,Obj_->wtf.position.z);

	/*ImGui::Text("HPPosion:%f,%f",hpgreenPosition.x,hpgreenPosition.y);
	ImGui::Text("backTimer:%d",backTimer);*/

}

void Player::Draw() {

	if ( isAliveFlag == true )
	{
		Obj_->Draw();
	}

	if ( isGameStartTimer >= 180 )
	{
/*extrusionRightObj_->Draw();
extrusionLeftObj_->Draw();
collObj_->Draw();*/
		if ( isCollSWFlag == true )
		{
/*collSWObj_->Draw();*/
		}
		if ( isCollSWRightFlag == true )
		{
			/*collSWRightObj_->Draw();*/
		}
	}

	if ( isShootFlag == true )
	{
		/*shootObj_->Draw();*/
	}
	for ( int i = 0; i < 3; i++ )
	{
		/*shootObj_[i]->Draw();*/
	}

	if ( isShootStFlag == true )
	{
		shootStObj_->Draw();
	}

	if ( retdisplay == true && isClearFlag == false && OperationbbTimer2 >= 60 )
	{
/*retObj_->Draw();*/
		retVisualObj_->Draw();
	}
	if ( isScatterFlag == true )
	{
		debrisObj_->Draw();
	}

	/*slashRObj_->Draw();
	slashLObj_->Draw();*/
}

void Player::FbxDraw() {

}

void Player::UIInitialize()
{
	//enpty
	bulletEnptyUI = new Sprite();
	bulletEnptyUI->Initialize(spriteCommon);
	bulletEnptyUI->SetPozition({ 0,0 });
	bulletEnptyUI->SetSize({ 1280.0f, 720.0f });

	//タコメータ
	metaUI = new Sprite();
	metaUI->Initialize(spriteCommon);
	metaUI->SetPozition({ 0,0 });
	metaUI->SetSize({ 1280.0f, 720.0f });
	//メーター矢印
	arrowUI = new Sprite();
	arrowUI->Initialize(spriteCommon);
	arrowPosition = arrowUI->GetPosition();
	arrowPosition.x = 1245;
	arrowPosition.y = 700;
	arrowUI->SetPozition(arrowPosition);
	arrowRotation.x = arrowUI->GetRotation();
	arrowRotation.x = -145;
	arrowUI->SetRotation(arrowRotation.x);
	arrowUI->SetSize({ 80.0f, 20.0f });

	//自機のHPのUI
	bullet1UI = new Sprite();
	bullet1UI->Initialize(spriteCommon);
	bullet1UI->SetPozition({ 0,0 });
	bullet1UI->SetSize({ 1280.0f, 720.0f });

	//自機のHP(ハート3)
	bullet2UI = new Sprite();
	bullet2UI->Initialize(spriteCommon);
	bullet2UI->SetPozition({ 0,0 });
	bullet2UI->SetSize({ 1280.0f, 720.0f });

	//自機のHP(ハート2)
	bullet3UI = new Sprite();
	bullet3UI->Initialize(spriteCommon);
	bullet3UI->SetPozition({ 0,0 });
	bullet3UI->SetSize({ 1280.0f, 720.0f });

	//自機のHP(ハート1)
	gageMaxUI = new Sprite();
	gageMaxUI->Initialize(spriteCommon);
	gageMaxUI->SetPozition({ 0,0 });
	gageMaxUI->SetSize({ 1280.0f, 720.0f });

	//自機のHP(ハート0)
	underBlackUI = new Sprite();
	underBlackUI->Initialize(spriteCommon);
	underBlackUI->SetPozition({ 0,0 });
	underBlackUI->SetSize({ 1280.0f, 720.0f });

	//被弾時エフェクト
	BloodUI = new Sprite();
	BloodUI->Initialize(spriteCommon);
	BloodUI->SetPozition({ 0,0 });
	BloodUI->SetSize({ 1280.0f, 720.0f });

	//遠距離攻撃のUI
	bulletUI = new Sprite();
	bulletUI->Initialize(spriteCommon);
	bulletPosition = bulletUI->GetPosition();
	bulletUI->SetPozition(bulletPosition);
	bulletUI->SetSize({ 1280.0f, 720.0f });

	//遠距離攻撃のゲージ
	bulletgageUI = new Sprite();
	bulletgageUI->Initialize(spriteCommon);
	bulletgagePosition = bulletgageUI->GetPosition();
	bulletgagePosition.x = -187.0f;
	bulletgageUI->SetPozition(bulletgagePosition);
	bulletgageUI->SetSize({ 1280.0f, 720.0f });

	//操作説明
	//1ステージ
	operationUI = new Sprite();
	operationUI->Initialize(spriteCommon);
	operationUI->SetPozition({ 0,0 });
	operationUI->SetSize({ 1280.0f, 720.0f });
	//視線誘導(1ステージ)
	operationbbUI = new Sprite();
	operationbbUI->Initialize(spriteCommon);
	operationbbUI->SetPozition({ 0,0 });
	operationbbUI->SetSize({ 1280.0f, 720.0f });

	//2ステージ
	operation2UI = new Sprite();
	operation2UI->Initialize(spriteCommon);
	operation2UI->SetPozition({ 0,0 });
	operation2UI->SetSize({ 1280.0f, 720.0f });
	//視線誘導(2ステージ)
	operationbb2UI = new Sprite();
	operationbb2UI->Initialize(spriteCommon);
	operationbb2UI->SetPozition({ 0,0 });
	operationbb2UI->SetSize({ 1280.0f, 720.0f });


	//画像読み込み
	//メーター
	spriteCommon->LoadTexture(24,"meta.png");
	metaUI->SetTextureIndex(24);

	//矢印
	spriteCommon->LoadTexture(25,"arrow.png");
	arrowUI->SetTextureIndex(25);

	//被弾エフェクト
	spriteCommon->LoadTexture(14,"blood.png");
	BloodUI->SetTextureIndex(14);

	//遠距離攻撃の弾表示
	spriteCommon->LoadTexture(23,"bulletenp.png");
	bulletEnptyUI->SetTextureIndex(23);
	spriteCommon->LoadTexture(15,"bullet1.png");
	bullet1UI->SetTextureIndex(15);
	spriteCommon->LoadTexture(16,"bullet2.png");
	bullet2UI->SetTextureIndex(16);
	spriteCommon->LoadTexture(17,"bullet3.png");
	bullet3UI->SetTextureIndex(17);

	//ゲージMAXの時のUI
	spriteCommon->LoadTexture(18,"gagemax.png");
	gageMaxUI->SetTextureIndex(18);
	spriteCommon->LoadTexture(19,"gageblack.png");
	underBlackUI->SetTextureIndex(19);

	//遠距離攻撃のUI
	spriteCommon->LoadTexture(31,"bulletgage.png");
	bulletUI->SetTextureIndex(31);

	//遠距離攻撃のゲージ
	spriteCommon->LoadTexture(32,"bluegage.png");
	bulletgageUI->SetTextureIndex(32);

	//操作説明
	spriteCommon->LoadTexture(33,"sousa.png");
	operationUI->SetTextureIndex(33);

	spriteCommon->LoadTexture(34,"sousabb.png");
	operationbbUI->SetTextureIndex(34);

	spriteCommon->LoadTexture(35,"sousa2.png");
	operation2UI->SetTextureIndex(35);

	spriteCommon->LoadTexture(36,"sousa2bb.png");
	operationbb2UI->SetTextureIndex(36);
}




void Player::UIDraw()
{
	//被弾時の赤いエフェクト
	if ( isCamShake == 1 )
	{
		BloodUI->Draw();
	}

//HP関連
	if ( isGameStartTimer >= 180 )
	{
/*hpbUI->Draw();
hpredUI->Draw();
hpgreenUI->Draw();
hpFlameUI->Draw();*/
		if ( OperationbbTimer2 >= 60 )
		{
			underBlackUI->Draw();
			bulletgageUI->Draw();
			bulletUI->Draw();

			if ( gageCount == 0 )
			{
				bulletEnptyUI->Draw();
			}
			if ( gageCount == 1 )
			{
				bullet1UI->Draw();
			}
			if ( gageCount == 2 )
			{
				bullet2UI->Draw();
			}
			if ( gageCount >= 3 )
			{
				bullet3UI->Draw();
				gageMaxUI->Draw();
			}
		}

		

		metaUI->Draw();
		arrowUI->Draw();
	}

	//操作説明関連
	if ( isGameStartTimer >= 310 )
	{
		//1ウェーブ目
		operationUI->Draw();
		if ( isOperationFlag == true )
		{
			operationbbUI->Draw();
		}
		if ( isOperationFlag2 == true )
		{
			operation2UI->Draw();
		}
		if ( isOperationFlag3 == true )
		{
			operationbb2UI->Draw();
		}

	}
}

void Player::PlayerAction()
{
	//自機とレティクルの速度
	const float playerSpeed = 0.08f;
	const float playerSpeed2 = 0.06f;
	const float retSpeed = 0.08f;
	const float retSpeed2 = 0.20f;
	////自機とレティクルの画面制限
	//float playerLimitX = 0.6f;
	//float playerLimitY = 0.19f;
	//float playerLimitY2 = 0.35f;
	//float retLimitX = 6.0f;
	//float retLimitY = 3.0f;



	//減速
	if ( isDecelerationFlag == true )
	{
		DecelerationTimer++;
		backTimer++;
		//減速で自機も少し後ろに下がる
		if ( backTimer >= 1 && backTimer <= 30 )
		{
			const float backSpeed = 0.07f;
			Obj_->wtf.position.z -= backSpeed;
		}
		if ( backTimer >= 31 && backTimer <= 90 )
		{
			const float backSpeedZ = 0.03f;
			Obj_->wtf.position.z += backSpeedZ;
			if ( Obj_->wtf.position.z >= -0.3f )
			{
				Obj_->wtf.position.z = -0.3f;
			}
		}
	}
	if ( DecelerationTimer >= 100 )
	{
		isDecelerationFlag = false;
		backTimer = 0;
		DecelerationTimer = 0;
	}

	//自機の攻撃モーション(射撃)
	if ( isAtTimerFlag == false )
	{
		if ( input_->TriggerKey(DIK_SPACE) || input_->PButtonTrigger(RT) )
		{
			isAtTimerFlag = true;
		}
	}
	if ( isAtTimerFlag == true )
	{
		AtTimer++;
		bikSpinTimer = 6;
	}
	if ( AtTimer >= 1 && AtTimer <= 5 )
	{
		Obj_->SetModel(Model_[ ActionState::BulletAttackMid]);
	}
	else if ( AtTimer >= 6 && AtTimer <= 20 )
	{
		Obj_->SetModel(Model_[ ActionState::BulletAttack]);
	}
	else if ( AtTimer >= 21 )
	{
		AtTimer = 0;
		bikSpinTimer++;
		isAtTimerFlag = false;
	}

	//自機の攻撃モーション(左近接攻撃)
	if ( isLeftAtFlag == false )
	{
		if ( input_->TriggerKey(DIK_Q) || input_->PButtonTrigger(LB) )
		{
			isLeftAtFlag = true;
			isCollSWFlag = true;
			if ( spineLffflag == false )
			{
				spineLffflag = true;
			}
		}
	}
	if ( isLeftAtFlag == true )
	{
		Obj_->wtf.rotation.z = 0.0f;
		leftAtTimer++;
		bikSpinTimer = 6;
	}
	if ( leftAtTimer >= 1 && leftAtTimer < 30 )
	{
		Obj_->SetModel(Model_[ ActionState::LSpinAttack]);
		const float LeftAtSpeed = 0.32f;
		Obj_->wtf.rotation.y += LeftAtSpeed;
		slashLObj_->wtf.position.z += 0.2f;
		slashLObj_->wtf.position.x += 0.03f;
		isLSpinEffFlag_ = 1;
	}
	else if ( leftAtTimer >= 30 )
	{
		leftAtTimer = 0;
		bikSpinTimer++;
		Obj_->wtf.rotation.y = 0.0f;
		isCollSWFlag = false;
		isLeftAtFlag = false;
		spineLffflag = false;
		isLSpinEffFlag_ = 0;
	}

	//自機の攻撃モーション(右近接攻撃)
	if ( isRightAtFlag == false )
	{
		if ( input_->TriggerKey(DIK_E) || input_->PButtonTrigger(RB) )
		{
			isRightAtFlag = true;
			isCollSWRightFlag = true;
			if ( spineRffflag == false )
			{
				spineRffflag = true;
			}
		}
	}
	if ( isRightAtFlag == true )
	{
		Obj_->wtf.rotation.z = 0.0f;
		rightAtTimer++;
		bikSpinTimer = 6;
	}
	if ( rightAtTimer >= 1 && rightAtTimer < 30 )
	{
		Obj_->SetModel(Model_[ ActionState::RSpinAttack]);
		const float rightAtSpeed = 0.32f;
		Obj_->wtf.rotation.y -= rightAtSpeed;
		slashRObj_->wtf.position.z += 0.2f;
		slashRObj_->wtf.position.x -= 0.03f;
		isRSpinEffFlag_ = 1;
	}
	else if ( rightAtTimer >= 30 )
	{
		rightAtTimer = 0;
		bikSpinTimer++;
		Obj_->wtf.rotation.y = 0.0f;
		isCollSWRightFlag = false;
		isRightAtFlag = false;
		spineRffflag = false;
		isRSpinEffFlag_ = 0;
	}



	//自機が左右に動いたらモデルも傾く
	if ( input_->PushKey(DIK_D) || input_->StickInput(L_RIGHT) )
	{
		const float Speed = 0.03f;
		Obj_->wtf.rotation.z -= Speed;
		if ( Obj_->wtf.rotation.z <= -0.4f )
		{
			Obj_->wtf.rotation.z = -0.4f;
		}
	}
	else if ( input_->PushKey(DIK_A) || input_->StickInput(L_LEFT) )
	{
		const float Speed = 0.03f;
		Obj_->wtf.rotation.z += Speed;
		if ( Obj_->wtf.rotation.z >= 0.4f )
		{
			Obj_->wtf.rotation.z = 0.4f;
		}
	}
	else
	{
		Obj_->wtf.rotation.z = 0.0f;
	}

	////自機のスライディング
	//if ( isbikslidFlag == false )
	//{
	//	if (input_->TriggerKey(DIK_X) ){isbikslidFlag = true;}
	//}
	//if ( isbikslidFlag == true ){
	//	bikslidTimer++;
	//	bikSpinTimer = 6;
	//}
	//if ( bikslidTimer >= 1 && bikslidTimer <= 30 ){
	//	Obj_->wtf.rotation.y = -1.5f;
	//	Obj_->wtf.rotation.x = -1.0f;
	//}
	//if ( bikslidTimer >= 31 ){
	//	Obj_->wtf.rotation.y = 0.0f;
	//	Obj_->wtf.rotation.x = 0.0f;
	//	bikslidTimer = 0;
	//	bikSpinTimer++;
	//	isbikslidFlag = false;
	//}


	//移動(自機)
	if ( isJumpFlag == false && isVertFlag == false )
	{
		if ( input_->TriggerKey(DIK_Z) )
		{
			isJumpFlag = true;
		}
	}
	if ( input_->PushKey(DIK_A) || input_->StickInput(L_LEFT) )
	{
		if ( limitmove == true )
		{
			Obj_->wtf.position.x -= 0.0f;
			collObj_->wtf.position.x -= 0.0f;
			retObj_->wtf.position.x += 0.0f;
			camera->wtf.position.x -= 0.0f;
		}
		else
		{
			limitmove2 = false;
			Obj_->wtf.position.x -= playerSpeed;
			collObj_->wtf.position.x -= playerSpeed;
			retObj_->wtf.position.x += playerSpeed2;
			/*camera->wtf.position.x -= 0.01f;*/
		}
	}
	if ( input_->PushKey(DIK_D) || input_->StickInput(L_RIGHT) )
	{
		if ( limitmove2 == true )
		{
			Obj_->wtf.position.x -= 0.0f;
			collObj_->wtf.position.x -= 0.0f;
			retObj_->wtf.position.x += 0.0f;
			camera->wtf.position.x -= 0.0f;
		}
		else
		{
			limitmove = false;
			Obj_->wtf.position.x += playerSpeed;
			collObj_->wtf.position.x += playerSpeed;
			retObj_->wtf.position.x -= playerSpeed2;
			/*camera->wtf.position.x += 0.01f;*/
		}
	}

	if ( input_->PushKey(DIK_W) || input_->StickInput(L_UP) )
	{
		limitmove = false;
		limitmove2 = false;
		isboostFlag = 1;
		isArrowUpFlag = true;
	}
	else if ( input_->PushKey(DIK_S) || input_->StickInput(L_DOWN) )
	{
		limitmove = false;
		limitmove2 = false;
		isboostFlag = 2;
		isArrowDwonFlag = true;
	}
	else
	{
		isboostFlag = 0;
		isArrowUpFlag = false;
		isArrowDwonFlag = false;
	}

	if ( input_->StickInput(L_UP) && isRightAtFlag == false && isLeftAtFlag == false && isAtTimerFlag == false )
	{
		/*&& input_->StickInput(L_UP)*/
		isAccelFlag = true;
	}
	else
	{
		isAccelFlag = false;
	}

	//移動(レティクル)
	if ( input_->PushKey(DIK_UP) || input_->StickInput(R_UP) )
	{
		retObj_->wtf.position.y += retSpeed2;
		retVisualObj_->wtf.position.y += retSpeed;
	}
	if ( input_->PushKey(DIK_DOWN) || input_->StickInput(R_DOWN) )
	{
		retObj_->wtf.position.y -= retSpeed2;
		retVisualObj_->wtf.position.y -= retSpeed;
	}
	if ( input_->PushKey(DIK_LEFT) || input_->StickInput(R_LEFT) )
	{
		retObj_->wtf.position.x -= retSpeed2;
		retVisualObj_->wtf.position.x -= retSpeed;
	}
	if ( input_->PushKey(DIK_RIGHT) || input_->StickInput(R_RIGHT) )
	{
		retObj_->wtf.position.x += retSpeed2;
		retVisualObj_->wtf.position.x += retSpeed;
	}

	//弾発射(弱)
	float ShortSpeed = 0.01f;
	if ( input_->TriggerKey(DIK_SPACE) || input_->PButtonTrigger(RT) )
	{
		if ( isShootFlag == false )
		{
			isShootFlag = true;
			isgageStopFlag = true;
		}
	}
	if ( BulletCoolTime == 0 )
	{
		bulletlen_[0] = retObj_->wtf.position - shootObj_[0]->wtf.position;
		bulletlen_[0].nomalize();

		bulletlen_[1] = retObj_->wtf.position - shootObj_[ 1 ]->wtf.position;
		bulletlen_[1].nomalize();

		bulletlen_[2] = retObj_->wtf.position - shootObj_[ 2 ]->wtf.position;
		bulletlen_[2].nomalize();

	}
	if ( isShootFlag == true )
	{
		BulletCoolTime++;
		if ( BulletCount >= 1 )
		{
			shootObj_[ 0 ]->wtf.rotation.z += 0.2f;
			shootObj_[ 0 ]->wtf.position += bulletlen_[ 0 ];
			len_[ 0 ] = bulletlen_[ 0 ];
			len_[ 0 ] *= ShortSpeed;
		}

		if ( BulletCoolTime >= 10)
		{
			if ( BulletCount >= 2 )
			{
				shootObj_[ 1 ]->wtf.rotation.z += 0.2f;
				shootObj_[ 1 ]->wtf.position += bulletlen_[ 1 ];
				len_[ 1 ] = bulletlen_[ 1 ];
				len_[ 1 ] *= ShortSpeed;
			}
		}

		if ( BulletCoolTime >= 20 )
		{
			if ( BulletCount >= 3 )
			{
				shootObj_[ 2 ]->wtf.rotation.z += 0.2f;
				shootObj_[ 2 ]->wtf.position += bulletlen_[ 2 ];
				len_[ 2 ] = bulletlen_[ 2 ];
				len_[ 2 ] *= ShortSpeed;
			}
		}

		isBallisticEffFlag_ = 1;
	}
	else
	{ 
		shootObj_[0]->wtf.position = {Obj_->wtf.position.x,Obj_->wtf.position.y, Obj_->wtf.position.z +1.0f};
		shootObj_[1]->wtf.position = { Obj_->wtf.position.x - 1.0f,Obj_->wtf.position.y - 0.4f, Obj_->wtf.position.z+1.0f};
		shootObj_[2]->wtf.position = { Obj_->wtf.position.x + 1.0f,Obj_->wtf.position.y - 0.8f, Obj_->wtf.position.z+1.0f};
		isBallisticEffFlag_ = 0;
	}
	if ( BulletCoolTime >= 55.0f )
	{
		BulletCoolTime = 0;
		isShootFlag = false;
		isBallisticEffFlag_ = 0;
		isgageStopFlag = false;
		gageCount = 1;
		BulletCount = 1;
	}

	//残り残数のUI表示
	if ( isgageStopFlag == true )
	{
		gageCount = 0;
		bulletgagePosition.x = -187.0f;
		bulletgageUI->SetPozition(bulletgagePosition);
	}
	else
	{
		if ( OperationbbTimer2 >= 60 )
		{
			bulletgagePosition.x += 0.8f;
			bulletgageUI->SetPozition(bulletgagePosition);
		}
	}
	if ( bulletgagePosition.x >= 0.0f )
	{
		gageCount++;
		BulletCount++;
		bulletgagePosition.x = -187.0f;
		bulletgageUI->SetPozition(bulletgagePosition);
	}
	if ( gageCount >= 3 )
	{
		gageCount = 3;
		bulletgagePosition.x = 0.0f;
		bulletgageUI->SetPozition(bulletgagePosition);
	}



	//突進を受けた時にノックバック(右からの突進)
	if ( isKnockbackFlag == true )
	{
		knockbackTimer++;
	}
	if ( knockbackTimer >= 1 )
	{
		Obj_->wtf.position.x -= 0.03f;
	}
	if ( knockbackTimer >= 21 )
	{
		knockbackTimer = 0;
		isKnockbackFlag = false;
	}
	//突進を受けた時にノックバック(左からの突進)
	if ( isKnockbackFlagL == true )
	{
		knockbackTimerL++;
	}
	if ( knockbackTimerL >= 1 )
	{
		Obj_->wtf.position.x += 0.03f;
	}
	if ( knockbackTimerL >= 21 )
	{
		knockbackTimerL = 0;
		isKnockbackFlagL = false;
	}

	//エンジンブルブルする
	arrowTimer++;
	if ( arrowTimer >= 6 )
	{
		arrowTimer = 0;
	}
	if ( arrowTimer >= 0 && arrowTimer <= 3 )
	{
		arrowRotation.x += 1.0f;
		arrowUI->SetRotation(arrowRotation.x);
	}
	if ( arrowTimer >= 4 && arrowTimer <= 6 )
	{
		arrowRotation.x -= 2.0f;
		arrowUI->SetRotation(arrowRotation.x);
	}
	//エンジン動く
	if ( isArrowUpFlag == true )
	{
		arrowRotation.x += 1.0f;
		arrowUI->SetRotation(arrowRotation.x);
		if ( arrowRotation.x >= -85.0f )
		{
			arrowRotation.x = -85.0f;
		}
	}
	if ( isArrowDwonFlag == true )
	{
		arrowRotation.x -= 1.0f;
		arrowUI->SetRotation(arrowRotation.x);
		if ( arrowRotation.x <= -190.0f )
		{
			arrowRotation.x = -190.0f;
		}
	}
	if ( isArrowUpFlag == false && isArrowDwonFlag == false )
	{
		if ( arrowRotation.x >= -146.0f )
		{
			arrowRotation.x -= 1.4f;
			arrowUI->SetRotation(arrowRotation.x);
			if ( arrowRotation.x <= -145.0f )
			{
				arrowRotation.x = -145.0f;
			}
		}

		if ( arrowRotation.x <= -144.0f )
		{
			arrowRotation.x += 1.4f;
			arrowUI->SetRotation(arrowRotation.x);
			if ( arrowRotation.x >= -145.0f )
			{
				arrowRotation.x = -145.0f;
			}
		}

	}

}

void Player::EffUpdate()
{
	//バイクのエンジン
	if ( isbulletEffFlag_ == 1 )
	{
		bulletEffTimer_++;
	}
	if ( bulletEffTimer_ <= 20 && bulletEffTimer_ >= 1 )
	{
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
	if ( bulletEffTimer_ >= 20 )
	{
		isbulletEffFlag_ = 0;
		bulletEffTimer_ = 0;
	}

	//自機の弾
	if ( isBallisticEffFlag_ == 1 )
	{
		ballisticEffTimer_++;
	}
	if ( ballisticEffTimer_ <= 10 && ballisticEffTimer_ >= 0 )
	{
		EffSummaryBullet(Vector3(shootObj_[0]->wtf.position.x,shootObj_[0]->wtf.position.y + 0.5f,shootObj_[0]->wtf.position.z));
		EffSummaryBullet2(Vector3(shootObj_[1]->wtf.position.x,shootObj_[1]->wtf.position.y + 0.5f,shootObj_[1]->wtf.position.z));
		EffSummaryBullet3(Vector3(shootObj_[2]->wtf.position.x,shootObj_[2]->wtf.position.y + 0.5f,shootObj_[2]->wtf.position.z));
	}
	if ( ballisticEffTimer_ >= 10 )
	{
		isBallisticEffFlag_ = 0;
		ballisticEffTimer_ = 0;
	}

	
	//スピン
	if ( isRSpinEffFlag_ == 1 )
	{
		RSpinEffTimer_++;
	}
	if ( RSpinEffTimer_ <= 10 && RSpinEffTimer_ >= 0 )
	{
		EffSummaryRSpin(Vector3(slashRObj_->wtf.position.x,slashRObj_->wtf.position.y + 0.5f,slashRObj_->wtf.position.z));
	}
	if ( RSpinEffTimer_ >= 10 )
	{
		isRSpinEffFlag_ = 0;
		RSpinEffTimer_ = 0;
	}

	if ( isLSpinEffFlag_ == 1 )
	{
		LSpinEffTimer_++;
	}
	if ( LSpinEffTimer_ <= 10 && LSpinEffTimer_ >= 0 )
	{
		EffSummaryLSpin(Vector3(slashLObj_->wtf.position.x,slashLObj_->wtf.position.y + 0.5f,slashLObj_->wtf.position.z));
	}
	if ( LSpinEffTimer_ >= 10 )
	{
		isLSpinEffFlag_ = 0;
		LSpinEffTimer_ = 0;
	}

}

void Player::EffSummary(Vector3 bulletpos)
{
	//パーティクル範囲
	for ( int i = 0; i < 20; i++ )
	{
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
	for ( int i = 0; i < 20; i++ )
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
	for ( int i = 0; i < 50; i++ )
	{
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
	for ( int i = 0; i < 5; i++ )
	{
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
	for ( int i = 0; i < 5; i++ )
	{
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

void Player::EffSummarySwordchage(Vector3 pos)
{
	//パーティクル範囲
	for ( int i = 0; i < 5; i++ )
	{
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_posG = 0.03f;
		const float rnd_posGy = 0.03f;
		const float rnd_posGz = 0.03f;
		Vector3 posG{};
		posG.x += ( float ) rand() / RAND_MAX * rnd_posG - rnd_posG / 2.0f;
		posG.y += ( float ) rand() / RAND_MAX * rnd_posGy - rnd_posGy / 2.0f;
		posG.z += ( float ) rand() / RAND_MAX * rnd_posGz - rnd_posGz / 2.0f;
		posG += pos;
		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		const float rnd_velG = 0.01f;
		const float rnd_velGy = 0.01f;
		const float rnd_velGz = 0.01f;
		Vector3 velG{};
		velG.x = ( float ) rand() / RAND_MAX * rnd_velG - rnd_velG / 2.0f;
		velG.y = ( float ) rand() / RAND_MAX * rnd_velGy - rnd_velGy / 2.0f;
		velG.z = ( float ) rand() / RAND_MAX * rnd_velGz - rnd_velGz / 2.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		const float rnd_accG = 0.000001f;
		Vector3 accG{};
		accG.x = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;
		accG.y = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;

		//追加
		swordchageParticle->Add(60,posG,velG,accG,0.3f,0.0f);

		swordchageParticle->Update();

	}
}

void Player::EffSummaryBullet(Vector3 bulletpos)
{
	//パーティクル範囲
	for ( int i = 0; i < 5; i++ )
	{
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
	for ( int i = 0; i < 5; i++ )
	{
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
	for ( int i = 0; i < 5; i++ )
	{
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
	for ( int i = 0; i < 5; i++ )
	{
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
	for ( int i = 0; i < 5; i++ )
	{
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
	if ( isbulletEffFlag_ == 1 )
	{
		if ( isboostFlag == 0 )
		{
			gasParticle->Draw();
			gasParticle2->Draw();
		}
		else if ( isboostFlag == 1 )
		{
			gasParticleAccelR->Draw();
			gasParticleAccelL->Draw();
		}
		else if ( isboostFlag == 2 )
		{
			/*gasParticleDecelR->Draw();
			gasParticleDecelL->Draw();*/
		}
	}

	if ( isswordchageEffFlag_ == 1 )
	{
		/*swordchageParticle->Draw();*/
	}

	if ( isBallisticEffFlag_ == 1 )
	{
		ballisticParticle_[ 0 ]->Draw();
		if ( BulletCount >= 2 )
		{
			ballisticParticle_[ 1 ]->Draw();
		}
		if ( BulletCount >= 3 )
		{
			ballisticParticle_[ 2 ]->Draw();
		}
	}

	playerEffect->Draw();

	if ( isRSpinEffFlag_ == 1 && RSpinEffTimer_ <= 10 && RSpinEffTimer_ >= 1 )
	{
		RSpinParticle->Draw();
	}

	if ( isLSpinEffFlag_ == 1 && LSpinEffTimer_ <= 10 && LSpinEffTimer_ >= 1 )
	{
		LSpinParticle->Draw();
	}

}

Vector3 Player::bVelocity(Vector3& velocity,Transform& worldTransform)
{
	Vector3 result = { 0,0,0 };

	//内積
	result.z = velocity.x * worldTransform.matWorld.m[ 0 ][ 2 ] +
		velocity.y * worldTransform.matWorld.m[ 1 ][ 2 ] +
		velocity.z * worldTransform.matWorld.m[ 2 ][ 2 ];

	result.x = velocity.x * worldTransform.matWorld.m[ 0 ][ 0 ] +
		velocity.y * worldTransform.matWorld.m[ 1 ][ 0 ] +
		velocity.z * worldTransform.matWorld.m[ 2 ][ 0 ];

	result.y = velocity.x * worldTransform.matWorld.m[ 0 ][ 1 ] +
		velocity.y * worldTransform.matWorld.m[ 1 ][ 1 ] +
		velocity.z * worldTransform.matWorld.m[ 2 ][ 1 ];

	return result;
}

Vector3 Player::GetWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos;

	collObj_->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	worldPos.x = collObj_->wtf.matWorld.m[ 3 ][ 0 ];
	worldPos.y = collObj_->wtf.matWorld.m[ 3 ][ 1 ];
	worldPos.z = collObj_->wtf.matWorld.m[ 3 ][ 2 ];

	return worldPos;
}

Vector3 Player::GetBulletWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 BulletWorldPos;

	shootObj_[0]->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	BulletWorldPos.x = shootObj_[0]->wtf.matWorld.m[ 3 ][ 0 ];
	BulletWorldPos.y = shootObj_[0]->wtf.matWorld.m[ 3 ][ 1 ];
	BulletWorldPos.z = shootObj_[0]->wtf.matWorld.m[ 3 ][ 2 ];

	return BulletWorldPos;
}

Vector3 Player::GetBulletWorldPosition2()
{
	//ワールド座標を入れる変数
	Vector3 BulletWorldPos1;

	shootObj_[ 1 ]->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	BulletWorldPos1.x = shootObj_[ 1 ]->wtf.matWorld.m[ 3 ][ 0 ];
	BulletWorldPos1.y = shootObj_[ 1 ]->wtf.matWorld.m[ 3 ][ 1 ];
	BulletWorldPos1.z = shootObj_[ 1 ]->wtf.matWorld.m[ 3 ][ 2 ];

	return BulletWorldPos1;
}

Vector3 Player::GetBulletWorldPosition3()
{
	//ワールド座標を入れる変数
	Vector3 BulletWorldPos2;

	shootObj_[ 2 ]->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	BulletWorldPos2.x = shootObj_[ 2 ]->wtf.matWorld.m[ 3 ][ 0 ];
	BulletWorldPos2.y = shootObj_[ 2 ]->wtf.matWorld.m[ 3 ][ 1 ];
	BulletWorldPos2.z = shootObj_[ 2 ]->wtf.matWorld.m[ 3 ][ 2 ];

	return BulletWorldPos2;
}

Vector3 Player::GetSwordLeftWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 SwordWorldPos;

	collSWObj_->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	SwordWorldPos.x = collSWObj_->wtf.matWorld.m[ 3 ][ 0 ];
	SwordWorldPos.y = collSWObj_->wtf.matWorld.m[ 3 ][ 1 ];
	SwordWorldPos.z = collSWObj_->wtf.matWorld.m[ 3 ][ 2 ];

	return SwordWorldPos;
}

Vector3 Player::GetSwordRightWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 SwordRightWorldPos;

	collSWRightObj_->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	SwordRightWorldPos.x = collSWRightObj_->wtf.matWorld.m[ 3 ][ 0 ];
	SwordRightWorldPos.y = collSWRightObj_->wtf.matWorld.m[ 3 ][ 1 ];
	SwordRightWorldPos.z = collSWRightObj_->wtf.matWorld.m[ 3 ][ 2 ];

	return SwordRightWorldPos;
}

Vector3 Player::GetCollLeftWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 collLeftWorldPos;

	extrusionLeftObj_->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	collLeftWorldPos.x = extrusionLeftObj_->wtf.matWorld.m[ 3 ][ 0 ];
	collLeftWorldPos.y = extrusionLeftObj_->wtf.matWorld.m[ 3 ][ 1 ];
	collLeftWorldPos.z = extrusionLeftObj_->wtf.matWorld.m[ 3 ][ 2 ];

	return collLeftWorldPos;
}

Vector3 Player::GetCollRightWorldPosition()
{
		//ワールド座標を入れる変数
	Vector3 collRightWorldPos;

	extrusionRightObj_->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	collRightWorldPos.x = extrusionRightObj_->wtf.matWorld.m[ 3 ][ 0 ];
	collRightWorldPos.y = extrusionRightObj_->wtf.matWorld.m[ 3 ][ 1 ];
	collRightWorldPos.z = extrusionRightObj_->wtf.matWorld.m[ 3 ][ 2 ];

	return collRightWorldPos;
}

Vector3 Player::GetRetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 RetWorldPos;

	retObj_->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	RetWorldPos.x = retObj_->wtf.matWorld.m[ 3 ][ 0 ];
	RetWorldPos.y = retObj_->wtf.matWorld.m[ 3 ][ 1 ];
	RetWorldPos.z = retObj_->wtf.matWorld.m[ 3 ][ 2 ];

	return RetWorldPos;
}

void Player::GameStartMovie()
{
	if ( isGameStartFlag == true )
	{
		acflag = true;
		camerasetFlag = true;
		isGameStartTimer++;
		bikstSpinTimer++;
	}

	if ( camerasetFlag == true )
	{
		camerasetFlag = false;
	}

	if ( acflag == true )
	{
		const float PosSpeed = 0.3f;
		Obj_->wtf.position.z += PosSpeed;
	}

	if ( isGameStartTimer >= 60 )
	{
		rotaflag = true;
	}

	if ( isGameStartTimer <= 180 && rotaflag == true )
	{
		const float RotSpeed = 0.05f;
		camera->wtf.rotation.y -= RotSpeed;
	}

	if ( isGameStartTimer <= 180 && camera->wtf.rotation.y <= 0.2f && rotaflag == true )
	{
		const float RotSpeed = 0.2f;
		camera->wtf.rotation.y = RotSpeed;
		rotaflag = false;
	}

	if ( isGameStartTimer >= 179 && isGameStartTimer <= 180 )
	{
		Obj_->wtf.position.z = 0.0f;
	}
	if ( isGameStartTimer >= 180 )
	{
		acflag = false;
		const float PosSpeed = 0.3f;
		Obj_->wtf.position.z -= PosSpeed;
		retdisplay = true;

	}

	if ( isGameStartTimer >= 180 && isGameStartTimer <= 220 )
	{
		camera->wtf.rotation.y = 0.0f;
	}

	if ( isGameStartTimer >= 220 )
	{
		isBikswordstyFlag = 1;
	}

	if ( isBikswordstyFlag == 1 )
	{
		BikswordstyTimer++;
	}

	if ( BikswordstyTimer >= 1 && BikswordstyTimer <= 5 )
	{
		Obj_->SetModel(Model_[ ActionState::ModeChangeMid]);
	}
	else if ( BikswordstyTimer >= 6 && BikswordstyTimer <= 11 )
	{
		Obj_->SetModel(Model_[ ActionState::ModeChange]);
	}

	if ( BikswordstyTimer >= 12 )
	{
		isBikswordstyFlag = 2;
	}

}

void Player::DamageCamShake()
{
	//画面シェイク
	if ( isCamShake == 1 )
	{
		const float shakePos = 0.2f;
		const float shakePosY = 0.1f;
		camShakeTimer--;
		if ( camShakeTimer <= camShakeLimit && camShakeTimer > camShakeLimit * 3 / 4 )
		{
			camera->wtf.position.y += shakePosY;
			/*camera->wtf.position.z += 0.1f;*/
		}
		else if ( camShakeTimer <= camShakeLimit * 3 / 4 && camShakeTimer > camShakeLimit * 2 / 4 )
		{
			camera->wtf.position.y -= shakePos;
			/*camera->wtf.position.z -= 0.1f;*/
		}
		else if ( camShakeTimer <= camShakeLimit * 2 / 4 && camShakeTimer > camShakeLimit * 1 / 4 )
		{
			camera->wtf.position.y += shakePos;
			/*camera->wtf.position.z += 0.1f;*/
		}
		else if ( camShakeTimer <= camShakeLimit * 1 / 4 && camShakeTimer > 0 )
		{
			camera->wtf.position.y -= shakePos;
			/*camera->wtf.position.z -= 0.1f;*/
		}
		else if ( camShakeTimer <= 0 )
		{
			isCamShake = 0;
			camera->wtf.position.y = 0.0f;
			/*camera->wtf.position.z = 0.0f;*/
		}
	}
}




