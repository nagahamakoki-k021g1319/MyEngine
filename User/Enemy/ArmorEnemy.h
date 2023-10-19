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

class ArmorEnemy{
public:
	ArmorEnemy();
	~ArmorEnemy();

	void Initialize(DirectXCommon* dxCommon,Input* input);
	void Update();

	void Draw();

	////ワールド座標を取得
	Vector3 GetWorldPosition();
private:
	const float PI = 3.141592f;
	Input* input_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
	Audio* audio = nullptr;
	SpriteCommon* spriteCommon = nullptr;
	Collision coll;

	//待機
	//maehito usirohito
	Object3d* Obj_ = nullptr;
	Model* Model_ = nullptr;
	Model* Modelst_ = nullptr;

	int isGameStartTimer = 0;

	//ポリゴン爆さん
	bool isExpolFlag = false;
	float ExpolTimer = 0;
	float ExpolMT = 100;
	bool isAliveFlag = true;

};









