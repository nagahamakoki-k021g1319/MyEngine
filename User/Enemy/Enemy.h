#pragma once
#include"Model.h"
#include"Object3d.h"


class Enemy
{
public:
	enum class EnemyType
	{
		Bike,
		Armor,
		Boss
	};

	//getter
	//IsDead
	bool GetIsDead() const {return isDead_;}
	bool GetIsDelete() const {return isDelete_;}
	bool GetIsHit() const {return isHit_;}
	int GetHP() const {return hp_;};

public:
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