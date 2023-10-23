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


class ArmorEnemy{
public:
	ArmorEnemy();
	~ArmorEnemy();

	void Initialize(DirectXCommon* dxCommon,Input* input);
	void Update(Vector3 playerPos);

	void Draw();

	//エフェクトの更新処理
	void EffUpdate();
	//エフェクトの情報
	void EffSummary(Vector3 bulletpos);
	//エフェクトの情報
	void EffSummary2(Vector3 bulletpos2);
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
	Model* Modelst_ = nullptr;

	//大砲の弾
	Object3d* bulletObj_ = nullptr;
	Model* bulletModel_ = nullptr;
	bool isShootFlag = false;
	int BulletdurationTime = 0;
	int BulletCoolTime = 0;
	Vector3 playerlen;
	Vector3 bitweenlen;

	//パーティクル
	std::unique_ptr<ParticleManager> gasParticle;
	std::unique_ptr<ParticleManager> gasParticle2;
	int bulletEffTimer_ = 0;
	int isbulletEffFlag_ = 0;

	//ポリゴン爆さん
	bool isExpolFlag = false;
	float ExpolTimer = 0;
	float ExpolMT = 40;
	bool isAliveFlag = true;

};









