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

class Enemy;
class EnemyBoss;

class Player {
public:
	Player();
	~Player();

	void Initialize(DirectXCommon* dxCommon, Input* input);
	void UIInitialize();
	void Update();

	void Draw();
	void FbxDraw();
	void UIDraw();

	//プレイヤーの行動一覧
	void PlayerAction();

	//エフェクトの更新処理
	void EffUpdate();
	//エフェクトの情報
	void EffSummary(Vector3 bulletpos);
	//エフェクトの情報
	void EffSummary2(Vector3 bulletpos2);
	//エフェクトの描画
	void EffDraw();

	Vector3 bVelocity(Vector3& velocity, Transform& worldTransform);

	////ワールド座標を取得
	Vector3 GetWorldPosition();
	//ワールド座標を取得(弾)
	Vector3 GetBulletWorldPosition();
	//ワールド座標を取得(強弾)
	Vector3 GetBulletStWorldPosition();
	//ワールド座標を取得(レティクル)
	Vector3 GetRetWorldPosition();
	



	Vector3 GetPos() { return Obj_->wtf.position; };
	Vector3 GetCamShake() { return camShakeVec; };


	//ゲームが始まるときのムービー
	void GameStartMovie();

	//被弾時のカメラシェイク
	void DamageCamShake();

	/// <summary>
	/// ポジション
	/// </summary>
	/// <param name="pos"></param>
	void SetPos(Vector3 pos) { Obj_->wtf.position = pos; };
	void SetCamera(Camera* cam) { camera = cam; };

	void SetEnemyBoss(EnemyBoss* eneenemyBoss) {enemyBoss_ = eneenemyBoss;};


public:
	//音を止める関数
	IXAudio2SourceVoice* pSourceVoice[10] = { 0 };

	//弾の弾数表示
	int bulletRest = 0;
	int bulletMax = 6;
	int bulletUpCount = 0;

	//カメラの向きによって自機の動きをかえるフラグ
	int isCameraBehavior = 0;



private:
	const float PI = 3.141592f;
	Input* input_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
	Audio* audio = nullptr;
	EnemyBoss* enemyBoss_ = nullptr;
	SpriteCommon* spriteCommon = nullptr;
	Collision coll;

	//ゲームが始まるフラグ
	bool isGameStartFlag = false;
	int isGameStartTimer = 0;
	bool acflag = false;
	bool rotaflag = false;
	bool camerasetFlag = false;

	//待機
	//maehito usirohito
	Object3d* Obj_ = nullptr;
	Model* Model_ = nullptr;
	Model* Model2_ = nullptr;
	Model* Model3_ = nullptr;
	Model* ModelAt_ = nullptr;
	Model* ModelBefo_ = nullptr;
	Model* ModelBack_ = nullptr;

	

	//自機のジャンプフラグ
	bool isJumpFlag = false;
	bool isVertFlag = false;
	bool isSlowlyDownFlag = false;
	int  SlowlyDownTimer = false;
	//自機の攻撃時モデル変更
	bool isAtTimerFlag = false;
	int AtTimer = 0;
	//自機のスライディング時のモデル変更
	bool isbikslidFlag = false;
	int bikslidTimer = 0;
	//バイクの車輪動かす
	int bikSpinTimer = 0;

	bool isAliveFlag = true;
	//自機のHP表示
	int playerHP = 3;
	//自機のHP表示(スプライト)
	Sprite* hpgageUI = nullptr;
	Sprite* hp3UI = nullptr;
	Sprite* hp2UI = nullptr;
	Sprite* hp1UI = nullptr;
	Sprite* overUI = nullptr;

	//登場演出
	Sprite* entryani1UI = nullptr;
	Vector2 entryani1Position;
	Sprite* entryani2UI = nullptr;
	Vector2 entryani2Position;
	bool isEntryFlag = false;
	int entryTimer = 0;


	//レティクル
	Object3d* retObj_ = nullptr;
	Model* retModel_ = nullptr;
	Model* ret1Model_ = nullptr;
	Model* ret2Model_ = nullptr;
	int retResetTimer = 0;
	bool retdisplay = false;
	Object3d* retVisualObj_ = nullptr;
	Model* retVisualModel_ = nullptr;

	//弾発射(弱)
	Object3d* shootObj_ = nullptr;
	Model* shootModel_ = nullptr;
	bool isShootFlag = false;
	int BulletCoolTime = 0;
	Vector3 enemylen;
	Vector3 len;

	//弾発射(強)
	Object3d* shootStObj_ = nullptr;
	Model* shootStModel_ = nullptr;
	bool isShootStFlag = false;
	int StBulletCoolTime = 0;
	int storeStBulletTime = 0;
	Vector3 enemylen2;
	Vector3 len2;

	
	//被弾時エフェクト
	Sprite* BloodUI = nullptr;
	int EffTimer = 0;
	int isEffFlag = 0;

	//パーティクル
	std::unique_ptr<ParticleManager> gasParticle;
	std::unique_ptr<ParticleManager> gasParticle2;
	int bulletEffTimer_ = 0;
	int isbulletEffFlag_ = 0;

	//弾の弾数表示(スプライト)
	Sprite* BulletFlameUI = nullptr;
	
	Sprite* Bullet1dUI = nullptr;
	Sprite* Bullet1fUI = nullptr;
	Sprite* Bullet1mUI = nullptr;
	
	Sprite* Bullet2dUI = nullptr;
	Sprite* Bullet2fUI = nullptr;
	Sprite* Bullet2mUI = nullptr;
	
	Sprite* Bullet3dUI = nullptr;
	Sprite* Bullet3fUI = nullptr;
	Sprite* Bullet3mUI = nullptr;
	
	Sprite* Bullet4dUI = nullptr;
	Sprite* Bullet4fUI = nullptr;
	Sprite* Bullet4mUI = nullptr;

	Sprite* Bullet5dUI = nullptr;
	Sprite* Bullet5fUI = nullptr;
	Sprite* Bullet5mUI = nullptr;

	Sprite* Bullet6dUI = nullptr;
	Sprite* Bullet6fUI = nullptr;
	Sprite* Bullet6mUI = nullptr;

	const float moveSpeed_ = 0.1f;
	const float rotaSpeed_ = 0.1f;

	Camera* camera = nullptr;
	Transform* camTransForm = nullptr;
	Vector3 targetPos;
	Vector3 eyePos;
	Vector3 centerPos;
	float targetTheta;
	float targetDistance = 10;
	float camMoveSpeed = 0.2f;
  
	Vector2 camRotaSpeed = { PI / 1800, PI / 1800};

	//画面シェイク
	bool isCamShake;
	const int camShakeLimit = 20;
	int camShakeTimer = camShakeLimit;
	Vector3 camShakeVec;
	Vector3 moveBack;


	
	
};