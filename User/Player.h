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
#include "SplinePosition.h"
#include "ImGuiManager.h"

class Enemy;

class Player {
public:
	Player();
	~Player();

	void Initialize(DirectXCommon* dxCommon, Input* input);
	void UIInitialize();
	void Update(int winpArrivalTimer,Vector3 pos,bool eneBulletFlag, Vector3 pos2, bool eneBulletFlag2);

	void Draw();
	void FbxDraw();
	void UIDraw();

	//プレイヤーの行動一覧
	void PlayerAction();



	Vector3 bVelocity(Vector3& velocity, Transform& worldTransform);

	////ワールド座標を取得
	Vector3 GetWorldPosition();
	//ワールド座標を取得(弾)
	Vector3 GetBulletWorldPosition();
	//ワールド座標を取得(弾)
	Vector3 GetBulletStWorldPosition();
	//ワールド座標を取得(レティクル)
	Vector3 GetRetWorldPosition();
	//ワールド座標を取得(抜刀)
	Vector3 GetSwordWorldPosition();



	Vector3 GetPos() { return fbxObject3d_->wtf.position; };
	Vector3 GetCamShake() { return camShakeVec; };

	/// <summary>
	/// ポジション
	/// </summary>
	/// <param name="pos"></param>
	void SetPos(Vector3 pos) {fbxObject3d_->wtf.position = pos; };
	void SetCamera(Camera* cam) { camera = cam; };

	void SetEnemy(Enemy* enemy) { enemy_ = enemy; };

public:
	//音を止める関数
	IXAudio2SourceVoice* pSourceVoice[10] = { 0 };
	SplinePosition* splinePosition_ = nullptr;
	

	//弾の弾数表示
	int bulletRest = 0;
	int bulletMax = 6;
	int bulletUpCount = 0;

	//カメラの向きによって自機の動きをかえるフラグ
	int isCameraBehavior = 0;


private:
	const float PI = 3.141592f;
	Input* input_ = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Audio* audio = nullptr;
	Enemy* enemy_ = nullptr;
	SpriteCommon* spriteCommon = nullptr;
	Collision coll;

	//待機
	FBXModel* fbxModel_ = nullptr;
	FBXObject3d* fbxObject3d_ = nullptr;
	bool isAliveFlag = true;
	//自機のHP表示
	int playerHP = 3;
	//自機のHP表示(スプライト)
	Sprite* hpgageUI = nullptr;
	Sprite* hp3UI = nullptr;
	Sprite* hp2UI = nullptr;
	Sprite* hp1UI = nullptr;
	Sprite* overUI = nullptr;

	//レティクル
	Object3d* retObj_ = nullptr;
	Model* retModel_ = nullptr;
	int retResetTimer = 0;

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

	//切り払いモーション
	FBXModel* fbxSlashModel_ = nullptr;
	FBXObject3d* fbxSlashObject3d_ = nullptr;
	bool isSlashFlag = false;
	float isSlashTimer = 0.0f;

	//盾モーション
	FBXModel* fbxGardModel_ = nullptr;
	FBXObject3d* fbxGardObject3d_ = nullptr;
	bool isGardFlag = false;
	float isGardTimer = 0.0f;

	//ヒットボックス
	Object3d* hitboxObj_ = nullptr;
	Model* hitboxModel_ = nullptr;
	
	
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
	

	//ローカル移動
	Vector3 playerlocalpos = { 0.0f,-0.3f,0.0f };
	Vector3 cameralocalpos = { 0.0f,0.0f,0.0f };
	Vector3 retlocalpos = { 0.0f,0.0f,10.0f };
	Vector3 retRotlocalpos = { 0.0f,0.0f,0.0f };

	


	//スプライン曲線
	int splineTimer = 0;
	
	////止めるとき
	//Vector3 Start = { 0.0f,0.0f,0.0f };
	//Vector3 end = { 0.0f,0.0f,0.0f };
	//std::vector<Vector3> points{Start, Start,end, end};


	//動かすとき
	Vector3 Start = { 0.0f,0.0f,0.0f };
	Vector3 p1 = { 0.0f,-2.0f,5.0f };
	Vector3 p2 = { 0.0f,2.0f,10.0f };
	Vector3 p3 = { 3.0f,0.0f,20.0f };
	Vector3 p4 = { -1.0f,2.0f,25.0f };
	Vector3 p5 = { 1.0f,-2.0f,30.0f };
	Vector3 p6 = { 0.0f,0.0f,35.0f };
	Vector3 p7 = { 1.0f,2.0f,40.0f };
	Vector3 p8 = { 3.0f,0.0f,45.0f };
	Vector3 p9 = { 1.0f,-1.0f,50.0f };
	Vector3 p10 = { 0.0f,0.0f,55.0f };
	Vector3 p11 = { 2.0f,2.0f,60.0f };
	Vector3 p12 = { 0.0f,0.0f,65.0f };
	Vector3 p13 = { -1.0f,0.0f,70.0f };
	Vector3 p14 = { 0.0f,0.0f,75.0f };
	Vector3 p15 = { -2.0f,3.0f,80.0f };
	Vector3 p16 = { 0.0f,0.0f,85.0f };
	Vector3 p17 = { -2.0f,1.0f,170.0f };
	Vector3 p18 = { 0.0f,-1.0f,175.0f };
	Vector3 end = { 0.0f,0.0f,180.0f };
	std::vector<Vector3> points{ 
		Start, Start, 
		p1, p2, p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15,p16,p17,p18,
		end, end 
	};
	
};