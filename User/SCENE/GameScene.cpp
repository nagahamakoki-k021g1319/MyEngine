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
	delete enemy_;
	delete obstacle_;
	delete enemyBoss_;
	delete skydome;
	delete skydomeMD;
	delete floor;
	delete floorMD;
	delete TitleSprite;
	delete skydomeTit_;
	delete skydomeTitMD_;
	delete floorTit_;
	delete floorTitMD_;
	delete standObj_;
	delete standModel_;

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
	spriteCommon->LoadTexture(0, "tt.png");
	TitleSprite->SetTextureIndex(0);

	// カメラ生成
	mainCamera = new Camera(WinApp::window_width, WinApp::window_height);

	ParticleManager::SetCamera(mainCamera);
	Object3d::SetCamera(mainCamera);
	FBXObject3d::SetCamera(mainCamera);


	//天球(タイトル)
	skydomeTitMD_ = Model::LoadFromOBJ("skydome");
	skydomeTit_ = Object3d::Create();
	skydomeTit_->SetModel(skydomeTitMD_);
	skydomeTit_->wtf.scale = { 100.0f,100.0f,100.0f };
	skydomeTit_->wtf.position = { 0.0f,-20.0f,50.0f };
	skydomeTit_->wtf.rotation = { 0.0f,0.0f,0.0f };

	//タイトルの床
	floorTitMD_ = Model::LoadFromOBJ("Ground");
	floorTit_ = Object3d::Create();
	floorTit_->SetModel(floorTitMD_);
	floorTit_->wtf.position = (Vector3{ 0, -1.0f, 0 });
	floorTit_->wtf.scale = (Vector3{ 5.0f, 5.0f, 5.0f });

	//タイトルの自機
	standModel_ = Model::LoadFromOBJ("taikihito");
	standObj_ = Object3d::Create();
	standObj_->SetModel(standModel_);
	standObj_->wtf.scale = { 0.1f,0.1f,0.1f };
	standObj_->wtf.position = { 0.0f,-1.0f,0.0f };

	//天球(ゲームシーン)
	skydomeMD = Model::LoadFromOBJ("skydome");
	skydome = Object3d::Create();
	skydome->SetModel(skydomeMD);
	skydome->wtf.scale = { 1000.0f,1000.0f,1000.0f };
	skydome->wtf.position = { 0.0f,-20.0f,50.0f };
	skydome->wtf.rotation = {0.0f,0.0f,0.0f};

	//ステージ
	floorMD = Model::LoadFromOBJ("stage2");
	floor = Object3d::Create();
	floor->SetModel(floorMD);
	floor->wtf.position = (Vector3{ 0, -10, 0 });
	floor->wtf.scale = (Vector3{ 0.5f, 0.5f, 0.5f });

	//プレイヤー
	player_ = new Player();
	player_->Initialize(dxCommon,input);
	player_->SetCamera(mainCamera);
	player_->SetEnemy(enemy_);

	//-------敵関連--------//
	//雑魚敵
	enemy_ = new Enemy();
	enemy_->Initialize(dxCommon, input);
	enemy_->SetPlayer(player_);
	//障害物
	obstacle_ = new Obstacle();
	obstacle_->Initialize(dxCommon, input);
	obstacle_->SetPlayer(player_);
	obstacle_->SetEnemy(enemy_);
	//敵Boss
	enemyBoss_ = new EnemyBoss();
	enemyBoss_->Initialize(dxCommon, input);
	enemyBoss_->SetPlayer(player_);
	enemyBoss_->SetEnemy(enemy_);


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
		if (input_->TriggerKey(DIK_SPACE) || input_->PButtonTrigger(B)) {
			mainCamera->wtf.rotation.y = 0.0f;
			sceneNo_ = SceneNo::Game;
		}
		skydomeTit_->Update();
		floorTit_->Update();
		standObj_->Update();
		mainCamera->wtf.rotation.y += 0.005f;
	}

	if (sceneNo_ == SceneNo::Game) {
		
		player_->Update(enemy_->winpArrivalTimer,enemy_->GetinductionWorldPosition(0),enemy_->isShootStFlag_[0],enemy_->GetinductionWorldPosition(1), enemy_->isShootStFlag_[1]);
		enemy_->Update(player_->splinePosition_);
		enemyBoss_->Update();
		obstacle_->Update();

		floor->Update();
		skydome->Update();
		skydome->wtf.position.z += 0.02f;
		skydome->wtf.rotation.y += 0.0008f;
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
		standObj_->Draw();
	}

	if (sceneNo_ == SceneNo::Game) {
		//// 3Dオブクジェクトの描画
		player_->Draw();
		enemy_->Draw();
		obstacle_->Draw();
		floor->Draw();
		skydome->Draw();
	}
	//3Dオブジェクト描画後処理
	Object3d::PostDraw();
	if (sceneNo_ == SceneNo::Title) {
		TitleSprite->Draw();
	}

	if (sceneNo_ == SceneNo::Game) {
		//// パーティクル UI FBX スプライト描画
		player_->FbxDraw();
	/*	player_->EffDraw();*/
		enemy_->FbxDraw();
		enemy_->EffDraw();
		enemy_->UIDraw();
		enemyBoss_->FbxDraw();
		player_->UIDraw();
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