#pragma once
#include "DirectXCommon.h"
#include "Object3d.h"
#include "Input.h"
#include "Camera.h"

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
	void Update();

	void Draw();
	void FbxDraw();

	//プレイヤーの行動一覧
	void PlayerAction();

	Vector3 bVelocity(Vector3& velocity, Transform& worldTransform);

	////ワールド座標を取得
	Vector3 GetWorldPosition();
	//ワールド座標を取得(弾)
	Vector3 GetBulletWorldPosition();
	//ワールド座標を取得(レティクル)
	Vector3 GetRetWorldPosition();
	//ワールド座標を取得(抜刀)
	Vector3 GetSwordWorldPosition();



	Vector3 GetPos() { return fbxObject3d_->wtf.position; };


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

private:
	const float PI = 3.141592f;
	Input* input_ = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Audio* audio = nullptr;
	Enemy* enemy_ = nullptr;
	

	//待機
	FBXModel* fbxModel_ = nullptr;
	FBXObject3d* fbxObject3d_ = nullptr;
	
	//レティクル
	Object3d* retObj_ = nullptr;
	Model* retModel_ = nullptr;

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
	Vector3 enemylen2;
	Vector3 len2;
	int storeStBulletTime = 0;
	

	//弾の弾数表示



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

	bool isAliveFlag = true;

	//ローカル移動
	Vector3 playerlocalpos = { 0.0f,-0.1f,0.0f };
	Vector3 playerlocalpos0 = { 0.0f,0.0f,-3.0f };
	Vector3 retlocalpos = { 0.0f,0.0f,0.0f };

	//スプライン曲線
	int splineTimer = 0;
	
	//止めるとき
	/*Vector3 Start = { 0.0f,0.0f,0.0f };
	Vector3 end = { 0.0f,0.0f,0.0f };
	std::vector<Vector3> points{Start, Start,end, end};*/

	//動かすとき
	Vector3 Start = { 0.0f,0.0f,0.0f };
	Vector3 p1 = { 0.0f,-2.0f,5.0f };
	Vector3 p2 = { 0.0f,2.0f,10.0f };
	Vector3 p3 = { 3.0f,0.0f,20.0f };
	Vector3 p4 = { -1.0f,2.0f,25.0f };
	Vector3 p5 = { 1.0f,-2.0f,30.0f };
	Vector3 p6 = { 0.0f,0.0f,35.0f };
	Vector3 end = { 0.0f,0.0f,100.0f };
	std::vector<Vector3> points{ 
		Start, Start, 
		p1, p2, p3,p4,p5,p6,
		end, end 
	};
	
};