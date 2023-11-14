#include "GameScene.h"
#include <imgui.h>

/// <summary>
	/// コンストクラタ
	/// </summary>
GameScene::GameScene() {
}

/// <summary>
/// デストラクタ
/// </summary>
GameScene::~GameScene() {
	delete spriteCommon;
	delete mainCamera;
	delete camera1;
	delete camera2;
	delete camera3;
	delete player_;
	delete lamp_;
	delete armorEnemy_;
	delete bikeEnemy_;
	delete bossEnemy_;
	delete obstacle_;
	delete skydome;
	delete skydomeMD;

	for ( int i = 0; i < 50; i++ ){
		delete floor_[i];
		delete floor2_[ i ];
	}
	for ( int i = 0; i < 100; i++ )
	{
		delete floor3_[ i ];
	}
	delete floorMD;
	delete floorMD2;
	delete floorMD3;
	delete TitleSprite;
	delete ClearSprite;
	delete skydomeTit_;
	delete skydomeTitMD_;
	for ( int i = 0; i < 3; i++ )
	{
		delete floorTit_[i];
		delete floorTitMD_[i];
	}
	delete cloudfloor_;
	delete cloudfloorMD_;
	delete cloudfloor2_;
	delete cloudfloorMD2_;
	delete cloudfloor3_;
	delete cloudfloorMD3_;
	delete standObj_;
	delete standModel_;
	delete standModel2_;
	delete bbout1;
	delete bbout2;
	delete bbout3;

	// ライトの解放
	delete light;

}

/// <summary>
/// 初期化
/// </summary>
void GameScene::Initialize(DirectXCommon* dxCommon, Input* input) {
	// nullチェック
	assert(dxCommon);
	assert(input);

	this->dxCommon_ = dxCommon;
	this->input_ = input;

	//スプライト共通部分の初期化
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon);

	//タイトル
	TitleSprite->Initialize(spriteCommon);
	TitleSprite->SetPozition({0,0});
	TitleSprite->SetSize({ 1280.0f, 720.0f });
	

	//ゲームクリア
	ClearSprite = new Sprite();
	ClearSprite->Initialize(spriteCommon);
	ClearSprite->SetPozition({ 0,0 });
	ClearSprite->SetSize({ 1280.0f, 720.0f });

	//タイトルからゲームシーンへの暗転
	bbout1 = new Sprite();
	bbout1->Initialize(spriteCommon);
	bbout1->SetPozition({ 0,0 });
	bbout1->SetSize({ 1280.0f, 720.0f });

	bbout2 = new Sprite();
	bbout2->Initialize(spriteCommon);
	bbout2->SetPozition({ 0,0 });
	bbout2->SetSize({ 1280.0f, 720.0f });

	bbout3 = new Sprite();
	bbout3->Initialize(spriteCommon);
	bbout3->SetPozition({ 0,0 });
	bbout3->SetSize({ 1280.0f, 720.0f });


	spriteCommon->LoadTexture(0,"tt.png");
	TitleSprite->SetTextureIndex(0);

	spriteCommon->LoadTexture(32,"clear.png");
	ClearSprite->SetTextureIndex(32);

	spriteCommon->LoadTexture(33,"bbout1.png");
	bbout1->SetTextureIndex(33);

	spriteCommon->LoadTexture(34,"bbout2.png");
	bbout2->SetTextureIndex(34);

	spriteCommon->LoadTexture(35,"bbout3.png");
	bbout3->SetTextureIndex(35);

	// カメラ生成
	mainCamera = new Camera(WinApp::window_width, WinApp::window_height);

	ParticleManager::SetCamera(mainCamera);
	Object3d::SetCamera(mainCamera);
	FBXObject3d::SetCamera(mainCamera);

	//// ライトの生成
	//light = LightGroup::Create();
	//// ライト設定
	//light->SetDirLightActive(0,true);
	//light->SetDirLightActive(1,true);
	//light->SetDirLightActive(2,true);

	//light->SetPointLightActive(0,true);
	////light->SetPointLightActive(1,false);
	////light->SetPointLightActive(2,false);

	//// 3Dオブジェクトにライトをセット
	//Object3d::SetLight(light);

	//天球(タイトル)
	skydomeTitMD_ = Model::LoadFromOBJ("skydome");
	skydomeTit_ = Object3d::Create();
	skydomeTit_->SetModel(skydomeTitMD_);
	skydomeTit_->wtf.scale = { 10000.0f,10000.0f,10000.0f };
	skydomeTit_->wtf.position = { 0.0f,400.0f,50.0f };
	skydomeTit_->wtf.rotation = { 0.0f,0.0f,0.0f };

	//タイトルの床
	for ( int i = 0; i < 3; i++ )
	{
		floorTitMD_[ i ] = Model::LoadFromOBJ("CloudGround");
		floorTit_[i] = Object3d::Create();
		floorTit_[i]->SetModel(floorTitMD_[ i ]);
		floorTit_[i]->wtf.position = ( Vector3{ 30, -5.0f, 0.0f + i * 20000.0f });
		floorTit_[i]->wtf.scale = ( Vector3{ 100.0f, 500.0f, 10000.0f } );
	}

	//タイトルの自機
	standModel_ = Model::LoadFromOBJ("bikst");
	standModel2_ = Model::LoadFromOBJ("bikst2");
	standObj_ = Object3d::Create();
	standObj_->SetModel(standModel_);
	standObj_->wtf.scale = { 0.4f,0.4f,0.4f };
	standObj_->wtf.position = { 0.0f,-1.0f,0.0f };

	//ステージ(右壁)
	floorMD = Model::LoadFromOBJ("woll");
	for ( int i = 0; i < 50; i++ )
	{
		floor_[i] = Object3d::Create();
		floor_[i]->SetModel(floorMD);
		floor_[i]->wtf.position = ( Vector3{ 150.0f, -30.0f, 0.0f + i * 230.0f } );
		floor_[i]->wtf.scale = ( Vector3{ 7.0f,30.0f, 10.0f } );
	}
	//ステージ(左壁)
	floorMD2 = Model::LoadFromOBJ("woll2");
	for ( int i = 0; i < 50; i++ ){
		floor2_[i] = Object3d::Create();
		floor2_[i]->SetModel(floorMD2);
		floor2_[i]->wtf.position = ( Vector3{ -20, -30, 0.0f + i * 230.0f } );
		floor2_[i]->wtf.scale = ( Vector3{ 7.0f,30.0f, 10.0f } );
	}
	//ステージ(上壁)
	floorMD3 = Model::LoadFromOBJ("woll3");
	for ( int i = 0; i < 100; i++ )
	{
		floor3_[i] = Object3d::Create();
		floor3_[i]->SetModel(floorMD3);
		floor3_[i]->wtf.position = ( Vector3{ 10, 200, 0.0f + i * 100.0f } );
	/*	floor3_[i]->wtf.rotation.y = 1.6f;*/
		floor3_[i]->wtf.scale = ( Vector3{ 20.0f,5.0f,5.0f } );
	}
	//天球(ゲームシーン)
	skydomeMD = Model::LoadFromOBJ("skydome");
	skydome = Object3d::Create();
	skydome->SetModel(skydomeMD);
	skydome->wtf.scale = { 10000.0f,10000.0f,10000.0f };
	skydome->wtf.position = { 0.0f,400.0f,50.0f };
	skydome->wtf.rotation = {0.0f,-3.0f,0.0f};

	

	

	//道路のステージ
	cloudfloorMD_ = Model::LoadFromOBJ("CloudGround");
	cloudfloor_ = Object3d::Create();
	cloudfloor_->SetModel(cloudfloorMD_);
	cloudfloor_->wtf.position = ( Vector3{ 30, -5.0f, 0 } );
	cloudfloor_->wtf.scale = ( Vector3{ 100.0f, 500.0f, 10000.0f } );

	//道路のステージ(繋ぎ)
	cloudfloorMD2_ = Model::LoadFromOBJ("CloudGround");
	cloudfloor2_ = Object3d::Create();
	cloudfloor2_->SetModel(cloudfloorMD2_);
	cloudfloor2_->wtf.position = ( Vector3{ 30, -5.0f, 20000 } );
	cloudfloor2_->wtf.scale = ( Vector3{ 100.0f, 500.0f, 10000.0f } );
	//道路のステージ(繋ぎ2)
	cloudfloorMD3_ = Model::LoadFromOBJ("CloudGround");
	cloudfloor3_ = Object3d::Create();
	cloudfloor3_->SetModel(cloudfloorMD3_);
	cloudfloor3_->wtf.position = ( Vector3{ 30, -5.0f, 40000 } );
	cloudfloor3_->wtf.scale = ( Vector3{ 100.0f, 500.0f, 10000.0f } );

	//プレイヤー
	player_ = new Player();
	player_->Initialize(dxCommon,input);
	player_->SetCamera(mainCamera);


	//-------敵関連--------//
	////雑魚敵
	//enemy_ = new Enemy();
	//enemy_->Initialize(dxCommon, input);
	//enemy_->SetPlayer(player_);
	armorEnemy_ = new ArmorEnemy();
	armorEnemy_->Initialize(dxCommon,input);
	armorEnemy_->SetPlayer(player_);

	bikeEnemy_ = new BikeEnemy();
	bikeEnemy_->Initialize(dxCommon,input);
	bikeEnemy_->SetPlayer(player_);

	bossEnemy_ = new BossEnemy();
	bossEnemy_->Initialize(dxCommon,input);
	bossEnemy_->SetPlayer(player_);

	//障害物
	obstacle_ = new Obstacle();
	obstacle_->Initialize(dxCommon, input);
	obstacle_->SetPlayer(player_);


	lamp_ = new Lamp();
	lamp_->Initialize(dxCommon,input);
	//--------------------//





}

void GameScene::Reset() {
	
}

/// <summary>
/// 毎フレーム処理
/// </summary>
void GameScene::Update() {
	mainCamera->Update();
	/*light->Update();*/
	if (sceneNo_ == SceneNo::Title) {
		spintimer++;
		if (input_->TriggerKey(DIK_SPACE) || input_->PButtonTrigger(B)) {isbboutFlag = true;}
		if ( isbboutFlag == true ){
			bboutTimer++;
			standObj_->wtf.position.z += 0.3f;
		}
		if ( bboutTimer >= 30 ){
			mainCamera->wtf.rotation.y = 2.5f;
			sceneNo_ = SceneNo::Game;
		}

		skydomeTit_->Update();
		//地面
		for ( int i = 0; i < 3; i++ ){
			floorTit_[i]->Update();
			floorTit_[i]->wtf.position.z -= 10.0f;
			if ( floorTit_[ i ]->wtf.position.z <= -20000.0f ){floorTit_[ i ]->wtf.position.z = 40000.0f;}
		}

		//両壁
		for ( int i = 0; i < 50; i++ ){
			floor_[i]->Update();
			floor_[ i ]->wtf.position.z -= 10.0f;
			if ( floor_[ i ]->wtf.position.z <= -4600.0f ){floor_[ i ]->wtf.position.z = 6900.0f;}
			floor2_[i]->Update();
			floor2_[ i ]->wtf.position.z -= 10.0f;
			if ( floor2_[ i ]->wtf.position.z <= -4600.0f ){floor2_[ i ]->wtf.position.z = 6900.0f;}
		}
		//天井
		for ( int i = 0; i < 100; i++ ){
			floor3_[ i ]->Update();
			floor3_[ i ]->wtf.position.z -= 10.0f;
			if ( floor3_[ i ]->wtf.position.z <= -2000.0f )
			{
				floor3_[ i ]->wtf.position.z = 8000.0f;
			}
		}
		standObj_->Update();
		if ( spintimer >= 10){spintimer = 0;}
		if(spintimer >= 0 && spintimer <= 5 ){standObj_->SetModel(standModel_);}
		else if( spintimer >= 6 && spintimer <= 10 ){standObj_->SetModel(standModel2_);}
		lamp_->Update();

		ImGui::Begin("Title");

		ImGui::Text("CameraRotation:%f,%f,%f",mainCamera->wtf.rotation.x,mainCamera->wtf.rotation.y,mainCamera->wtf.rotation.z);
		ImGui::Text("CameraPosition:%f,%f,%f",mainCamera->wtf.position.x,mainCamera->wtf.position.y,mainCamera->wtf.position.z);

		ImGui::End();
	}

	if (sceneNo_ == SceneNo::Game) {
		if (player_->isclearFlagTimer >= 100){
			sceneNo_ = SceneNo::Clear;
		}
		
		player_->Update();
		obstacle_->Update();
		armorEnemy_->Update(player_->GetWorldPosition(),player_->GetBulletWorldPosition(),player_->isShootFlag);
		bikeEnemy_->Update(player_->GetSwordLeftWorldPosition(),player_->isCollSWFlag,player_->GetSwordRightWorldPosition(),player_->isCollSWRightFlag,player_->GetWorldPosition());
		bossEnemy_->Update( player_->GetWorldPosition(), player_->GetBulletWorldPosition());

		skydome->Update();
		lamp_->Update();

		//地面
		for ( int i = 0; i < 3; i++ ){
			floorTit_[ i ]->Update();
			floorTit_[ i ]->wtf.position.z -= 10.0f;
			if ( floorTit_[ i ]->wtf.position.z <= -20000.0f )
			{
				floorTit_[ i ]->wtf.position.z = 40000.0f;
			}
		}

		//両壁
		for ( int i = 0; i < 50; i++ ){
			floor_[ i ]->Update();
			floor_[ i ]->wtf.position.z -= 10.0f;
			if ( floor_[ i ]->wtf.position.z <= -4600.0f ){floor_[ i ]->wtf.position.z = 6900.0f;}
			floor2_[ i ]->Update();
			floor2_[ i ]->wtf.position.z -= 10.0f;
			if ( floor2_[ i ]->wtf.position.z <= -4600.0f ){floor2_[ i ]->wtf.position.z = 6900.0f;}
		}
		//天井
		for ( int i = 0; i < 100; i++ )
		{
			floor3_[ i ]->Update();
			floor3_[ i ]->wtf.position.z -= 10.0f;
			if ( floor3_[ i ]->wtf.position.z <= -2000.0f ){floor3_[ i ]->wtf.position.z = 8000.0f;}
		}

	}

	if ( sceneNo_ == SceneNo::Clear ){
		isbboutFlag = false;
		bboutTimer = 0;
		standObj_->wtf.position = { 0.0f,-1.0f,0.0f };
		mainCamera->wtf.rotation.x = 0.0f;
		mainCamera->wtf.rotation.y = 0.0f;
		mainCamera->wtf.rotation.z = 0.0f;
		mainCamera->wtf.position.z = 0.0f;
		if ( input_->TriggerKey(DIK_Q) || input_->PButtonTrigger(B) ){
			sceneNo_ = SceneNo::Title;
		}
	}
	if ( sceneNo_ == SceneNo::Over )
	{
		if ( input_->TriggerKey(DIK_SPACE) || input_->PButtonTrigger(B) )
		{
			sceneNo_ = SceneNo::Title;
		}
	}
}

/// <summary>
/// 描画
/// </summary>
void GameScene::Draw() {

	

	/// <summary>
	/// 3Dオブジェクトの描画
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// <summary>
	//3Dオブジェクト描画前処理
	Object3d::PreDraw(dxCommon_->GetCommandList());
	if (sceneNo_ == SceneNo::Title) {
		skydomeTit_->Draw();

		for ( int i = 0; i < 3; i++ )
		{
			floorTit_[i]->Draw();
		}
		for ( int i = 0; i < 50; i++ )
		{
			floor_[i]->Draw();
			floor2_[i]->Draw();
		}
		for ( int i = 0; i < 100; i++ )
		{
			floor3_[ i ]->Draw();
		}
		standObj_->Draw();
		lamp_->Draw();
	}

	if (sceneNo_ == SceneNo::Game) {
		//// 3Dオブクジェクトの描画
		player_->Draw();
		obstacle_->Draw();
		armorEnemy_->Draw();
		bikeEnemy_->Draw();
		bossEnemy_->Draw();
		skydome->Draw();
		lamp_->Draw();
		for ( int i = 0; i < 3; i++ ){floorTit_[ i ]->Draw();}
		for ( int i = 0; i < 50; i++ ){
			floor_[ i ]->Draw();
			floor2_[ i ]->Draw();
		}
		for ( int i = 0; i < 100; i++ ){floor3_[ i ]->Draw();}


	}
	//3Dオブジェクト描画後処理
	Object3d::PostDraw();
	if (sceneNo_ == SceneNo::Title) {
		/*TitleSprite->Draw();*/
		if ( bboutTimer >= 1 && bboutTimer <= 10)
		{
			bbout1->Draw();
		}
		else if ( bboutTimer >= 11 && bboutTimer <= 20 )
		{
			bbout2->Draw();
		}
		else if ( bboutTimer >= 21)
		{
			bbout3->Draw();
		}

	}

	if (sceneNo_ == SceneNo::Game) {
		//// パーティクル UI FBX スプライト描画
		player_->FbxDraw();
		player_->EffDraw();
		armorEnemy_->EffDraw();
		bikeEnemy_->EffDraw();
		bossEnemy_->EffDraw();
		player_->UIDraw();
	}

	if ( sceneNo_ == SceneNo::Clear)
	{
		ClearSprite->Draw();
	}

}

Vector3 GameScene::bVelocity(Vector3& velocity, Transform& worldTransform)
{
	Vector3 result = { 0,0,0 };

	//内積
	result.z = velocity.x * worldTransform.matWorld.m[0][2] +
		velocity.y * worldTransform.matWorld.m[1][2] +
		velocity.z * worldTransform.matWorld.m[2][2];

	result.x = velocity.x * worldTransform.matWorld.m[0][0] +
		velocity.y * worldTransform.matWorld.m[1][0] +
		velocity.z * worldTransform.matWorld.m[2][0];

	result.y = velocity.x * worldTransform.matWorld.m[0][1] +
		velocity.y * worldTransform.matWorld.m[1][1] +
		velocity.z * worldTransform.matWorld.m[2][1];

	return result;
}