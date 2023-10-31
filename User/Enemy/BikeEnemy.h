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
	void Update(Vector3 playerSWPos,bool isCollSWFlag);

	void Draw();

	//エフェクトの更新処理
	void EffUpdate();
	//エフェクトの情報
	void EffSummary(Vector3 bulletpos);
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
	//maehito usirohito
	Object3d* Obj_ = nullptr;
	Model* Model_ = nullptr;
	//当たり判定のモデル
	Object3d* collObj_ = nullptr;
	Model* collModel_ = nullptr;
	bool isCollFlag = false;

	//死んだときのバイクがスピンするモデル
	Object3d* bikclushObj_ = nullptr;
	Model* bikclushModel_ = nullptr;
	bool isEntryFlag = false;
	//死んだときの敵が吹っ飛ぶするモデル
	Object3d* blowenemyObj_ = nullptr;
	Model* blowenemyModel_ = nullptr;
	bool isBikclushFlag = false;
	bool isBikSpinFlag = false;

	int HP = 1;

	//パーティクル
	std::unique_ptr<ParticleManager> gasParticle;
	int bulletEffTimer_ = 0;
	int isbulletEffFlag_ = 0;

};

