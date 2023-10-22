#include"Player.h"
#include"Enemy.h"
#include"EnemyBoss.h"
#include <imgui.h>

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

	delete BulletFlameUI;

	delete Bullet1dUI;
	delete Bullet1fUI;
	delete Bullet1mUI;

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


	// デバイスをセット
	FBXObject3d::SetDevice(dxCommon->GetDevice());
	// グラフィックスパイプライン生成
	FBXObject3d::CreateGraphicsPipeline();

	//自機
	Model_ = Model::LoadFromOBJ("bikst");
	ModelBefo_ = Model::LoadFromOBJ("bikst2");

	Model3_ = Model::LoadFromOBJ("bikslid");
	//攻撃
	Model2_ = Model::LoadFromOBJ("bikmid");
	ModelAt_ = Model::LoadFromOBJ("bikAt");

	ModelBack_ = Model::LoadFromOBJ("usirohito");

	Obj_ = Object3d::Create();
	Obj_->SetModel(Model_);
	Obj_->wtf.scale = { 0.4f,0.4f,0.4f };
	Obj_->wtf.position = { 0.0f,-2.0f,-20.0f };

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
	retObj_->wtf.position = { 0.0f,0.0f,30.0f };

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
	enemylen = retObj_->wtf.position - shootObj_->wtf.position;
	enemylen.nomalize();
	enemylen2 = retObj_->wtf.position - shootStObj_->wtf.position;
	enemylen2.nomalize();
	retVisualObj_->Update();
	Obj_->Update();
	EffUpdate();
	isGameStartFlag = true;

	//ゲームが始まる
	GameStartMovie();


	//プレイヤーの行動一覧
	PlayerAction();
	/*isbulletEffFlag_ = 1;*/
	if ( isbikslidFlag == true ){
		isbulletEffFlag_ = 0;
		bulletEffTimer_ = 0;
	}
	else{isbulletEffFlag_ = 1;}

	bikSpinTimer++;

	//バイクの車輪が動き出す
	if ( bikSpinTimer > 10 ){bikSpinTimer = 0;}

	if ( isAtTimerFlag == false )
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
	//自機の被弾エフェクト(敵の攻撃がないのでおいてる)
	if ( input_->TriggerKey(DIK_Q) ){
		isCamShake = true;
		camShakeTimer = camShakeLimit;
	}
	if ( isCamShake == true ){
		DamageCamShake();
	}
	


	ImGui::Begin("Player");

	ImGui::Text("isGameStartTimer:%d",isGameStartTimer);
	ImGui::Text("Cameraposition:%f,%f,%f",camera->wtf.rotation.x,camera->wtf.rotation.y,camera->wtf.rotation.z);

	ImGui::End();

}

void Player::Draw() {

	if ( isAliveFlag == true ){
		Obj_->Draw();
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
		retObj_->Draw();
		retVisualObj_->Draw();
	}

}

void Player::FbxDraw() {

}

void Player::UIInitialize()
{
	//自機の弾のUI
	BulletFlameUI = new Sprite();
	BulletFlameUI->Initialize(spriteCommon);
	BulletFlameUI->SetPozition({ 0,0 });
	BulletFlameUI->SetSize({ 1280.0f, 720.0f });

	//1発目
	Bullet1dUI = new Sprite();
	Bullet1dUI->Initialize(spriteCommon);
	Bullet1dUI->SetPozition({ 0,0 });
	Bullet1dUI->SetSize({ 1280.0f, 720.0f });

	Bullet1fUI = new Sprite();
	Bullet1fUI->Initialize(spriteCommon);
	Bullet1fUI->SetPozition({ 0,0 });
	Bullet1fUI->SetSize({ 1280.0f, 720.0f });

	Bullet1mUI = new Sprite();
	Bullet1mUI->Initialize(spriteCommon);
	Bullet1mUI->SetPozition({ 0,0 });
	Bullet1mUI->SetSize({ 1280.0f, 720.0f });

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
	//フレーム
	spriteCommon->LoadTexture(1,"ff.png");
	BulletFlameUI->SetTextureIndex(1);

	//1発目
	spriteCommon->LoadTexture(2,"ff1d.png");
	Bullet1dUI->SetTextureIndex(2);
	spriteCommon->LoadTexture(3,"ff1f.png");
	Bullet1fUI->SetTextureIndex(3);
	spriteCommon->LoadTexture(4,"ff1m.png");
	Bullet1mUI->SetTextureIndex(4);

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
	if ( EffTimer <= 20 && EffTimer >= 1 )
	{
		BloodUI->Draw();
	}


	////スプライト、UI
	//hpgageUI->Draw();
	//if ( playerHP >= 3 )
	//{
	//	hp3UI->Draw();
	//}
	//else if ( playerHP == 2 )
	//{
	//	hp2UI->Draw();
	//}
	//else if ( playerHP == 1 )
	//{
	//	hp1UI->Draw();
	//}
	//else if ( playerHP <= 0 )
	//{
	//	overUI->Draw();
	//}

	//BulletFlameUI->Draw();
	//if ( bulletRest == 0 )
	//{
	//	Bullet1mUI->Draw();
	//}
	//else if ( bulletRest == 1 )
	//{
	//	Bullet1fUI->Draw();
	//}
	//else if ( bulletRest >= 2 )
	//{
	//	Bullet1dUI->Draw();
	//}

	//if ( bulletRest <= 2 )
	//{
	//	Bullet2mUI->Draw();
	//}
	//else if ( bulletRest == 3 )
	//{
	//	Bullet2fUI->Draw();
	//}
	//else if ( bulletRest >= 4 )
	//{
	//	Bullet2dUI->Draw();
	//}

	//if ( bulletRest <= 4 )
	//{
	//	Bullet3mUI->Draw();
	//}
	//else if ( bulletRest == 5 )
	//{
	//	Bullet3fUI->Draw();
	//}
	//else if ( bulletRest >= 6 )
	//{
	//	Bullet3dUI->Draw();
	//}

	//if ( bulletMax >= 7 )
	//{
	//	if ( bulletRest <= 6 )
	//	{
	//		Bullet4mUI->Draw();
	//	}
	//	else if ( bulletRest == 7 )
	//	{
	//		Bullet4fUI->Draw();
	//	}
	//	else if ( bulletRest >= 8 )
	//	{
	//		Bullet4dUI->Draw();
	//	}
	//}

	//if ( bulletMax >= 9 )
	//{
	//	if ( bulletRest <= 8 )
	//	{
	//		Bullet5mUI->Draw();
	//	}
	//	else if ( bulletRest == 9 )
	//	{
	//		Bullet5fUI->Draw();
	//	}
	//	else if ( bulletRest >= 10 )
	//	{
	//		Bullet5dUI->Draw();
	//	}
	//}

	//if ( bulletMax >= 11 )
	//{
	//	if ( bulletRest <= 10 )
	//	{
	//		Bullet6mUI->Draw();
	//	}
	//	else if ( bulletRest == 11 )
	//	{
	//		Bullet6fUI->Draw();
	//	}
	//	else if ( bulletRest >= 12 )
	//	{
	//		Bullet6dUI->Draw();
	//	}
	//}

	//if ( isEntryFlag == true )
	//{
	//	entryani1UI->Draw();
	//	entryani2UI->Draw();
	//}

}

void Player::PlayerAction()
{
	//自機とレティクルの速度
	float playerSpeed = 0.08f;
	float playerSpeed2 = 0.06f;
	float retSpeed = 0.08f;
	float retSpeed2 = 0.16f;
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

	//自機の攻撃モーション
	if ( isAtTimerFlag == false ){
		if ( input_->TriggerKey(DIK_SPACE) )
		{
			isAtTimerFlag = true;
		}
	}
	if ( isAtTimerFlag == true )
	{
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

	//自機が左右に動いたらモデルも傾く
	if ( input_->PushKey(DIK_D) ){Obj_->wtf.rotation.z = -0.4f;}
	else if ( input_->PushKey(DIK_A) ){Obj_->wtf.rotation.z = 0.4f;}
	else{Obj_->wtf.rotation.z = 0.0f;}

	//自機のスライディング
	if ( isbikslidFlag == false )
	{
		if (input_->TriggerKey(DIK_S) ){isbikslidFlag = true;}
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
		if ( input_->TriggerKey(DIK_W) || input_->StickInput(L_UP) ){isJumpFlag = true;}
	}
	if ( input_->PushKey(DIK_A) || input_->StickInput(L_LEFT) )
	{
		Obj_->wtf.position.x -= playerSpeed;
		retObj_->wtf.position.x += playerSpeed2;
		camera->wtf.position.x -= 0.01f;
	}
	if ( input_->PushKey(DIK_D) || input_->StickInput(L_RIGHT) )
	{
		Obj_->wtf.position.x += playerSpeed;
		retObj_->wtf.position.x -= playerSpeed2;
		camera->wtf.position.x += 0.01f;
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
	////移動制限(自機とレティクル)
	//if (fbxObject3d_->wtf.position.x >= playerLimitX) {
	//	fbxObject3d_->wtf.position.x = playerLimitX;
	//}
	//if (fbxObject3d_->wtf.position.x <= -playerLimitX) {
	//	fbxObject3d_->wtf.position.x = -playerLimitX;
	//}
	//if (fbxObject3d_->wtf.position.y >= playerLimitY) {
	//	fbxObject3d_->wtf.position.y = playerLimitY;
	//}
	//if (fbxObject3d_->wtf.position.y <= -playerLimitY2) {
	//	fbxObject3d_->wtf.position.y = -playerLimitY2;
	//}

	/*if (retObj_->wtf.position.x >= retLimitX) {
		retObj_->wtf.position.x = retLimitX;
	}
	if (retObj_->wtf.position.x <= -retLimitX) {
		retObj_->wtf.position.x = -retLimitX;
	}
	if (retObj_->wtf.position.y >= retLimitY) {
		retObj_->wtf.position.y = retLimitY;
	}
	if (retObj_->wtf.position.y <= -retLimitY) {
		retObj_->wtf.position.y = -retLimitY;
	}*/

	//弾の制限
	if ( bulletMax > bulletMax + 1 )
	{
		bulletMax = bulletMax - 1;
	}

	//弾発射(弱)
	float ShortSpeed = 0.01f;
	if ( input_->TriggerKey(DIK_SPACE) || input_->ButtonInput(RT) ){
		if ( isShootFlag == false){isShootFlag = true;}
	}
	if ( isShootFlag == true )
	{
		BulletCoolTime++;
		shootObj_->wtf.rotation.z += 0.03f;
		shootObj_->wtf.position += enemylen;
		len = enemylen;
		len *= ShortSpeed;
	}
	else{shootObj_->wtf.position = { Obj_->wtf.position.x,Obj_->wtf.position.y, Obj_->wtf.position.z };}
	if ( BulletCoolTime >= 40.0f )
	{
		BulletCoolTime = 0;
		shootObj_->wtf.rotation.z = 0.0f;
		isShootFlag = false;
	}

	//弾発射(強)
	float ShortStSpeed = 0.02f;
	if ( input_->PushKey(DIK_Z) || input_->ButtonInput(LT) )
	{
		storeStBulletTime++;
	}
	else
	{
		storeStBulletTime = 0;
	}
	if ( storeStBulletTime >= 50 )
	{
		if ( isShootStFlag == false && bulletRest < bulletMax )
		{
			isShootStFlag = true;
		}
	}
	if ( isShootStFlag == true )
	{
		StBulletCoolTime++;
		shootStObj_->wtf.position += enemylen2;
		len2 = enemylen2;
		len2 *= ShortStSpeed;

	}
	else
	{
		shootStObj_->wtf.position = { Obj_->wtf.position.x,Obj_->wtf.position.y - 0.2f, Obj_->wtf.position.z };
	}
	if ( StBulletCoolTime >= 10.0f )
	{
		bulletRest += 2;
		storeStBulletTime = 0;
		StBulletCoolTime = 0;
		isShootStFlag = false;
	}


	if ( storeStBulletTime >= 1 && storeStBulletTime < 25 )
	{
		retObj_->SetModel(ret1Model_);
	}
	else if ( storeStBulletTime >= 25 && storeStBulletTime < 60 )
	{
		retObj_->SetModel(ret2Model_);
	}
	else
	{
		retObj_->SetModel(retModel_);
	}

	//盾
	if ( input_->TriggerKey(DIK_E) || input_->PButtonTrigger(RB) )
	{

		bulletRest = 0;
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

	Obj_->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	worldPos.x = Obj_->wtf.matWorld.m[ 3 ][ 0 ];
	worldPos.y = Obj_->wtf.matWorld.m[ 3 ][ 1 ];
	worldPos.z = Obj_->wtf.matWorld.m[ 3 ][ 2 ];

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

Vector3 Player::GetBulletStWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 BulletStWorldPos;

	shootStObj_->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	BulletStWorldPos.x = shootStObj_->wtf.matWorld.m[ 3 ][ 0 ];
	BulletStWorldPos.y = shootStObj_->wtf.matWorld.m[ 3 ][ 1 ];
	BulletStWorldPos.z = shootStObj_->wtf.matWorld.m[ 3 ][ 2 ];

	return BulletStWorldPos;
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

	if ( rotaflag == true )
	{
		camera->wtf.rotation.y -= 0.05f;
	}

	if ( camera->wtf.rotation.y <= 0.2f )
	{
		camera->wtf.rotation.y = 0.2f;
	}

	if ( isGameStartTimer >= 180 )
	{
		isGameStartTimer = 180;
		acflag = false;
		Obj_->wtf.position.z = 0.0f;
		retdisplay = true;
		camera->wtf.rotation.y = 0.0f;
	}

}

void Player::DamageCamShake()
{
	//画面シェイク
	if ( isCamShake == true )
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
		else if ( camShakeTimer <= 0 )
		{
			isCamShake = false;
			camera->wtf.position = { 0,0,0 };
		}
	}
}




