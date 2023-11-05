#include "GameScene.h"


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
	delete armorEnemy_;
	delete bikeEnemy_;
	delete bossEnemy_;
	delete obstacle_;
	delete skydome;
	delete skydomeMD;
	delete floor;
	delete floorMD;
	delete floor2;
	delete floorMD2;
	delete TitleSprite;
	delete ClearSprite;
	delete skydomeTit_;
	delete skydomeTitMD_;
	delete floorTit_;
	delete floorTitMD_;
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


	//天球(タイトル)
	skydomeTitMD_ = Model::LoadFromOBJ("skydome");
	skydomeTit_ = Object3d::Create();
	skydomeTit_->SetModel(skydomeTitMD_);
	skydomeTit_->wtf.scale = { 10000.0f,10000.0f,10000.0f };
	skydomeTit_->wtf.position = { 0.0f,400.0f,50.0f };
	skydomeTit_->wtf.rotation = { 0.0f,0.0f,0.0f };

	//タイトルの床
	floorTitMD_ = Model::LoadFromOBJ("CloudGround");
	floorTit_ = Object3d::Create();
	floorTit_->SetModel(floorTitMD_);
	floorTit_->wtf.position = ( Vector3{ 30, -5.0f, 0 } );
	floorTit_->wtf.scale = ( Vector3{ 100.0f, 500.0f, 10000.0f } );

	//タイトルの自機
	standModel_ = Model::LoadFromOBJ("bikst");
	standModel2_ = Model::LoadFromOBJ("bikst2");
	standObj_ = Object3d::Create();
	standObj_->SetModel(standModel_);
	standObj_->wtf.scale = { 0.4f,0.4f,0.4f };
	standObj_->wtf.position = { 0.0f,-1.0f,0.0f };

	floorMD2 = Model::LoadFromOBJ("tunnel");
	floor2 = Object3d::Create();
	floor2->SetModel(floorMD2);
	floor2->wtf.position = ( Vector3{ 0, 0, 70 } );
	floor2->wtf.rotation.z = 10.0f;
	floor2->wtf.scale = ( Vector3{ 20.0f, 20.0f, 10000.0f } );

	//天球(ゲームシーン)
	skydomeMD = Model::LoadFromOBJ("skydome");
	skydome = Object3d::Create();
	skydome->SetModel(skydomeMD);
	skydome->wtf.scale = { 10000.0f,10000.0f,10000.0f };
	skydome->wtf.position = { 0.0f,400.0f,50.0f };
	skydome->wtf.rotation = {0.0f,0.0f,0.0f};

	//ステージ
	floorMD = Model::LoadFromOBJ("tunnel");
	floor = Object3d::Create();
	floor->SetModel(floorMD);
	floor->wtf.position = (Vector3{ 0, 0, -70 });
	floor->wtf.rotation.z = 10.0f;
	floor->wtf.scale = (Vector3{ 20.0f, 20.0f, 10000.0f });

	

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
	
	//--------------------//





}

void GameScene::Reset() {
	
}

/// <summary>
/// 毎フレーム処理
/// </summary>
void GameScene::Update() {
	mainCamera->Update();
	
	if (sceneNo_ == SceneNo::Title) {
		spintimer++;
		if (input_->TriggerKey(DIK_SPACE) || input_->PButtonTrigger(B)) {
			isbboutFlag = true;
		}
		if ( isbboutFlag == true )
		{
			bboutTimer++;
			standObj_->wtf.position.z += 0.3f;
		}
		if ( bboutTimer >= 30 )
		{
			mainCamera->wtf.rotation.y = 2.5f;
			sceneNo_ = SceneNo::Game;
		}

		skydomeTit_->Update();
		skydomeTit_->wtf.rotation.y += 0.001f;
		floorTit_->Update();
		floorTit_->wtf.position.z -= 10.0f;
		floor2->Update();
		standObj_->Update();
		if ( spintimer >= 10){spintimer = 0;}
		if(spintimer >= 0 && spintimer <= 5 ){standObj_->SetModel(standModel_);}
		else if( spintimer >= 6 && spintimer <= 10 ){standObj_->SetModel(standModel2_);}
	}

	if (sceneNo_ == SceneNo::Game) {
		if (player_->isclearFlagTimer >= 100){
			sceneNo_ = SceneNo::Clear;
		}
		
		player_->Update();
		obstacle_->Update();
		armorEnemy_->Update(player_->GetWorldPosition(),player_->GetBulletWorldPosition(),player_->isShootFlag);
		bikeEnemy_->Update( player_->GetSwordWorldPosition(),player_->isCollSWFlag);
		bossEnemy_->Update( player_->GetWorldPosition(), player_->GetBulletWorldPosition());

		skydome->Update();
		skydome->wtf.position.z -= 0.02f;
		skydome->wtf.rotation.y += 0.0008f;

		floor->Update();
		floor->wtf.position.z -= 0.1f;
		cloudfloor_->Update();
		cloudfloor_->wtf.position.z -= 10.0f;
		cloudfloor2_->Update();
		cloudfloor2_->wtf.position.z -= 10.0f;
		cloudfloor3_->Update();
		cloudfloor3_->wtf.position.z -= 10.0f;
		
	
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
		floorTit_->Draw();
		cloudfloor_->Draw();
		floor2->Draw();
		standObj_->Draw();
	}

	if (sceneNo_ == SceneNo::Game) {
		//// 3Dオブクジェクトの描画
		player_->Draw();
		obstacle_->Draw();
		armorEnemy_->Draw();
		bikeEnemy_->Draw();
		bossEnemy_->Draw();
		floor->Draw();
		skydome->Draw();
		cloudfloor_->Draw();
		cloudfloor2_->Draw();
		cloudfloor3_->Draw();
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