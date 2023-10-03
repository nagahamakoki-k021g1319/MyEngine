#include"Player.h"
#include"Enemy.h"
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

	// デバイスをセット
	FBXObject3d::SetDevice(dxCommon->GetDevice());
	// グラフィックスパイプライン生成
	FBXObject3d::CreateGraphicsPipeline();

	//自機
	Model_ = Model::LoadFromOBJ("hito");//maehito usirohito
	Model2_ = Model::LoadFromOBJ("hidarihito");
	Model3_  = Model::LoadFromOBJ("migihito");
	ModelAt_ = Model::LoadFromOBJ("hitoAt");
	ModelBefo_ = Model::LoadFromOBJ("maehito");
	ModelBack_ = Model::LoadFromOBJ("usirohito");

	Obj_ = Object3d::Create();
	Obj_->SetModel(Model_);
	Obj_->wtf.scale = { 0.2f,0.2f,0.2f };
	Obj_->wtf.position = { 0.0f,0.0f,-10.0f };

	//自機の弾(弱)
	shootModel_ = Model::LoadFromOBJ("boll2");
	shootObj_ = Object3d::Create();
	shootObj_->SetModel(shootModel_);
	shootObj_->wtf.position = { Obj_->wtf.position.x,Obj_->wtf.position.y, Obj_->wtf.position.z };
	shootObj_->wtf.scale = { 3.0f,3.0f,3.0f };

	//自機の弾(強)
	shootStModel_ = Model::LoadFromOBJ("boll");
	shootStObj_ = Object3d::Create();
	shootStObj_->SetModel(shootStModel_);
	shootStObj_->wtf.position = { Obj_->wtf.position.x,Obj_->wtf.position.y, Obj_->wtf.position.z };
	shootStObj_->wtf.scale = { 0.8f,0.8f,0.8f };

	//レティクル
	retModel_ = Model::LoadFromOBJ("ster");
	ret1Model_ = Model::LoadFromOBJ("ster1");
	ret2Model_ = Model::LoadFromOBJ("ster2");
	retObj_ = Object3d::Create();
	retObj_->SetModel(retModel_);
	retObj_->wtf.scale = { 0.2f,0.2f,0.2f };
	retObj_->wtf.position = { Obj_->wtf.position.x - 1.5f,Obj_->wtf.position.y + 1.0f,Obj_->wtf.position.z + 10.0f };

	//UIの初期化(枚数が多いため)
	UIInitialize();


}



void Player::Update(int winpArrivalTimer, Vector3 pos, bool eneBulletFlag, Vector3 pos2, bool eneBulletFlag2) {
	camera->Update();
	shootObj_->Update();
	shootStObj_->Update();
	retObj_->Update();
	enemylen = retObj_->wtf.position - shootObj_->wtf.position;
	enemylen.nomalize();
	enemylen2 = retObj_->wtf.position - shootStObj_->wtf.position;
	enemylen2.nomalize();
	splineTimer++;
	Obj_->Update();

	//溜め攻撃のロックオン切り替え
	/*if (input_->PushKey(DIK_4)) {
		retObj_->SetModel(ret1Model_);
	}
	if (input_->PushKey(DIK_5)) {
		retObj_->SetModel(ret2Model_);
	}*/


	if (splineTimer >= 100) {
		//スプライン曲線の更新
		float speed = 0.01f;
		splinePosition_->Update(speed);
		Obj_->wtf.position = splinePosition_->NowPos;
		retObj_->wtf.position = splinePosition_->NowPos;
		camera->wtf.position = splinePosition_->NowPos;

		//移動
		camera->wtf.rotation = camera->wtf.rotation + cameralocalpos;

		Obj_->wtf.position = Obj_->wtf.position + playerlocalpos;

		retObj_->wtf.position = retObj_->wtf.position + retlocalpos;
		retObj_->wtf.rotation = retObj_->wtf.rotation + retRotlocalpos;


		//プレイヤーの行動一覧
		PlayerAction();

		if (winpArrivalTimer >= 850 && winpArrivalTimer < 1250) {
			camera->wtf.rotation.y += 0.01f;

			if (camera->wtf.rotation.y >= 1.2f) {
				camera->wtf.rotation.y = 1.2f;
				retlocalpos.x = 8.5f;
				playerlocalpos.x = 0.1f;
				isCameraBehavior = 1;
			}
		}
		else if (winpArrivalTimer >= 1250) {
			camera->wtf.rotation.y -= 0.01f;
			if (camera->wtf.rotation.y <= 0.0f) {
				camera->wtf.rotation.y = 0.0f;
				retResetTimer++;
				retlocalpos.z = 10.0f;
				isCameraBehavior = 0;
			}
			if (retResetTimer >= 1 && retResetTimer <= 2) {
				retlocalpos.x = 0.0f;
				playerlocalpos.x = 0.0f;
				playerlocalpos.z = 0.0f;
			}
			else if (retResetTimer >= 3){
				retResetTimer = 3;
			}

		}

		if (input_->PushKey(DIK_A) || input_->StickInput(R_LEFT)) {
			if (isCameraBehavior == 0) {
				Obj_->SetModel(Model2_);
			}
			else if (isCameraBehavior == 1) {
				Obj_->SetModel(ModelBefo_);
			}
		}
		else if (input_->PushKey(DIK_D) || input_->StickInput(R_RIGHT)) {
			if (isCameraBehavior == 0) {
				Obj_->SetModel(Model3_);
			}
			else if (isCameraBehavior == 1) {
				Obj_->SetModel(ModelBack_);
			}
		}
		else if (input_->PushKey(DIK_SPACE) || input_->ButtonInput(RT)) {
			Obj_->SetModel(ModelAt_);
		}
		else if (input_->PushKey(DIK_Z) || input_->ButtonInput(LT)) {
			Obj_->SetModel(ModelAt_);
		}
		else{
			Obj_->SetModel(Model_);
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
		if (input_->TriggerKey(DIK_Q)) {
			isEffFlag = 1;
			playerHP--;
		}
		if (isEffFlag == 1) {
			EffTimer++;
		}
		if (EffTimer >= 1 && EffTimer < 2) {
			playerHP--;
		}
		if (EffTimer >= 20) {
			EffTimer = 0;
			isEffFlag = 0;
			
		}

		//ボスのファンネルの当たり判定
		if (eneBulletFlag == true) {
			if (coll.CircleCollision(GetWorldPosition(), pos, 0.1f, 0.4f)) {
				isEffFlag = 1;
			}
		}
		//苦肉の策(あとで直す)
		if (eneBulletFlag2 == true) {
			if (coll.CircleCollision(GetWorldPosition(), pos2, 0.1f, 0.4f)) {
				isEffFlag = 1;
			}
		}




	}

	ImGui::Begin("Player");

	ImGui::Text("position:%f,%f,%f", Obj_->wtf.position.x, Obj_->wtf.position.y, Obj_->wtf.position.z);
	ImGui::Text("Cameraposition:%f,%f,%f", camera->wtf.rotation.x, camera->wtf.rotation.y, camera->wtf.rotation.z);
	ImGui::Text("RetPosition:%f,%f,%f", retlocalpos.x, retlocalpos.y, retlocalpos.z);
	ImGui::Text("PlayerPosition:%f,%f,%f", playerlocalpos.x, playerlocalpos.y, playerlocalpos.z);

	ImGui::End();

}

void Player::Draw() {
	if (splineTimer >= 110) {
		Obj_->Draw();
	}
	
	if (isShootFlag == true) {
		shootObj_->Draw();
	}

	if (isShootStFlag == true) {
		shootStObj_->Draw();
	}
	/*shootStObj_->Draw();*/

	if (splineTimer >= 110) {
		retObj_->Draw();
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

	//5発目
	spriteCommon->LoadTexture(20, "ff5d.png");
	Bullet5dUI->SetTextureIndex(20);
	spriteCommon->LoadTexture(21, "ff5f.png");
	Bullet5fUI->SetTextureIndex(21);
	spriteCommon->LoadTexture(22, "ff5m.png");
	Bullet5mUI->SetTextureIndex(22);

	//6発目
	spriteCommon->LoadTexture(23, "ff6d.png");
	Bullet6dUI->SetTextureIndex(23);
	spriteCommon->LoadTexture(24, "ff6f.png");
	Bullet6fUI->SetTextureIndex(24);
	spriteCommon->LoadTexture(25, "ff6m.png");
	Bullet6mUI->SetTextureIndex(25);

	//被弾エフェクト
	spriteCommon->LoadTexture(14, "blood.png");
	BloodUI->SetTextureIndex(14);

	//自機のHPのUI
	spriteCommon->LoadTexture(15, "hpgage.png");
	hpgageUI->SetTextureIndex(15);

	//自機のHP(ハート3)
	spriteCommon->LoadTexture(16, "hp3.png");
	hp3UI->SetTextureIndex(16);

	//自機のHP(ハート2)
	spriteCommon->LoadTexture(17, "hp2.png");
	hp2UI->SetTextureIndex(17);
	
	//自機のHP(ハート1)
	spriteCommon->LoadTexture(18, "hp1.png");
	hp1UI->SetTextureIndex(18);

	//自機のHP(ハート0)
	spriteCommon->LoadTexture(19, "over.png");
	overUI->SetTextureIndex(19);

	//最初の登場シーン
	spriteCommon->LoadTexture(31, "entry.png");
	entryani1UI->SetTextureIndex(31);


}

void Player::UIDraw()
{
	if (EffTimer <= 20 && EffTimer >= 1) {
		BloodUI->Draw();
	}


	//スプライト、UI
	hpgageUI->Draw();
	if (playerHP >= 3) { hp3UI->Draw(); }
	else if (playerHP == 2) { hp2UI->Draw(); }
	else if (playerHP == 1) { hp1UI->Draw(); }
	else if (playerHP <= 0) { overUI->Draw(); }
	
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

	if (bulletMax >= 9) {
		if (bulletRest <= 8) { Bullet5mUI->Draw(); }
		else if (bulletRest == 9) { Bullet5fUI->Draw(); }
		else if (bulletRest >= 10) { Bullet5dUI->Draw(); }
	}

	if (bulletMax >= 11) {
		if (bulletRest <= 10) { Bullet6mUI->Draw(); }
		else if (bulletRest == 11) { Bullet6fUI->Draw(); }
		else if (bulletRest >= 12) { Bullet6dUI->Draw(); }
	}
	
	entryani1UI->Draw();
	entryani2UI->Draw();
}

void Player::PlayerAction()
{
	//自機とレティクルの速度
	float playerSpeed = 0.08f;
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
		if (isCameraBehavior == 0) {
			playerlocalpos.x -= playerSpeed;
		}
		else if (isCameraBehavior == 1) {
			playerlocalpos.z += playerSpeed;
		}
	}
	if (input_->PushKey(DIK_D) || input_->StickInput(L_RIGHT)) {
		if (isCameraBehavior == 0) {
			playerlocalpos.x += playerSpeed;
		}
		else if (isCameraBehavior == 1) {
			playerlocalpos.z -= playerSpeed;
		}
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
	if (bulletMax > bulletMax + 1) {
		bulletMax = bulletMax - 1;
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
		shootObj_->wtf.position = { Obj_->wtf.position.x,Obj_->wtf.position.y, Obj_->wtf.position.z };
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
		shootStObj_->wtf.position = { Obj_->wtf.position.x,Obj_->wtf.position.y - 0.2f, Obj_->wtf.position.z};
	}
	if (StBulletCoolTime >= 10.0f) {
		bulletRest += 2;
		storeStBulletTime = 0;
		StBulletCoolTime = 0;
		isShootStFlag = false;
	}

	//盾
	if (input_->TriggerKey(DIK_E) || input_->PButtonTrigger(RB)) {
		
		bulletRest = 0;
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

	Obj_->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	worldPos.x = Obj_->wtf.matWorld.m[3][0];
	worldPos.y = Obj_->wtf.matWorld.m[3][1];
	worldPos.z = Obj_->wtf.matWorld.m[3][2];

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




