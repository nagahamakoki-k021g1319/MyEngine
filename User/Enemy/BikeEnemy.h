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

class BikeEnemy
{
public:
	BikeEnemy();
	~BikeEnemy();

	void Initialize(DirectXCommon* dxCommon,Input* input);
	void Update(Vector3 playerSWPos,bool isCollSWFlag,Vector3 playerSWRightPos,bool isCollSWRightFlag,Vector3 playerRPos,Vector3 playerLPos);

	void Draw();

	//エフェクトの更新処理
	void EffUpdate();
	//エフェクトの情報
	void EffSummary(Vector3 bulletpos,int num);
	void DamageLeftSummary(Vector3 firepos,int num);
	void DamageRightSummary(Vector3 firepos,int num);
	//エフェクトの描画
	void EffDraw();

	////ワールド座標を取得
	Vector3 GetWorldPosition();

	//バイク兵のエントリーを関数化
	void BikeEnemyEntry();

	//バイク兵の登場してからの挙動
	void BikeEnemyAction();

	//バイク兵同士の当たり判定
	void BiketoBikeColl();

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

	//突進用に自機に対して横一列になってる当たり判定モデル
	//右
	Object3d* collRushObj_[ 5 ] = { 0 };
	//左
	Object3d* collRushObjL_[ 5 ] = { 0 };
	Model* collRushModel_[ 5 ] = { 0 };

	//待機
	Object3d* Obj_[9] = {0};
	Model*  Model_[9] = { 0 };
	Model* Model2_[9] = { 0 };
	//当たり判定のモデル(本体)
	Object3d* collObj_[9] = { 0 };
	Model* collModel_[9] = { 0 };
	int isCollFlag_[9] = { 0 };
	//当たり判定のモデル(衝突してノックバックする用)
	Object3d* collRightObj_[ 9 ] = { 0 };
	Object3d* collLeftObj_[ 9 ] = { 0 };
	Object3d* collFrontObj_[ 9 ] = { 0 };
	Object3d* collBackObj_[ 9 ] = { 0 };
	Model* collLRModel_[ 9 ] = { 0 };

	//バイク兵同士が当たった時のノックバックフラグ
	int isEachKnockbackFlag_ = 0;
	int eachKnockbackTimer_ = 0;

	//死んだときにタイマー動す
	int deathTimer_[ 9 ] = { 0 };

	//自機と横軸があった時に突進するフラグ(0 待機,1 突進,2 自機と衝突)
	int isRushFlag_[ 9 ] = { 0 };
	int isRushFlagR_[ 9 ] = { 0 };
	//衝突した時のノックバックタイマーとフラグ
	int rushKnockbackTimer_[ 9 ] = { 0 };
	int rushKnockbackTimerR_[ 9 ] = { 0 };
	int isRushKnockbackFlag_[ 9 ] = { 0 };
	int rushCoolTimer_[ 9 ] = { 0 };

	//自機のタックルでバイク兵が喰らうフラグ
	int isHit_[ 9 ] = { 0 };

	//バイクの車輪動かす
	int bikstSpinTimer = 0;

	//死んだときのバイクがスピンするモデル
	Object3d* bikclushObj_[9] = { 0 };
	Model* bikclushModel_[9] = { 0 };
	int isBikclushFlag_[9] = { 0 };
	int isBikSpinFlag_[9] = { 0 };

	//敵の生存フラグ(0 生きる,1 死亡)
	int isAliveFlag_[ 9 ] = { 0};
	int AliveR2Timer = 0;
	int AliveR3Timer = 0;
	int AliveR4Timer = 0;

	//体力(一旦0で初期化のタイミングで設定する)
	int HP_[9] = { 0 };


	//後ろから登場するフラグ
	int isBackEntryFlag_[9] = { 0 };

	//バイク兵のアクションタイマー
	int actionTimer_[9] = { 0 };
	int isMoveFlag_[9] = { 0 };
	int stopTimer_[9] = { 0 };
	int stopTimerR_[ 9 ] = { 0 };
	//自機とバイク兵の押し出し処理(0 false,1 true)
	int limitRightmove_[ 9 ] = {0};
	int limitLeftmove_[ 9 ] = { 0 };

	//衝突した時のノックバックタイマー(通常時)
	int knockbackTimer_[ 9 ] = { 0 };
	int knockbackTimer2_[ 9 ] = { 0 };

	//生きているバイク兵で後ろに近いやつを基準にカメラを少し下げる(数字はバイク兵の番号)
	int standardPos = 0;

	//パーティクル
	//地面のズサ
	std::unique_ptr<ParticleManager> gasParticle_[9];
	int bulletEffTimer_[9] = { 0 };
	int isbulletEffFlag_[9] = { 0 };

	//攻撃受けた時の火花のパーティクル(左側)
	std::unique_ptr<ParticleManager> DamageLeftParticle_[9];
	int DamageLeftEffTimer_[9] = { 0 };
	int isDamageLeftEffFlag_[9] = { 0 };

	//攻撃受けた時の火花のパーティクル(右側)
	std::unique_ptr<ParticleManager> DamageRightParticle_[ 9 ];
	int DamageRightEffTimer_[ 9 ] = { 0 };
	int isDamageRightEffFlag_[ 9 ] = { 0 };

};

