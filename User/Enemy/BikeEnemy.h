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
	Object3d* Obj_[7] = {0};
	Model* Model_[7] = { 0 };
	Model* Model2_[7] = { 0 };
	//当たり判定のモデル
	Object3d* collObj_[7] = { 0 };
	Model* collModel_[7] = { 0 };
	int isCollFlag_[7] = { 0 };

	//バイクの車輪動かす
	int bikstSpinTimer = 0;

	//死んだときのバイクがスピンするモデル
	Object3d* bikclushObj_[7] = { 0 };
	Model* bikclushModel_[7] = { 0 };
	int isBikclushFlag_[7] = { 0 };
	int isBikSpinFlag_[7] = { 0 };

	//体力(一旦0で初期化のタイミングで設定する)
	int HP_[7] = { 0 };

	//後ろから登場するフラグ
	int isBackEntryFlag_[7] = { 0 };

	//パーティクル
	std::unique_ptr<ParticleManager> gasParticle_[7];
	int bulletEffTimer_[7] = { 0 };
	int isbulletEffFlag_[7] = { 0 };

};

