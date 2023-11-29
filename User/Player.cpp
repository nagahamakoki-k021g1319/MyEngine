#include"Player.h"
#include"Enemy.h"
#include <imgui.h>
#include "Enemy/ArmorEnemy.h"

Player::Player() {

}

Player::~Player() {
	delete spriteCommon;
	//FBXオブジェクト解放
	delete shootObj_;
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

	delete hpgageUI;
	delete hp3UI;
	delete hp2UI;
	delete hp1UI;
	delete overUI;

	delete hpFlameUI;

	delete hpbUI;
	delete hpgreenUI;
	delete hpredUI;

	delete Bullet2dUI;
	delete Bullet2fUI;
	delete Bullet2mUI;

	delete Bullet3dUI;
	delete Bullet3fUI;
	delete Bullet3mUI;

	delete Bullet4dUI;
	delete Bullet4fUI;
	delete Bullet4mUI;

	delete Bullet5dUI;
	delete Bullet5fUI;
	delete Bullet5mUI;

	delete Bullet6dUI;
	delete Bullet6fUI;
	delete Bullet6mUI;

	delete Obj_;
	delete Model_;
	delete Model2_;
	delete Model3_;
	delete ModelAt_;
	delete ModelBefo_;
	delete ModelBack_;
	delete Modelst_;
	delete Modelst2_;
	delete ModelBikswordsty_;
	delete ModelBikswordsty2_;

	delete Modelbiksword0_;
	delete Modelbiksword1_;

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

	delete entryani1UI;
	delete entryani2UI;

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
	Model_ = Model::LoadFromOBJ("bik");
	ModelBefo_ = Model::LoadFromOBJ("bik2");

	//攻撃
	Model2_ = Model::LoadFromOBJ("bikmid");
	ModelAt_ = Model::LoadFromOBJ("bikAt");

	//自機納刀
	Modelst_ = Model::LoadFromOBJ("bikst");
	Modelst2_ = Model::LoadFromOBJ("bikst2");
	ModelBikswordsty_ = Model::LoadFromOBJ("bikswordsty");
	ModelBikswordsty2_ = Model::LoadFromOBJ("bikswordsty2");

	//近接攻撃(左側)
	Modelbiksword0_ = Model::LoadFromOBJ("bikswordAt");
	Modelbiksword1_ = Model::LoadFromOBJ("bikswordAt");

	//自機
	Obj_ = Object3d::Create();
	Obj_->SetModel(Modelst_);
	Obj_->wtf.scale = { 0.4f,0.4f,0.4f };
	Obj_->wtf.position = { 0.0f,-2.0f,-20.0f };

	//自機の当たり判定のモデル
	collModel_ = Model::LoadFromOBJ("collboll");
	collObj_ = Object3d::Create();
	collObj_->SetModel(collModel_);
	collObj_->wtf.position = { Obj_->wtf.position.x,Obj_->wtf.position.y + 0.5f,-1.0f };

	//自機の近接攻撃判定のモデル(左)
	collSWModel_ = Model::LoadFromOBJ("collboll");
	collSWObj_ = Object3d::Create();
	collSWObj_->SetModel(collSWModel_);
	collSWObj_->wtf.position = { Obj_->wtf.position.x - 0.5f,Obj_->wtf.position.y + 0.5f,-1.0f };

	//自機の近接攻撃判定のモデル(左)
	collSWRightModel_ = Model::LoadFromOBJ("collboll");
	collSWRightObj_ = Object3d::Create();
	collSWRightObj_->SetModel(collSWRightModel_);
	collSWRightObj_->wtf.position = { Obj_->wtf.position.x + 0.5f,Obj_->wtf.position.y + 0.5f,-1.0f };

	//自機が衝突した時用のモデル
	//右
	extrusionRightModel_ = Model::LoadFromOBJ("collboll");
	extrusionRightObj_ = Object3d::Create();
	extrusionRightObj_->SetModel(extrusionRightModel_);
	extrusionRightObj_->wtf.position = { Obj_->wtf.position.x + 0.2f,Obj_->wtf.position.y + 0.5f,-1.0f };
	//左
	extrusionLeftModel_ = Model::LoadFromOBJ("collboll");
	extrusionLeftObj_ = Object3d::Create();
	extrusionLeftObj_->SetModel(extrusionLeftModel_);
	extrusionLeftObj_->wtf.position = { Obj_->wtf.position.x - 0.2f,Obj_->wtf.position.y + 0.5f,-1.0f };

	//自機の弾(弱)
	shootModel_ = Model::LoadFromOBJ("boll2");
	shootObj_ = Object3d::Create();
	shootObj_->SetModel(shootModel_);
	shootObj_->wtf.position = { Obj_->wtf.position.x,Obj_->wtf.position.y, Obj_->wtf.position.z };
	shootObj_->wtf.scale = { 5.0f,5.0f,5.0f };

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

	//パーティクル生成
	gasParticle = std::make_unique<ParticleManager>();
	gasParticle.get()->Initialize();
	gasParticle->LoadTexture("gas.png");
	gasParticle->Update();

	gasParticle2 = std::make_unique<ParticleManager>();
	gasParticle2.get()->Initialize();
	gasParticle2->LoadTexture("gas.png");
	gasParticle2->Update();

	//UIの初期化(枚数が多いため)
	UIInitialize();


}



void Player::Update() {
	camera->Update();
	shootObj_->Update();
	shootStObj_->Update();
	retObj_->Update();
	
	enemylen2 = retObj_->wtf.position - shootStObj_->wtf.position;
	enemylen2.nomalize();
	retVisualObj_->Update();
	Obj_->Update();
	EffUpdate();
	collObj_->Update();
	collSWObj_->Update();
	collSWObj_->wtf.position = { Obj_->wtf.position.x - 1.0f,Obj_->wtf.position.y + 0.5f,-1.0f };
	collSWRightObj_->Update();
	collSWRightObj_->wtf.position = { Obj_->wtf.position.x + 1.0f,Obj_->wtf.position.y + 0.5f,-1.0f };
	extrusionRightObj_->Update();
	extrusionRightObj_->wtf.position = { Obj_->wtf.position.x + 0.2f,Obj_->wtf.position.y + 0.5f,-1.0f };
	extrusionLeftObj_->Update();
	extrusionLeftObj_->wtf.position = { Obj_->wtf.position.x - 0.2f,Obj_->wtf.position.y + 0.5f,-1.0f };

	isGameStartFlag = true;

	//ゲームが始まる
	GameStartMovie();

	//ダメージを受けた時のHP減少
	//if ( input_->TriggerKey(DIK_4) ){
	//	hpgreenPosition.x -= 20.0f;
	//	hpgreenUI->SetPozition(hpgreenPosition);
	//}


	//プレイヤーの行動一覧
	PlayerAction();
	if ( isbikslidFlag == true || isLeftAtFlag == true || isRightAtFlag == true ){
		isbulletEffFlag_ = 0;
		bulletEffTimer_ = 0;
	}
	else{isbulletEffFlag_ = 1;}

	if ( isBikswordstyFlag == 2){
		bikSpinTimer++;
		//バイクの車輪が動き出す(抜刀)
		if ( bikSpinTimer > 10 )
		{
			bikSpinTimer = 0;
		}
		if ( isAtTimerFlag == false && isLeftAtFlag == false && isRightAtFlag == false )
		{
			if ( bikSpinTimer >= 1 && bikSpinTimer <= 5 )
			{
				Obj_->SetModel(ModelBefo_);
			}
			else if ( bikSpinTimer >= 6 && bikSpinTimer <= 10 )
			{
				Obj_->SetModel(Model_);
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
				Obj_->SetModel(Modelst_);
			}
			else if ( bikstSpinTimer >= 6 && bikstSpinTimer <= 10 )
			{
				Obj_->SetModel(Modelst2_);
			}
		}
	}

	//当たり判定
	if ( isCamShake == 1 ){DamageCamShake();}
	

	//ゲームクリア時に自機が前に進む
	if ( isClearFlag == true ){
		Obj_->wtf.position.z += 0.5f;
		isclearFlagTimer++;
	}
	if ( isclearFlagTimer >= 100 ){isclearFlagTimer = 100;}

	//ボス登場時のカメラ
	if ( input_->TriggerKey(DIK_4) ){isCameraBehavior = 1;}
	if ( isCameraBehavior == 1 ){CameraBehaviorTimer++;}
	if ( isCameraBehavior == 1 && isRoundFlag == 6 )
	{
		if ( CameraBehaviorTimer <= 70 ){
			camera->wtf.position.z -= 0.2f;
		}
		if ( CameraBehaviorTimer >= 20 ){
			camera->wtf.rotation.y += 0.05f;
		}

		if ( camera->wtf.position.z <= -14.0f){camera->wtf.position.z = -14.0f;}
		if ( camera->wtf.rotation.y >= 2.5f ){
			camera->wtf.rotation.y = 2.5f;
			isCameraBehavior = 2;
		}
	}
	if ( isCameraBehavior == 2 && isRoundFlag == 6 ){
		CameraBehaviorTimer2++;
		if ( CameraBehaviorTimer2 >= 80)
		{
			camera->wtf.position.z += 0.2f;
		}
		if ( CameraBehaviorTimer2 >= 100 )
		{
			camera->wtf.rotation.y -= 0.05f;
		}

		if ( camera->wtf.position.z >= 10.0f )
		{
			camera->wtf.position.z = 10.0f;
		}
		if ( camera->wtf.rotation.y <= 0.0f )
		{
			camera->wtf.rotation.y = 0.0f;
			isCameraBehavior = 0;
		}
	}

	//ラウンド変化(2ラウンド目)
	if ( input_->TriggerKey(DIK_1) ){
		isRoundFlag = 1;
	}
	if ( isRoundFlag == 1){
		camera->wtf.position.z -= 0.3f;
		if ( incidenceCamera == 0 ){
			if ( camera->wtf.position.z <= -10.0f ){
				camera->wtf.position.z = -10.0f;
				incidenceCamera = 1;
			}
		}
		else if ( incidenceCamera == 1 )
		{
			camera->wtf.position.z += 0.5f;
			if ( camera->wtf.position.z >= 0.0f )
			{
				camera->wtf.position.z = 0.0f;
				incidenceCamera = 2;
				isRoundFlag = 2;
			}
		}

	}

	//ラウンド変化(3ラウンド目)
	if ( input_->TriggerKey(DIK_2) ){
		isRoundFlag = 3;
	}
	if ( isRoundFlag == 3 )
	{
		camera->wtf.position.z -= 0.3f;
		if ( incidenceCamera2 == 0 )
		{
			if ( camera->wtf.position.z <= -10.0f )
			{
				camera->wtf.position.z = -10.0f;
				incidenceCamera2 = 1;
			}
		}
		else if ( incidenceCamera2 == 1 )
		{
			camera->wtf.position.z += 0.5f;
			if ( camera->wtf.position.z >= 0.0f )
			{
				camera->wtf.position.z = 0.0f;
				incidenceCamera2 = 2;
				isRoundFlag = 4;
			}
		}

	}

	//ラウンド変化(3ラウンド目)
	if ( input_->TriggerKey(DIK_3) )
	{
		isRoundFlag = 5;
	}
	if ( isRoundFlag == 5 )
	{
		camera->wtf.position.z -= 0.3f;
		if ( incidenceCamera3 == 0 )
		{
			if ( camera->wtf.position.z <= -10.0f )
			{
				camera->wtf.position.z = -10.0f;
				incidenceCamera3 = 1;
			}
		}
		else if ( incidenceCamera3 == 1 )
		{
			camera->wtf.position.z += 0.5f;
			if ( camera->wtf.position.z >= 0.0f )
			{
				camera->wtf.position.z = 0.0f;
				incidenceCamera3 = 2;
				isRoundFlag = 6;
			}
		}

	}

	/*if ( input_->PushKey(DIK_I) ){camera->wtf.rotation.y -= 0.01f;}
	else if ( input_->PushKey(DIK_K) ){camera->wtf.rotation.y += 0.01f;}
	if ( input_->PushKey(DIK_J) ){camera->wtf.rotation.x -= 0.01f;}
	else if ( input_->PushKey(DIK_L) ){camera->wtf.rotation.x += 0.01f;}
	if ( input_->PushKey(DIK_M) ){
		camera->wtf.rotation.x = 0.0f;
		camera->wtf.rotation.y = 0.0f;
	}*/

	ImGui::Begin("Player");

	ImGui::Text("isGameStartTimer:%d",isGameStartTimer);
	ImGui::Text("CameraBehaviorTimer:%d",CameraBehaviorTimer);
	ImGui::Text("CameraRotation:%f,%f,%f",camera->wtf.rotation.x,camera->wtf.rotation.y,camera->wtf.rotation.z);
	ImGui::Text("CameraPosition:%f,%f,%f",camera->wtf.position.x,camera->wtf.position.y,camera->wtf.position.z);
	ImGui::Text("HPPosion:%f,%f",hpgreenPosition.x,hpgreenPosition.y);
	ImGui::Text("isCamShake:%d",isCamShake);

	ImGui::End();

}

void Player::Draw() {

	if ( isAliveFlag == true ){
		Obj_->Draw();
	}

	if ( isGameStartTimer >= 180 ){
		/*extrusionRightObj_->Draw();
		extrusionLeftObj_->Draw();*/
		/*collObj_->Draw();*/
		if ( isCollSWFlag == true ){
			/*collSWObj_->Draw();*/
		}
		if ( isCollSWRightFlag == true )
		{
			/*collSWRightObj_->Draw();*/
		}
	}

	if ( isShootFlag == true )
	{
		shootObj_->Draw();
	}

	if ( isShootStFlag == true )
	{
		shootStObj_->Draw();
	}
	/*shootStObj_->Draw();*/

	if ( retdisplay == true ){
		/*retObj_->Draw();*/
		retVisualObj_->Draw();
	}

}

void Player::FbxDraw() {

}

void Player::UIInitialize()
{
	//HPのUI
	hpFlameUI = new Sprite();
	hpFlameUI->Initialize(spriteCommon);
	hpFlameUI->SetPozition({ 0,0 });
	hpFlameUI->SetSize({ 1280.0f, 720.0f });

	//HPの裏の黒い部分
	hpbUI = new Sprite();
	hpbUI->Initialize(spriteCommon);
	hpbUI->SetPozition({ 0,0 });
	hpbUI->SetSize({ 1280.0f, 720.0f });

	//HPの緑の部分
	hpgreenUI = new Sprite();
	hpgreenUI->Initialize(spriteCommon);
	hpgreenPosition = hpgreenUI->GetPosition();
	hpgreenUI->SetPozition(hpgreenPosition);
	hpgreenUI->SetSize({ 1280.0f, 720.0f });

	hpredUI = new Sprite();
	hpredUI->Initialize(spriteCommon);
	hpredPosition = hpredUI->GetPosition();
	hpredUI->SetPozition(hpredPosition);
	hpredUI->SetSize({ 1280.0f, 720.0f });

	//2発目
	Bullet2dUI = new Sprite();
	Bullet2dUI->Initialize(spriteCommon);
	Bullet2dUI->SetPozition({ 0,0 });
	Bullet2dUI->SetSize({ 1280.0f, 720.0f });

	Bullet2fUI = new Sprite();
	Bullet2fUI->Initialize(spriteCommon);
	Bullet2fUI->SetPozition({ 0,0 });
	Bullet2fUI->SetSize({ 1280.0f, 720.0f });

	Bullet2mUI = new Sprite();
	Bullet2mUI->Initialize(spriteCommon);
	Bullet2mUI->SetPozition({ 0,0 });
	Bullet2mUI->SetSize({ 1280.0f, 720.0f });

	//3発目
	Bullet3dUI = new Sprite();
	Bullet3dUI->Initialize(spriteCommon);
	Bullet3dUI->SetPozition({ 0,0 });
	Bullet3dUI->SetSize({ 1280.0f, 720.0f });

	Bullet3fUI = new Sprite();
	Bullet3fUI->Initialize(spriteCommon);
	Bullet3fUI->SetPozition({ 0,0 });
	Bullet3fUI->SetSize({ 1280.0f, 720.0f });

	Bullet3mUI = new Sprite();
	Bullet3mUI->Initialize(spriteCommon);
	Bullet3mUI->SetPozition({ 0,0 });
	Bullet3mUI->SetSize({ 1280.0f, 720.0f });

	//4発目
	Bullet4dUI = new Sprite();
	Bullet4dUI->Initialize(spriteCommon);
	Bullet4dUI->SetPozition({ 0,0 });
	Bullet4dUI->SetSize({ 1280.0f, 720.0f });

	Bullet4fUI = new Sprite();
	Bullet4fUI->Initialize(spriteCommon);
	Bullet4fUI->SetPozition({ 0,0 });
	Bullet4fUI->SetSize({ 1280.0f, 720.0f });

	Bullet4mUI = new Sprite();
	Bullet4mUI->Initialize(spriteCommon);
	Bullet4mUI->SetPozition({ 0,0 });
	Bullet4mUI->SetSize({ 1280.0f, 720.0f });

	//5発目
	Bullet5dUI = new Sprite();
	Bullet5dUI->Initialize(spriteCommon);
	Bullet5dUI->SetPozition({ 0,0 });
	Bullet5dUI->SetSize({ 1280.0f, 720.0f });

	Bullet5fUI = new Sprite();
	Bullet5fUI->Initialize(spriteCommon);
	Bullet5fUI->SetPozition({ 0,0 });
	Bullet5fUI->SetSize({ 1280.0f, 720.0f });

	Bullet5mUI = new Sprite();
	Bullet5mUI->Initialize(spriteCommon);
	Bullet5mUI->SetPozition({ 0,0 });
	Bullet5mUI->SetSize({ 1280.0f, 720.0f });

	//6発目
	Bullet6dUI = new Sprite();
	Bullet6dUI->Initialize(spriteCommon);
	Bullet6dUI->SetPozition({ 0,0 });
	Bullet6dUI->SetSize({ 1280.0f, 720.0f });

	Bullet6fUI = new Sprite();
	Bullet6fUI->Initialize(spriteCommon);
	Bullet6fUI->SetPozition({ 0,0 });
	Bullet6fUI->SetSize({ 1280.0f, 720.0f });

	Bullet6mUI = new Sprite();
	Bullet6mUI->Initialize(spriteCommon);
	Bullet6mUI->SetPozition({ 0,0 });
	Bullet6mUI->SetSize({ 1280.0f, 720.0f });

	//自機のHPのUI
	hpgageUI = new Sprite();
	hpgageUI->Initialize(spriteCommon);
	hpgageUI->SetPozition({ 0,0 });
	hpgageUI->SetSize({ 1280.0f, 720.0f });

	//自機のHP(ハート3)
	hp3UI = new Sprite();
	hp3UI->Initialize(spriteCommon);
	hp3UI->SetPozition({ 0,0 });
	hp3UI->SetSize({ 1280.0f, 720.0f });

	//自機のHP(ハート2)
	hp2UI = new Sprite();
	hp2UI->Initialize(spriteCommon);
	hp2UI->SetPozition({ 0,0 });
	hp2UI->SetSize({ 1280.0f, 720.0f });

	//自機のHP(ハート1)
	hp1UI = new Sprite();
	hp1UI->Initialize(spriteCommon);
	hp1UI->SetPozition({ 0,0 });
	hp1UI->SetSize({ 1280.0f, 720.0f });

	//自機のHP(ハート0)
	overUI = new Sprite();
	overUI->Initialize(spriteCommon);
	overUI->SetPozition({ 0,0 });
	overUI->SetSize({ 1280.0f, 720.0f });

	//被弾時エフェクト
	BloodUI = new Sprite();
	BloodUI->Initialize(spriteCommon);
	BloodUI->SetPozition({ 0,0 });
	BloodUI->SetSize({ 1280.0f, 720.0f });

	//最初の登場アニメーション上
	entryani1UI = new Sprite();
	entryani1UI->Initialize(spriteCommon);
	entryani1Position = entryani1UI->GetPosition();
	entryani1UI->SetPozition(entryani1Position);
	entryani1UI->SetSize({ 1280.0f, 720.0f });

	//最初の登場アニメーション下
	entryani2UI = new Sprite();
	entryani2UI->Initialize(spriteCommon);
	entryani2Position = entryani2UI->GetPosition();
	entryani2UI->SetPozition(entryani2Position);
	entryani2UI->SetSize({ 1280.0f, 720.0f });

	//画像読み込み
	//HPゲージ
	spriteCommon->LoadTexture(1,"hpflame.png");
	hpFlameUI->SetTextureIndex(1);

	//HPの裏の黒い部分
	spriteCommon->LoadTexture(2,"hpflameblack.png");
	hpbUI->SetTextureIndex(2);
	//HPの緑の部分
	spriteCommon->LoadTexture(3,"hpgreen.png");
	hpgreenUI->SetTextureIndex(3);
	//HPの赤の部分
	spriteCommon->LoadTexture(4,"hpred.png");
	hpredUI->SetTextureIndex(4);

	//2発目
	spriteCommon->LoadTexture(5,"ff2d.png");
	Bullet2dUI->SetTextureIndex(5);
	spriteCommon->LoadTexture(6,"ff2f.png");
	Bullet2fUI->SetTextureIndex(6);
	spriteCommon->LoadTexture(7,"ff2m.png");
	Bullet2mUI->SetTextureIndex(7);

	//3発目
	spriteCommon->LoadTexture(8,"ff3d.png");
	Bullet3dUI->SetTextureIndex(8);
	spriteCommon->LoadTexture(9,"ff3f.png");
	Bullet3fUI->SetTextureIndex(9);
	spriteCommon->LoadTexture(10,"ff3m.png");
	Bullet3mUI->SetTextureIndex(10);

	//4発目
	spriteCommon->LoadTexture(11,"ff4d.png");
	Bullet4dUI->SetTextureIndex(11);
	spriteCommon->LoadTexture(12,"ff4f.png");
	Bullet4fUI->SetTextureIndex(12);
	spriteCommon->LoadTexture(13,"ff4m.png");
	Bullet4mUI->SetTextureIndex(13);

	//5発目
	spriteCommon->LoadTexture(20,"ff5d.png");
	Bullet5dUI->SetTextureIndex(20);
	spriteCommon->LoadTexture(21,"ff5f.png");
	Bullet5fUI->SetTextureIndex(21);
	spriteCommon->LoadTexture(22,"ff5m.png");
	Bullet5mUI->SetTextureIndex(22);

	//6発目
	spriteCommon->LoadTexture(23,"ff6d.png");
	Bullet6dUI->SetTextureIndex(23);
	spriteCommon->LoadTexture(24,"ff6f.png");
	Bullet6fUI->SetTextureIndex(24);
	spriteCommon->LoadTexture(25,"ff6m.png");
	Bullet6mUI->SetTextureIndex(25);

	//被弾エフェクト
	spriteCommon->LoadTexture(14,"blood.png");
	BloodUI->SetTextureIndex(14);

	//自機のHPのUI
	spriteCommon->LoadTexture(15,"hpgage.png");
	hpgageUI->SetTextureIndex(15);

	//自機のHP(ハート3)
	spriteCommon->LoadTexture(16,"hp3.png");
	hp3UI->SetTextureIndex(16);

	//自機のHP(ハート2)
	spriteCommon->LoadTexture(17,"hp2.png");
	hp2UI->SetTextureIndex(17);

	//自機のHP(ハート1)
	spriteCommon->LoadTexture(18,"hp1.png");
	hp1UI->SetTextureIndex(18);

	//自機のHP(ハート0)
	spriteCommon->LoadTexture(19,"over.png");
	overUI->SetTextureIndex(19);

	//最初の登場シーン(上の魔法陣)
	spriteCommon->LoadTexture(31,"entryani1.png");
	entryani1UI->SetTextureIndex(31);

	//最初の登場シーン(下の魔法陣)
	spriteCommon->LoadTexture(32,"entryani2.png");
	entryani2UI->SetTextureIndex(32);
}

void Player::UIDraw()
{
	//被弾時の赤いエフェクト
	if ( isCamShake == 1 ){BloodUI->Draw();}

	//HP関連
	if ( isGameStartTimer >= 180 ){
		hpbUI->Draw();
		hpredUI->Draw();
		hpgreenUI->Draw();
		hpFlameUI->Draw();
	}

}

void Player::PlayerAction()
{
	//自機とレティクルの速度
	float playerSpeed = 0.08f;
	float playerSpeed2 = 0.06f;
	float retSpeed = 0.08f;
	float retSpeed2 = 0.20f;
	////自機とレティクルの画面制限
	//float playerLimitX = 0.6f;
	//float playerLimitY = 0.19f;
	//float playerLimitY2 = 0.35f;
	//float retLimitX = 6.0f;
	//float retLimitY = 3.0f;

	//自機のジャンプの挙動とモデル変更
	if ( isJumpFlag == true ){
		Obj_->wtf.rotation.x = -0.5f;
		Obj_->wtf.position.y += 0.2f;
	}
	if ( Obj_->wtf.position.y >= 1.0f ){
		isJumpFlag = false;
		isVertFlag = true;
	}
	if ( isVertFlag == true ){
		Obj_->wtf.rotation.x = -0.5f;
		isSlowlyDownFlag = true;
		
	}
	if ( isSlowlyDownFlag == true){
		SlowlyDownTimer++;
	}

	//最初はゆっくり落ちて途中から普通に落下
	if (SlowlyDownTimer <= 10 ){
		Obj_->wtf.position.y -= 0.05f;
	}
	else if (SlowlyDownTimer > 10)
	{
		SlowlyDownTimer = 10;
		Obj_->wtf.position.y -= 0.2f;
	}

	//着地したらまたジャンプ出来る
	if ( Obj_->wtf.position.y <= -2.0f ){
		isSlowlyDownFlag = false;
		SlowlyDownTimer = 0;
		Obj_->wtf.rotation.x = 0.0f;
		isVertFlag = false;
	}
	//自機の上下移動の制限
	if ( Obj_->wtf.position.y >= 1.0f ){Obj_->wtf.position.y = 1.0f;}
	if ( Obj_->wtf.position.y <= -2.0f ){Obj_->wtf.position.y = -2.0f;}

	//自機の攻撃モーション(射撃)
	if ( isAtTimerFlag == false ){
		if ( input_->TriggerKey(DIK_SPACE) ){isAtTimerFlag = true;}
	}
	if ( isAtTimerFlag == true ){
		AtTimer++;
		bikSpinTimer = 6;
	}
	if ( AtTimer >= 1 && AtTimer <= 5 ){Obj_->SetModel(Model2_);}
	else if ( AtTimer >= 6 && AtTimer <= 20 ){Obj_->SetModel(ModelAt_);}
	else if ( AtTimer >= 21 ){
		AtTimer = 0;
		bikSpinTimer++;
		isAtTimerFlag = false;
	}

	//自機の攻撃モーション(左近接攻撃)
	if ( isLeftAtFlag == false ){
		if ( input_->TriggerKey(DIK_Q)){
			isLeftAtFlag = true;
			isCollSWFlag = true;
		}
	}
	if ( isLeftAtFlag == true ){
		leftAtTimer++;
		bikSpinTimer = 6;
	}
	if ( leftAtTimer >= 1 && leftAtTimer < 30 ){	
		Obj_->SetModel(Modelbiksword0_);
		Obj_->wtf.rotation.y += 0.25f;
	}
	else if ( leftAtTimer >= 30 )
	{
		leftAtTimer = 0;
		bikSpinTimer++;
		Obj_->wtf.rotation.y = 0.0f;
		isCollSWFlag = false;
		isLeftAtFlag = false;
	}

	//自機の攻撃モーション(右近接攻撃)
	if ( isRightAtFlag == false )
	{
		if ( input_->TriggerKey(DIK_E) )
		{
			isRightAtFlag = true;
			isCollSWRightFlag = true;
		}
	}
	if ( isRightAtFlag == true )
	{
		rightAtTimer++;
		bikSpinTimer = 6;
	}
	if ( rightAtTimer >= 1 && rightAtTimer < 30 )
	{
		Obj_->SetModel(Modelbiksword1_);
		Obj_->wtf.rotation.y -= 0.25f;
	}
	else if ( rightAtTimer >= 30 )
	{
		rightAtTimer = 0;
		bikSpinTimer++;
		Obj_->wtf.rotation.y = 0.0f;
		isCollSWRightFlag = false;
		isRightAtFlag = false;
	}



	//自機が左右に動いたらモデルも傾く
	if ( input_->PushKey(DIK_D) ){
		Obj_->wtf.rotation.z -= 0.03f;
		if( Obj_->wtf.rotation.z <= -0.4f){Obj_->wtf.rotation.z = -0.4f;}
	}
	else if ( input_->PushKey(DIK_A) ){
		Obj_->wtf.rotation.z += 0.03f;
		if ( Obj_->wtf.rotation.z >= 0.4f ){Obj_->wtf.rotation.z = 0.4f;}
	}
	else{
		Obj_->wtf.rotation.z = 0.0f;
	}

	//自機のスライディング
	if ( isbikslidFlag == false )
	{
		if (input_->TriggerKey(DIK_X) ){isbikslidFlag = true;}
	}
	if ( isbikslidFlag == true ){
		bikslidTimer++;
		bikSpinTimer = 6;
	}
	if ( bikslidTimer >= 1 && bikslidTimer <= 30 ){
		Obj_->wtf.rotation.y = -1.5f;
		Obj_->wtf.rotation.x = -1.0f;
	}
	if ( bikslidTimer >= 31 )
	{
		Obj_->wtf.rotation.y = 0.0f;
		Obj_->wtf.rotation.x = 0.0f;
		bikslidTimer = 0;
		bikSpinTimer++;
		isbikslidFlag = false;
	}


	//移動(自機)
	if ( isJumpFlag == false && isVertFlag == false )
	{
		if ( input_->TriggerKey(DIK_Z) || input_->StickInput(L_UP) ){isJumpFlag = true;}
	}

	
	
	if ( input_->PushKey(DIK_A) || input_->StickInput(L_LEFT) ){
		if ( limitmove == true ){
			Obj_->wtf.position.x -= 0.0f;
			collObj_->wtf.position.x -= 0.0f;
			retObj_->wtf.position.x += 0.0f;
			camera->wtf.position.x -= 0.0f;
		}
		else{
			limitmove2 = false;
			Obj_->wtf.position.x -= playerSpeed;
			collObj_->wtf.position.x -= playerSpeed;
			retObj_->wtf.position.x += playerSpeed2;
			/*camera->wtf.position.x -= 0.02f;*/
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
		else{
			limitmove = false;
			Obj_->wtf.position.x += playerSpeed;
			collObj_->wtf.position.x += playerSpeed;
			retObj_->wtf.position.x -= playerSpeed2;
			/*camera->wtf.position.x += 0.02f;*/
		}
	}

	if ( input_->PushKey(DIK_W) )
	{
		limitmove = false;
		limitmove2 = false;
	}
	if ( input_->PushKey(DIK_S) )
	{
		limitmove = false;
		limitmove2 = false;
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
	if ( input_->TriggerKey(DIK_SPACE) || input_->ButtonInput(RT) ){
		if ( isShootFlag == false){isShootFlag = true;}
	}

	if ( BulletCoolTime == 0)
	{
		enemylen = retObj_->wtf.position - shootObj_->wtf.position;
		enemylen.nomalize();
	}

	if ( isShootFlag == true )
	{
		BulletCoolTime++;
		shootObj_->wtf.rotation.z += 0.2f;
		shootObj_->wtf.position += enemylen;
		len = enemylen;
		len *= ShortSpeed;
	}
	else{shootObj_->wtf.position = { Obj_->wtf.position.x,Obj_->wtf.position.y, Obj_->wtf.position.z };}
	if ( BulletCoolTime >= 45.0f )
	{
		BulletCoolTime = 0;
		shootObj_->wtf.rotation.z = 0.0f;
		isShootFlag = false;
	}

}

void Player::EffUpdate()
{
	if ( isbulletEffFlag_ == 1 )
	{
		bulletEffTimer_++;
	}
	if ( bulletEffTimer_ <= 20 && bulletEffTimer_ >= 1 )
	{
		EffSummary(Vector3(Obj_->wtf.position.x - 0.15f,Obj_->wtf.position.y + 0.2f,Obj_->wtf.position.z - 1.5f));
		EffSummary2(Vector3(Obj_->wtf.position.x + 0.13f,Obj_->wtf.position.y + 0.2f,Obj_->wtf.position.z - 1.5f));
	}
	if ( bulletEffTimer_ >= 20 )
	{
		isbulletEffFlag_ = 0;
		bulletEffTimer_ = 0;
	}
}

void Player::EffSummary(Vector3 bulletpos)
{
	//パーティクル範囲
	for( int i = 0; i < 5; i++ )
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
		const float rnd_velGz = 0.05f;
		Vector3 velG{};
		velG.x = ( float ) rand() / RAND_MAX * rnd_velG - rnd_velG / 2.0f;
		velG.y = ( float ) rand() / RAND_MAX * rnd_velGy - rnd_velGy / 2.0f;
		velG.z = ( float ) rand() / RAND_MAX * rnd_velGz - rnd_velGz / 0.2f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		const float rnd_accG = 0.000001f;
		Vector3 accG{};
		accG.x = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;
		accG.y = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;

		//追加
		gasParticle->Add(60,posG,velG,accG,0.05f,0.0f);

		gasParticle->Update();

	}

}

void Player::EffSummary2(Vector3 bulletpos2)
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
		const float rnd_velz2 = 0.05f;
		Vector3 vel2{};
		vel2.x = ( float ) rand() / RAND_MAX * rnd_vel2 - rnd_vel2 / 2.0f;
		vel2.y = ( float ) rand() / RAND_MAX * rnd_vely2 - rnd_vely2 / 2.0f;
		vel2.z = ( float ) rand() / RAND_MAX * rnd_velz2 - rnd_velz2 / 0.2f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		const float rnd_acc2 = 0.000001f;
		Vector3 acc2{};
		acc2.x = ( float ) rand() / RAND_MAX * rnd_acc2 - rnd_acc2 / 2.0f;
		acc2.y = ( float ) rand() / RAND_MAX * rnd_acc2 - rnd_acc2 / 2.0f;

		//追加
		gasParticle2->Add(60,pos2,vel2,acc2,0.05f,0.0f);

		gasParticle2->Update();

	}

}

void Player::EffDraw()
{
	if ( isbulletEffFlag_ == 1 )
	{
		gasParticle->Draw();
		gasParticle2->Draw();
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

	shootObj_->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	BulletWorldPos.x = shootObj_->wtf.matWorld.m[ 3 ][ 0 ];
	BulletWorldPos.y = shootObj_->wtf.matWorld.m[ 3 ][ 1 ];
	BulletWorldPos.z = shootObj_->wtf.matWorld.m[ 3 ][ 2 ];

	return BulletWorldPos;
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
		Obj_->wtf.position.z += 0.3f;
	}

	if ( isGameStartTimer >= 60 )
	{
		rotaflag = true;
	}

	if ( isGameStartTimer <= 180 && rotaflag == true )
	{
		camera->wtf.rotation.y -= 0.05f;
	}

	if ( isGameStartTimer <= 180 && camera->wtf.rotation.y <= 0.2f && rotaflag == true ){
		camera->wtf.rotation.y = 0.2f;
		rotaflag = false;
	}

	if ( isGameStartTimer >= 179 && isGameStartTimer <= 180 ){
		Obj_->wtf.position.z = 0.0f;
	}
	if ( isGameStartTimer >= 180 )
	{
		acflag = false;
		Obj_->wtf.position.z -= 0.3f;
		retdisplay = true;
		
	}

	if ( isGameStartTimer >= 180 && isGameStartTimer <= 220 )
	{
		camera->wtf.rotation.y = 0.0f;
	}

	if ( isGameStartTimer >= 220 ){
		isBikswordstyFlag = 1;
	}

	if ( isBikswordstyFlag == 1){
		BikswordstyTimer++;
	}

	if ( BikswordstyTimer >= 1 && BikswordstyTimer <= 5 ){
		Obj_->SetModel(ModelBikswordsty_);
	}
	else if ( BikswordstyTimer >= 6 && BikswordstyTimer <= 11 ){
		Obj_->SetModel(ModelBikswordsty2_);
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
		camShakeTimer--;
		if ( camShakeTimer <= camShakeLimit && camShakeTimer > camShakeLimit * 3 / 4 )
		{
			camera->wtf.position.y += 0.1f;
			camera->wtf.position.z += 0.1f;
		}
		else if ( camShakeTimer <= camShakeLimit * 3 / 4 && camShakeTimer > camShakeLimit * 2 / 4 )
		{
			camera->wtf.position.y -= 0.1f;
			camera->wtf.position.z -= 0.1f;
		}
		else if ( camShakeTimer <= camShakeLimit * 2 / 4 && camShakeTimer > camShakeLimit * 1 / 4 )
		{
			camera->wtf.position.y += 0.1f;
			camera->wtf.position.z += 0.1f;
		}
		else if ( camShakeTimer <= camShakeLimit * 1 / 4 && camShakeTimer > 0 )
		{
			camera->wtf.position.y -= 0.1f;
			camera->wtf.position.z -= 0.1f;
		}
		else if ( camShakeTimer <= 0 ){
			isCamShake = 0;
			camera->wtf.position.y = 0.0f;
			camera->wtf.position.z = 0.0f;
		}
	}
}




