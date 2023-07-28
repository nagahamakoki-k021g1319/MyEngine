#include"Player.h"
#include"Enemy.h"
#include <imgui.h>

Player::Player() {

}

Player::~Player() {

	//FBXオブジェクト解放
	delete fbxObject3d_;
	delete fbxModel_;
	delete shootObj_;
	delete shootModel_;
	delete shootStObj_;
	delete shootStModel_;
	delete fbxSlashObject3d_;
	delete fbxSlashModel_;
	delete fbxGardObject3d_;
	delete fbxGardModel_;
	delete hitboxObj_;
	delete hitboxModel_;
	delete retObj_;
	delete retModel_;
}

void Player::Initialize(DirectXCommon* dxCommon, Input* input) {
	// nullptrチェック
	assert(dxCommon);
	assert(input);

	this->dxCommon = dxCommon;
	input_ = input;
	camTransForm = new Transform();
	camera = new Camera(WinApp::window_width, WinApp::window_height);

	splinePosition_ = new SplinePosition(points);

	fbxModel_ = FbxLoader::GetInstance()->LoadModelFromFile("stand");
	fbxSlashModel_ = FbxLoader::GetInstance()->LoadModelFromFile("strongAttack");
	fbxGardModel_ = FbxLoader::GetInstance()->LoadModelFromFile("mera");

	// デバイスをセット
	FBXObject3d::SetDevice(dxCommon->GetDevice());
	// グラフィックスパイプライン生成
	FBXObject3d::CreateGraphicsPipeline();

	//待機
	fbxObject3d_ = new FBXObject3d;
	fbxObject3d_->Initialize();
	fbxObject3d_->SetModel(fbxModel_);
	fbxObject3d_->wtf.position = { 0.0f,0.0f,-10.0f };
	fbxObject3d_->wtf.scale = { 0.05f,0.05f,0.05f };
	fbxObject3d_->PlayAnimation(1.0f, true);

	//斬り払い
	fbxSlashObject3d_ = new FBXObject3d;
	fbxSlashObject3d_->Initialize();
	fbxSlashObject3d_->SetModel(fbxSlashModel_);
	fbxSlashObject3d_->wtf.position = { 0.0f,-0.3f,0.0f };
	fbxSlashObject3d_->wtf.scale = { 0.05f,0.05f,0.05f };

	//盾
	fbxGardObject3d_ = new FBXObject3d;
	fbxGardObject3d_->Initialize();
	fbxGardObject3d_->SetModel(fbxGardModel_);
	fbxGardObject3d_->wtf.position = { 0.0f,-0.3f,0.0f };
	fbxGardObject3d_->wtf.scale = { 0.05f,0.05f,0.05f };

	//自機の弾(弱)
	shootModel_ = Model::LoadFromOBJ("boll2");
	shootObj_ = Object3d::Create();
	shootObj_->SetModel(shootModel_);
	shootObj_->wtf.position = { fbxObject3d_->wtf.position.x,fbxObject3d_->wtf.position.y, fbxObject3d_->wtf.position.z };

	//自機の弾(強)
	shootStModel_ = Model::LoadFromOBJ("boll");
	shootStObj_ = Object3d::Create();
	shootStObj_->SetModel(shootStModel_);
	shootStObj_->wtf.position = { fbxObject3d_->wtf.position.x,fbxObject3d_->wtf.position.y, fbxObject3d_->wtf.position.z };
	shootStObj_->wtf.scale = { 0.1f,0.1f,0.1f };

	//ヒットボックス可視化
	hitboxModel_ = Model::LoadFromOBJ("hit");
	hitboxObj_ = Object3d::Create();
	hitboxObj_->SetModel(hitboxModel_);
	hitboxObj_->wtf.position = { fbxObject3d_->wtf.position.x,fbxObject3d_->wtf.position.y, fbxObject3d_->wtf.position.z };
	hitboxObj_->wtf.scale = { 0.06f,0.06f,0.06f };

	//レティクル
	retModel_ = Model::LoadFromOBJ("ster");
	retObj_ = Object3d::Create();
	retObj_->SetModel(retModel_);
	retObj_->wtf.scale = { 0.5f,0.5f,0.5f };
	retObj_->wtf.position = { fbxObject3d_->wtf.position.x -1.5f,fbxObject3d_->wtf.position.y + 1.0f,fbxObject3d_->wtf.position.z + 10.0f };
}

void Player::Update() {
	camera->Update();
	fbxObject3d_->Update();
	fbxSlashObject3d_->Update();
	fbxSlashObject3d_->wtf.position = { fbxObject3d_->wtf.position.x,fbxObject3d_->wtf.position.y, fbxObject3d_->wtf.position.z };
	fbxGardObject3d_->Update();
	fbxGardObject3d_->wtf.position = { fbxObject3d_->wtf.position.x,fbxObject3d_->wtf.position.y, fbxObject3d_->wtf.position.z };
	shootObj_->Update();
	shootStObj_->Update();
	if (isSlashFlag == true || isGardFlag == true) {
		hitboxObj_->wtf.position = { fbxObject3d_->wtf.position.x,fbxObject3d_->wtf.position.y + 0.07f, fbxObject3d_->wtf.position.z + 0.1f };
		hitboxObj_->Update();
	}
	retObj_->Update();
	enemylen = retObj_->wtf.position - shootObj_->wtf.position;
	enemylen.nomalize();
	enemylen2 = retObj_->wtf.position - shootStObj_->wtf.position;
	enemylen2.nomalize();
	splineTimer++;


	if (splineTimer >= 100) {
		//スプライン曲線の更新
		float speed = 0.01f;
		splinePosition_->Update(speed);
		fbxObject3d_->wtf.position = splinePosition_->NowPos;
		retObj_->wtf.position = splinePosition_->NowPos;
		camera->wtf.position = splinePosition_->NowPos;

		//移動
		fbxObject3d_->wtf.position = fbxObject3d_->wtf.position + playerlocalpos;
		retObj_->wtf.position = retObj_->wtf.position + retlocalpos;
		retObj_->wtf.position.z = fbxObject3d_->wtf.position.z + 10.0f;
	}

	//プレイヤーの行動一覧
	PlayerAction();

	


	ImGui::Begin("Player");

	ImGui::Text("position:%f,%f,%f", fbxObject3d_->wtf.position.x, fbxObject3d_->wtf.position.y, fbxObject3d_->wtf.position.z);
	ImGui::Text("Cameraposition:%f,%f,%f", camera->wtf.position.x, camera->wtf.position.y, camera->wtf.position.z);

	ImGui::End();

}

void Player::Draw() {
	if (isShootFlag == true) {
		shootObj_->Draw();
	}

	if (isShootStFlag == true) {
		shootStObj_->Draw();
	}

	if (isSlashFlag == true || isGardFlag == true) {
		hitboxObj_->Draw();
	}
	if (splineTimer >= 100) {
		retObj_->Draw();
	}
	
}

void Player::FbxDraw() {
	
	if (isSlashFlag == true) {
		fbxSlashObject3d_->Draw(dxCommon->GetCommandList());
	}
	else if (isGardFlag == true) {
		fbxGardObject3d_->Draw(dxCommon->GetCommandList());
	}
	else
	{
		fbxObject3d_->Draw(dxCommon->GetCommandList());
	}
}

void Player::PlayerAction()
{
	//自機とレティクルの速度
	float playerSpeed = 0.01f;
	float retSpeed = 0.08f;

	//自機とレティクルの画面制限
	float playerLimitX = 0.6f;
	float playerLimitY = 0.19f;
	float playerLimitY2 = 0.35f;
	float retLimitX = 6.0f;
	float retLimitY = 3.0f;

	if (input_->PushKey(DIK_P)) {
		splinePosition_->Reset();
	}
	//移動(自機)
	if (input_->PushKey(DIK_W) || input_->StickInput(L_UP)) {
		playerlocalpos.y += playerSpeed;
	}
	if (input_->PushKey(DIK_S) || input_->StickInput(L_DOWN)) {
		playerlocalpos.y -= playerSpeed;
	}
	if (input_->PushKey(DIK_A) || input_->StickInput(L_LEFT)) {
		playerlocalpos.x -= playerSpeed;
	}
	if (input_->PushKey(DIK_D) || input_->StickInput(L_RIGHT)) {
		playerlocalpos.x += playerSpeed;
	}
	//移動(レティクル)
	if (input_->PushKey(DIK_UP) || input_->StickInput(R_UP)) {
		retlocalpos.y += retSpeed;
	}
	if (input_->PushKey(DIK_DOWN) || input_->StickInput(R_DOWN)) {
		retlocalpos.y -= retSpeed;
	}
	if (input_->PushKey(DIK_LEFT) || input_->StickInput(R_LEFT)) {
		retlocalpos.x -= retSpeed;
	}
	if (input_->PushKey(DIK_RIGHT) || input_->StickInput(R_RIGHT)) {
		retlocalpos.x += retSpeed;
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

	//if (retObj_->wtf.position.x >= retLimitX) {
	//	retObj_->wtf.position.x = retLimitX;
	//}
	//if (retObj_->wtf.position.x <= -retLimitX) {
	//	retObj_->wtf.position.x = -retLimitX;
	//}
	//if (retObj_->wtf.position.y >= retLimitY) {
	//	retObj_->wtf.position.y = retLimitY;
	//}
	//if (retObj_->wtf.position.y <= -retLimitY) {
	//	retObj_->wtf.position.y = -retLimitY;
	//}

	//弾発射(弱)
	float ShortSpeed = 0.01f;
	if (input_->PushKey(DIK_SPACE) || input_->ButtonInput(RT)) {
		isShootFlag = true;
	}
	if (isShootFlag == true) {
		BulletCoolTime++;
		shootObj_->wtf.position += enemylen;
		len = enemylen;
		len *= ShortSpeed;
	}
	else {
		shootObj_->wtf.position = { fbxObject3d_->wtf.position.x,fbxObject3d_->wtf.position.y, fbxObject3d_->wtf.position.z};
	}
	if (BulletCoolTime >= 10.0f) {
		BulletCoolTime = 0;
		isShootFlag = false;
	}

	//弾発射(強)
	float ShortStSpeed = 0.02f;
	if (input_->PushKey(DIK_Z) || input_->ButtonInput(RT)) {storeStBulletTime++;}
	if(storeStBulletTime >= 30){isShootStFlag = true;}
	if (isShootStFlag == true) {
		StBulletCoolTime++;
		shootStObj_->wtf.position += enemylen2;
		len2 = enemylen2;
		len2 *= ShortStSpeed;
	}
	else {
		shootStObj_->wtf.position = { fbxObject3d_->wtf.position.x,fbxObject3d_->wtf.position.y, fbxObject3d_->wtf.position.z };
	}
	if (StBulletCoolTime >= 10.0f) {
		storeStBulletTime = 0;
		StBulletCoolTime = 0;
		isShootStFlag = false;
	}



	//斬り払い
	if (input_->TriggerKey(DIK_Q) || input_->PButtonTrigger(LT)) {
		isSlashFlag = true;
		fbxSlashObject3d_->PlayAnimation(1.5f, true);
	}
	if (isSlashFlag == true) {
		isSlashTimer++;
	}
	if (isSlashTimer >= 15.0f) {
		isSlashTimer = 0.0f;
		isSlashFlag = false;
	}

	//盾
	if (input_->TriggerKey(DIK_E) || input_->PButtonTrigger(RB)) {
		isGardFlag = true;
		fbxGardObject3d_->PlayAnimation(1.5f, true);
	}
	if (isGardFlag == true) {
		isGardTimer++;
	}
	if (isGardTimer >= 15.0f) {
		isGardTimer = 0.0f;
		isGardFlag = false;
	}


}

Vector3 Player::bVelocity(Vector3& velocity, Transform& worldTransform)
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

Vector3 Player::GetWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos;

	fbxObject3d_->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	worldPos.x = fbxObject3d_->wtf.matWorld.m[3][0];
	worldPos.y = fbxObject3d_->wtf.matWorld.m[3][1];
	worldPos.z = fbxObject3d_->wtf.matWorld.m[3][2];

	return worldPos;
}

Vector3 Player::GetBulletWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 BulletWorldPos;

	shootObj_->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	BulletWorldPos.x = shootObj_->wtf.matWorld.m[3][0];
	BulletWorldPos.y = shootObj_->wtf.matWorld.m[3][1];
	BulletWorldPos.z = shootObj_->wtf.matWorld.m[3][2];

	return BulletWorldPos;
}

Vector3 Player::GetRetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 RetWorldPos;

	shootObj_->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	RetWorldPos.x = retObj_->wtf.matWorld.m[3][0];
	RetWorldPos.y = retObj_->wtf.matWorld.m[3][1];
	RetWorldPos.z = retObj_->wtf.matWorld.m[3][2];

	return RetWorldPos;
}

Vector3 Player::GetSwordWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 SwordWorldPos;

	hitboxObj_->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	SwordWorldPos.x = hitboxObj_->wtf.matWorld.m[3][0];
	SwordWorldPos.y = hitboxObj_->wtf.matWorld.m[3][1];
	SwordWorldPos.z = hitboxObj_->wtf.matWorld.m[3][2];

	return SwordWorldPos;
}



