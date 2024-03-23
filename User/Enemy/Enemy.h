#pragma once
#include"Model.h"
#include"Object3d.h"


class Enemy
{
public:
	//敵の種類
	enum class EnemyType
	{
		Bike, //バイク乗った敵
		Armor, //遠距離の魔導兵
		drone, //突進するドローン
		Boss //ボス
	};

	//getter
	//IsDead
	bool GetIsDead() const {return isDead_;}
	bool GetIsDelete() const {return isDelete_;}
	bool GetIsHit() const {return isHit_;}
	int GetHP() const {return hp_;};

public:

	EnemyType enemyType_;

	//デスフラグ
	//倒されたか
	bool isDead_ = false;
	//デスポーンしたか
	bool isDelete_ = false;
	//弾が当たったか
	bool isHit_ = false;

	//体力
	int hp_;

};