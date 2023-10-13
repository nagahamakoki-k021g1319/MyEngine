#pragma once
#include "DirectXCommon.h"
#include "Object3d.h"
#include "Input.h"
#include "Camera.h"

class Player;
#include "Collision.h"
#include "ParticleManager.h"
#include "Audio.h"

#include "FBXModel.h"
#include "FbxLoader.h"
#include "FBXObject3d.h"
#include "Enemy.h"

class Obstacle
{
public:
	Obstacle();
	~Obstacle();

	void Initialize(DirectXCommon* dxCommon, Input* input);
	void Update();

	void Draw();
	void FbxDraw();

	////ワールド座標を取得
	Vector3 GetWorldPosition();

	void SetPlayer(Player* player) { player_ = player; };
	void SetEnemy(Enemy* enemy) { enemy_ = enemy; };

public:
	//音を止める関数
	IXAudio2SourceVoice* pSourceVoice[10] = { 0 };


private:
	const float PI = 3.141592f;
	Input* input_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
	Audio* audio = nullptr;
	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;
	Collision coll;
	
	//障害物
	Object3d* obstacleObj_[8] = { 0 };
	Model* obstacleModel_ = nullptr;
	int obstacleTimer = 0;
	//生きているかどうか(0生きる,1死亡)
	int isObsAliveFlag_[4] = { 0 };

	const float moveSpeed_ = 0.1f;
	const float rotaSpeed_ = 0.1f;



	//ワールド座標を入れる変数
	Vector3 worldPos;

};

