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
class BikeEnemy;


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

	//障害物の初期値
	void InitialPosition();

	Vector3 GetWorldPosition();

	void SetPlayer(Player* player) {
		player_ = player;
	};

	void SetBikeEnemy(BikeEnemy* bikeEnemy) {
		bikeEnemy_ = bikeEnemy;
	};

private:
	const float PI = 3.141592f;
	Input* input_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
	Audio* audio = nullptr;
	SpriteCommon* spriteCommon = nullptr;
	Collision coll;
	Player* player_ = nullptr;
	BikeEnemy* bikeEnemy_ = nullptr;

	int isGameStartTimer = 0;

	//当たり判定のモデル(本体)
	Object3d* collObj_ = nullptr;
	Model* collModel_ = nullptr;


	//木箱
	Object3d* boxObj_[12] = {0};
	Model* boxModel_[12] = { 0 };

	//三角コーン
	Object3d* konObj_[8] = { 0 };
	Model* konModel_[8] = { 0 };

public:
	//障害物に当たった時に散らばるフラグ
	//木箱
	int isBoxScatterFlag_[ 12 ] = { 0 };
	//木箱散らばるタイマー
	int boxScattertimer_[ 12 ] = { 0 };
	//三角コーン
	int isKonScatterFlag_[8] = { 0 };
	//三角コーン散らばるタイマー
	int konScattertimer_[8] = { 0 };
};

