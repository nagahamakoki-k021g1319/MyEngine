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

class BossEnemy
{
public:
	BossEnemy();
	~BossEnemy();

	void Initialize(DirectXCommon* dxCommon,Input* input);
	void Update(Vector3 playerPos,Vector3 playerBpos);

	void Draw();

	//エフェクトの更新処理
	void EffUpdate();
	//エフェクトの情報
	void EffSummary(Vector3 bulletpos);
	//エフェクトの描画
	void EffDraw();

	////ワールド座標を取得
	Vector3 GetWorldPosition();

	//ボスが来るときのムービー
	void BossStartMovie();

	void SetPlayer(Player* player) {
		player_ = player;
	};


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
	//ボス
	Object3d* Obj_ = nullptr;
	Model* Model_ = nullptr;
	Model* Model2_ = nullptr;

	int HP = 1;
	//バイクの車輪動かす
	int bikSpinTimer = 0;
	//ボスの攻撃時モデル
	Model* ModelAt_ = nullptr;

	//当たり判定のモデル
	Object3d* collObj_ = nullptr;
	Model* collModel_ = nullptr;
	bool isCollFlag = false;

	//自機の当たり判定
	Object3d* collPlayerObj_ = nullptr;

	//攻撃時のモデル(剣のみ)
	Object3d* swObj_[10] = {0};
	Model* swModel_ = nullptr;
	bool isSWFlag = false;
	int SWTimer = 0;
	//当たり判定のモデル(攻撃時)
	Object3d* collswObj_ = nullptr;
	Model* collswModel_ = nullptr;
	bool isshotFlag = false;
	//攻撃の間隔タイマー
	int SwAtTimer = -60;

	//誘導弾のモデル(攻撃時)
	Object3d* guidbulletObj_ = nullptr;
	Model* guidbulletModel_ = nullptr;
	Vector3 playerlen;
	Vector3 bitweenlen;
	bool isShootFlag = false;
	int BulletdurationTime = 0;
	




	int isBesideFlag = 0;
	int isUpFlag = 0;

	//パーティクル
	std::unique_ptr<ParticleManager> gasParticle;
	int bulletEffTimer_ = 0;
	int isbulletEffFlag_ = 0;

};

