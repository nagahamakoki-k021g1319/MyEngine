#pragma once
#include "DirectXCommon.h"
#include "Transform.h"
#include "Object3d.h"
#include "Input.h"
#include "Camera.h"
#include "SpriteCommon.h"
#include "Sprite.h"

#include "Collision.h"
#include "ParticleManager.h"
#include "Audio.h"

#include "FBXModel.h"
#include "FbxLoader.h"
#include "FBXObject3d.h"
#include "ImGuiManager.h"

class Player;

class BikeEnemy
{
public:
	BikeEnemy();
	~BikeEnemy();

	void Initialize(DirectXCommon* dxCommon,Input* input);
	void Update(Vector3 playerSWPos,bool isCollSWFlag,Vector3 playerSWRightPos,bool isCollSWRightFlag,Vector3 playerPos);

	void Draw();

	//エフェクトの更新処理
	void EffUpdate();
	//エフェクトの情報
	void EffSummary(Vector3 bulletpos,int num);
	//エフェクトの描画
	void EffDraw();

	////ワールド座標を取得
	Vector3 GetWorldPosition();

	void SetPlayer(Player* player) {player_ = player;};

private:
	const float PI = 3.141592f;
	Input* input_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
	Audio* audio = nullptr;
	Player* player_ = nullptr;
	SpriteCommon* spriteCommon = nullptr;
	Collision coll;

	int isGameStartTimer = 0;

	//待機
	Object3d* Obj_[4] = {0};
	Model* Model_[4] = { 0 };
	Model* Model2_[4] = { 0 };
	//当たり判定のモデル
	Object3d* collObj_[4] = { 0 };
	Model* collModel_[4] = { 0 };
	int isCollFlag_[4] = { 0 };

	//バイクの車輪動かす
	int bikstSpinTimer = 0;

	//死んだときのバイクがスピンするモデル
	Object3d* bikclushObj_[4] = { 0 };
	Model* bikclushModel_[4] = { 0 };
	int isBikclushFlag_[4] = { 0 };
	int isBikSpinFlag_[4] = { 0 };

	//体力(一旦0で初期化のタイミングで設定する)
	int HP_[4] = { 0 };

	//後ろから登場するフラグ
	int isBackEntryFlag_[4] = { 0 };

	//パーティクル
	std::unique_ptr<ParticleManager> gasParticle_[4];
	int bulletEffTimer_[4] = { 0 };
	int isbulletEffFlag_[4] = { 0 };

};

