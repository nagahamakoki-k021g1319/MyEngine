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


class Lamp
{
public:
	Lamp();
	~Lamp();


	///< summary>
	///初期化
	///</summary>
	void Initialize(DirectXCommon* dxCommon,Input* input);

	///< summary>
	///更新
	///</summary>
	void Update();

	///< summary>
	///描画
	///</summary>
	void Draw();

private:
	const float PI = 3.141592f;
	Input* input_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
	Audio* audio = nullptr;
	SpriteCommon* spriteCommon = nullptr;
	Collision coll;

	//ランプ
	Object3d* RightObj_[ 80 ] = {0};
	Object3d* LeftObj_[ 80 ] = { 0 };
	Model* Model_ = nullptr;


};

