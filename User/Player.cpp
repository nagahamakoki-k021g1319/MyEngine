#include"Player.h"
#include"Enemy.h"
#include <imgui.h>

Player::Player() {

}

Player::~Player() {
	delete spriteCommon;
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

	delete BloodUI;

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

}

void Player::Initialize(DirectXCommon* dxCommon, Input* input) {
	// nullptrチェック
	assert(dxCommon);
	assert(input);

	this->dxCommon = dxCommon;
	input_ = input;
	//スプライト共通部分の初期化
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon);

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
	shootStObj_->wtf.scale = { 0.5f,0.5f,0.5f };

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
	retObj_->wtf.position = { fbxObject3d_->wtf.position.x - 1.5f,fbxObject3d_->wtf.position.y + 1.0f,fbxObject3d_->wtf.position.z + 10.0f };

	//UIの初期化(枚数が多いため)
	UIInitialize();


}



void Player::Update(int winpArrivalTimer) {
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
		camera->wtf.rotation = camera->wtf.rotation + cameralocalpos;

		fbxObject3d_->wtf.position = fbxObject3d_->wtf.position + playerlocalpos;

		retObj_->wtf.position = retObj_->wtf.position + retlocalpos;
		retObj_->wtf.rotation = retObj_->wtf.rotation + retRotlocalpos;


		//プレイヤーの行動一覧
		PlayerAction();

		if (winpArrivalTimer >= 650) {
			isCameraBehavior = 1;
			camera->wtf.rotation.y = 1.2f;
			retlocalpos.x = 10.0f;
		}


		if (input_->PushKey(DIK_1)) {
			isCameraBehavior = 1;
			camera->wtf.rotation.y = 1.2f;
			retlocalpos.x = 10.0f;
		}
		if (input_->PushKey(DIK_2)) {
			isCameraBehavior = 0;
			camera->wtf.rotation.y = 0.0f;
			retlocalpos.x = 0.0f;
			retlocalpos.z = 10.0f;
		}


		//自機の被弾エフェクト(敵の攻撃がないのでおいてる)
		if (input_->PushKey(DIK_Q)) {
			isEffFlag = 1;
		}
		if (isEffFlag == 1) {
			EffTimer++;
		}
		if (EffTimer >= 20) {
			EffTimer = 0;
			isEffFlag = 0;
		}



	}





	ImGui::Begin("Player");

	ImGui::Text("position:%f,%f,%f", fbxObject3d_->wtf.position.x, fbxObject3d_->wtf.position.y, fbxObject3d_->wtf.position.z);
	ImGui::Text("Cameraposition:%f,%f,%f", camera->wtf.rotation.x, camera->wtf.rotation.y, camera->wtf.rotation.z);
	ImGui::Text("RetPosition:%f,%f,%f", retlocalpos.x, retlocalpos.y, retlocalpos.z);
	ImGui::Text("RetRotation:%f,%f,%f", retObj_->wtf.rotation.x, retObj_->wtf.rotation.y, retObj_->wtf.rotation.z);

	ImGui::End();

}

void Player::Draw() {
	if (isShootFlag == true) {
		shootObj_->Draw();
	}

	if (isShootStFlag == true) {
		shootStObj_->Draw();
	}

	if (isSlashFlag == true) {
		hitboxObj_->Draw();
	}
	if (splineTimer >= 100) {
		retObj_->Draw();
	}

}

void Player::FbxDraw() {
	//ここにスプライト関係も入れる

	//FBX
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

void Player::UIInitialize()
{
	//UI
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

	//被弾時エフェクト
	BloodUI = new Sprite();
	BloodUI->Initialize(spriteCommon);
	BloodUI->SetPozition({ 0,0 });
	BloodUI->SetSize({ 1280.0f, 720.0f });


	//画像読み込み
	//フレーム
	spriteCommon->LoadTexture(1, "ff.png");
	BulletFlameUI->SetTextureIndex(1);

	//1発目
	spriteCommon->LoadTexture(2, "ff1d.png");
	Bullet1dUI->SetTextureIndex(2);
	spriteCommon->LoadTexture(3, "ff1f.png");
	Bullet1fUI->SetTextureIndex(3);
	spriteCommon->LoadTexture(4, "ff1m.png");
	Bullet1mUI->SetTextureIndex(4);

	//2発目
	spriteCommon->LoadTexture(5, "ff2d.png");
	Bullet2dUI->SetTextureIndex(5);
	spriteCommon->LoadTexture(6, "ff2f.png");
	Bullet2fUI->SetTextureIndex(6);
	spriteCommon->LoadTexture(7, "ff2m.png");
	Bullet2mUI->SetTextureIndex(7);

	//3発目
	spriteCommon->LoadTexture(8, "ff3d.png");
	Bullet3dUI->SetTextureIndex(8);
	spriteCommon->LoadTexture(9, "ff3f.png");
	Bullet3fUI->SetTextureIndex(9);
	spriteCommon->LoadTexture(10, "ff3m.png");
	Bullet3mUI->SetTextureIndex(10);

	//4発目
	spriteCommon->LoadTexture(11, "ff4d.png");
	Bullet4dUI->SetTextureIndex(11);
	spriteCommon->LoadTexture(12, "ff4f.png");
	Bullet4fUI->SetTextureIndex(12);
	spriteCommon->LoadTexture(13, "ff4m.png");
	Bullet4mUI->SetTextureIndex(13);

	//被弾エフェクト
	spriteCommon->LoadTexture(14, "blood.png");
	BloodUI->SetTextureIndex(14);
	


}

void Player::UIDraw()
{
	if (EffTimer <= 20 && EffTimer >= 1) {
		BloodUI->Draw();
	}


	//スプライト、UI
	BulletFlameUI->Draw();
	if (bulletRest == 0) { Bullet1mUI->Draw(); }
	else if (bulletRest == 1) { Bullet1fUI->Draw(); }
	else if (bulletRest >= 2) { Bullet1dUI->Draw(); }

	if (bulletRest <= 2) { Bullet2mUI->Draw(); }
	else if (bulletRest == 3) { Bullet2fUI->Draw(); }
	else if (bulletRest >= 4) { Bullet2dUI->Draw(); }

	if (bulletRest <= 4) { Bullet3mUI->Draw(); }
	else if (bulletRest == 5) { Bullet3fUI->Draw(); }
	else if (bulletRest >= 6) { Bullet3dUI->Draw(); }

	if (bulletMax >= 7) {
		if (bulletRest <= 6) { Bullet4mUI->Draw(); }
		else if (bulletRest == 7) { Bullet4fUI->Draw(); }
		else if (bulletRest >= 8) { Bullet4dUI->Draw(); }
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
		retlocalpos.z = 10.0f;
		/*splinePosition_->Reset();*/
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
	if (input_->PushKey(DIK_R)) {
		playerlocalpos.z += playerSpeed;
	}
	if (input_->PushKey(DIK_F)) {
		playerlocalpos.z -= playerSpeed;
	}
	//移動(レティクル)
	if (input_->PushKey(DIK_UP) || input_->StickInput(R_UP)) {
		retlocalpos.y += retSpeed;
	}
	if (input_->PushKey(DIK_DOWN) || input_->StickInput(R_DOWN)) {
		retlocalpos.y -= retSpeed;
	}
	if (input_->PushKey(DIK_LEFT) || input_->StickInput(R_LEFT)) {
		if (isCameraBehavior == 0) {
			retlocalpos.x -= retSpeed;
		}
		else if (isCameraBehavior == 1) {
			retlocalpos.z += retSpeed;
		}

	}
	if (input_->PushKey(DIK_RIGHT) || input_->StickInput(R_RIGHT)) {
		if (isCameraBehavior == 0) {
			retlocalpos.x += retSpeed;
		}
		else if (isCameraBehavior == 1) {
			retlocalpos.z -= retSpeed;
		}
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

	//弾の制限
	if (bulletMax > bulletMax + 1) {
		bulletMax = bulletMax;
	}

	//弾発射(弱)
	float ShortSpeed = 0.01f;
	if (input_->TriggerKey(DIK_SPACE) || input_->ButtonInput(RT)) {
		if (isShootFlag == false && bulletRest <= bulletMax) {
			bulletRest += 1;
			isShootFlag = true;
		}
	}
	if (isShootFlag == true) {
		BulletCoolTime++;
		shootObj_->wtf.position += enemylen;
		len = enemylen;
		len *= ShortSpeed;
	}
	else {
		shootObj_->wtf.position = { fbxObject3d_->wtf.position.x,fbxObject3d_->wtf.position.y, fbxObject3d_->wtf.position.z };
	}
	if (BulletCoolTime >= 10.0f) {
		BulletCoolTime = 0;
		isShootFlag = false;
	}

	//弾発射(強)
	float ShortStSpeed = 0.02f;
	if (input_->PushKey(DIK_Z) || input_->ButtonInput(LT)) { storeStBulletTime++; }
	if (storeStBulletTime >= 30) {
		if (isShootStFlag == false && bulletRest < bulletMax) {
			isShootStFlag = true;
		}
	}
	if (isShootStFlag == true) {
		StBulletCoolTime++;
		shootStObj_->wtf.position += enemylen2;
		len2 = enemylen2;
		len2 *= ShortStSpeed;

	}
	else {
		shootStObj_->wtf.position = { fbxObject3d_->wtf.position.x,fbxObject3d_->wtf.position.y - 1.5f, fbxObject3d_->wtf.position.z + 1.0f };
	}
	if (StBulletCoolTime >= 10.0f) {
		bulletRest += 2;
		storeStBulletTime = 0;
		StBulletCoolTime = 0;
		isShootStFlag = false;
	}



	//斬り払い
	if (input_->TriggerKey(DIK_Q) || input_->PButtonTrigger(LB)) {
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
		bulletRest = 0;
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

Vector3 Player::GetBulletStWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 BulletStWorldPos;

	shootStObj_->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	BulletStWorldPos.x = shootStObj_->wtf.matWorld.m[3][0];
	BulletStWorldPos.y = shootStObj_->wtf.matWorld.m[3][1];
	BulletStWorldPos.z = shootStObj_->wtf.matWorld.m[3][2];

	return BulletStWorldPos;
}

Vector3 Player::GetRetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 RetWorldPos;

	retObj_->wtf.UpdateMat();
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



