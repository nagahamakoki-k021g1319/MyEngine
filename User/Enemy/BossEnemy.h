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

#include "Enemy.h"

class Player;

class BossEnemy : public Enemy
{
public:
	BossEnemy();
	~BossEnemy();
	void BossReset();

	void Initialize(DirectXCommon* dxCommon,Input* input);
	void Update(Vector3 playerPos,Vector3 playerBpos,Vector3 playerB2pos,Vector3 playerB3pos);

	void Draw();

	void UIInitialize();
	void UIDraw();

	//エフェクトの更新処理
	void EffUpdate();
	//エフェクトの情報
	void EffSummary(Vector3 bulletpos);
	void EfflinkageSummary(Vector3 bulletpos,int num);
	void EffmeteorSummary(Vector3 bulletpos,int num);
	void EffdamageSummary(Vector3 bulletpos);
	//エフェクトの描画
	void EffDraw();

	////ワールド座標を取得
	Vector3 GetWorldPosition();

	//5連誘導弾
	void inductionAttack();

	//メテオフォール
	void MeteorAttack();

	//ボスが来るときのムービー
	void BossStartMovie();

	void SetPlayer(Player* player) {
		player_ = player;
	};


private:
	const float PI = 3.141592f;
	Input* input_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
	Audio* audio = nullptr;
	Player* player_ = nullptr;
	SpriteCommon* spriteCommon = nullptr;
	Collision coll;

	int isGameStartTimer = 0;

	//待機
	//ボス
	Object3d* Obj_ = nullptr;
	Model* Model_ = nullptr;
	Model* Model2_ = nullptr;

	//バイクの車輪動かす
	int bikSpinTimer = 0;
	//ボスの攻撃時モデル
	Model* ModelAt_ = nullptr;

	//当たり判定のモデル
	Object3d* collObj_ = nullptr;
	Model* collModel_ = nullptr;
	bool isCollFlag = false;

	//自機の当たり判定
	Object3d* collPlayerObj_ = nullptr;

	//攻撃の間隔タイマー
	int SwAtTimer = -60;

	//誘導弾のモデル
	Object3d* guidbulletObj_ = nullptr;
	Model* guidbulletModel_ = nullptr;
	Vector3 playerlen;
	Vector3 bitweenlen;
	bool isShootFlag = false;
	int BulletdurationTime = 0;

	//5連誘導弾のモデル
	Object3d* linkagebulletObj_[ 5 ] = { 0 };
	Model* linkagebulletModel_[ 5 ] = { 0 };
	Vector3 playerlen_[ 5 ];
	Vector3 bitweenlen_[ 5 ];
	int islinkageShootFlag_[ 5 ] = { 0,0,0,0,0 };
	int linkageBulletdurationTime_[ 5 ] = { 0,0,0,0,0 };
	int linkageCoolTimer_ = 0;
	int isLinkageMoveFlag = 0;
	int isdurationShootFlag = 0;

	//メテオフォールのモデル
	Object3d* MeteorObj_[ 5 ] = { 0 };
	Model* MeteorModel_[ 5 ] = { 0 };
	bool isMeteorFlag = false;
	int MeteorCoolTime = 0;
	bool isFollFlag = false;

	//魔導兵の移動
	int isMoveFlag_ = 0;

	int isBesideFlag = 0;
	int isUpFlag = 0;

	//パーティクル
	std::unique_ptr<ParticleManager> gasParticle;
	int bulletEffTimer_ = 0;
	int isbulletEffFlag_ = 0;

	//5連攻撃
	std::unique_ptr<ParticleManager> linkageParticle_[ 5 ];
	int linkageEffTimer_[ 5 ] = { 0 };
	int islinkageEffFlag_[ 5 ] = { 0 };

	//メテオフォール
	std::unique_ptr<ParticleManager> meteorParticle_[ 5 ];
	int meteorEffTimer_[ 5 ] = { 0 };
	int ismeteorEffFlag_[ 5 ] = { 0 };

	//くらった時のエフェクト
	std::unique_ptr<ParticleManager> damageParticle;
	int damageEffTimer_ = 0;
	int isdamageEffFlag_ = 0;

	//HPフレームゲージ
	Sprite* hpFlameUI = nullptr;
	//HPゲージ
	Sprite* hpbUI = nullptr;
	Sprite* hpUI = nullptr;
	Vector2 hpPosition;


};

