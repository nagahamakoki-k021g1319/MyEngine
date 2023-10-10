#pragma once
#include "DirectXCommon.h"
#include "Object3d.h"
#include "Input.h"
#include "Camera.h"
#include "SpriteCommon.h"
#include "Sprite.h"

class Player;
#include "Collision.h"
#include "ParticleManager.h"
#include "Audio.h"

#include "FBXModel.h"
#include "FbxLoader.h"
#include "FBXObject3d.h"
class Enemy;

class EnemyBoss
{
public:
	EnemyBoss();
	~EnemyBoss();

	void Initialize(DirectXCommon* dxCommon, Input* input);
	void Update();

	void Draw();
	void FbxDraw();
	void UIDraw();

	//エフェクトの更新処理
	void EffUpdate();
	//エフェクトの情報
	void EffSummary(Vector3 Bosspos);
	//エフェクトの描画
	void EffDraw();

	////ワールド座標を取得
	Vector3 GetWorldPosition();

	void SetPlayer(Player* player) { player_ = player; };
	void SetEnemy(Enemy* enemy) { enemy_ = enemy; };

public:
	//音を止める関数
	IXAudio2SourceVoice* pSourceVoice[10] = { 0 };

	//撃破演出のタイマー(これが終わるとゲームクリア)
	int clushingTimer = 0;

private:
	const float PI = 3.141592f;
	Input* input_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
	SpriteCommon* spriteCommon = nullptr;
	Audio* audio = nullptr;
	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;
	Collision coll;

	//ボス
	FBXModel* fbxModel_ = nullptr;
	FBXObject3d* fbxObject3d_ = nullptr;

	//ボスの当たり判定
	Object3d* Obj_ = nullptr;
	Model* Model_ = nullptr;
	bool isCollFlag = false;
	

	//ボスHPのUI
	Sprite* bosshpflameUI = nullptr;
	Sprite* bosshpRedUI = nullptr;
	Sprite* bosshpUI = nullptr;
	Vector2 bosshpPosition;

	//パーティクル
	std::unique_ptr<ParticleManager> DamageParticle;
	int EffTimer_ = 0;
	int isEffFlag_ = 0;

	const float moveSpeed_ = 0.1f;
	const float rotaSpeed_ = 0.1f;




};

