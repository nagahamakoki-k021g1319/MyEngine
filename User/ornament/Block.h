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

class Block
{
public:
	Block();
	~Block();


	///< summary>
	///初期化
	///</summary>
	void Initialize(DirectXCommon* dxCommon,Input* input);

	///< summary>
	///更新
	///</summary>
	void Update(Vector3 playerPos);

	///< summary>
	///描画
	///</summary>
	void Draw();

	void SetPlayer(Player* player) {
		player_ = player;
	};

private:
	const float PI = 3.141592f;
	Input* input_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
	Audio* audio = nullptr;
	SpriteCommon* spriteCommon = nullptr;
	Collision coll;
	Player* player_ = nullptr;

	int isGameStartTimer = 0;

	//当たり判定のモデル(本体)
	Object3d* collObj_ = nullptr;
	Model* collModel_ = nullptr;

	//木箱
	Object3d* boxObj_[ 3 ] = {0};
	Model* boxModel_[ 3 ] = { 0 };

	//三角コーン
	Object3d* konObj_[2] = { 0 };
	Model* konModel_[2] = { 0 };

	//障害物に当たった時に散らばるフラグ
	//木箱
	int isBoxScatterFlag_[ 3 ] = { 0 };
	//三角コーン
	int isKonScatterFlag_[2] = { 0 };

	//木箱散らばるタイマー
	int boxScattertimer_[ 3 ] = { 0 };
	//三角コーン散らばるタイマー
	int konScattertimer_[2] = { 0 };
};

