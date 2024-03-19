#pragma once
#include"Model.h"
#include"Object3d.h"

struct EnemyState
{
	bool isDead_ = true;
	int hp_ = 1;
};

class Enemy
{
//public:
//	// 初期化
//	virtual void Initialize(Model* Model) = 0;
//	// 更新
//	virtual void Update() = 0;
//	// 描画
//	virtual void Draw(ID3D12GraphicsCommandList* cmdList) = 0;
//	// その他の敵の共通機能

};