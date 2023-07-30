#pragma once
#include "DirectXCommon.h"
#include "Object3d.h"
#include "Input.h"
#include "Camera.h"

class Player;
#include "Collision.h"
#include "ParticleManager.h"
#include "Audio.h"

#include "FBXModel.h"
#include "FbxLoader.h"
#include "FBXObject3d.h"
#include "SplinePosition.h"

class Enemy
{
public:
	Enemy();
	~Enemy();

	void Initialize(DirectXCommon* dxCommon, Input* input);
	void Update(SplinePosition* spPosition_);
	void WinpUpdate();
	void Draw();
	void FbxDraw();

	//エフェクトの更新処理
	void EffUpdate();
	//エフェクトの情報
	void EffSummary(Vector3 enemyPos);
	void EffSimpleSummary();
	//エフェクトの描画
	void EffDraw();

	////ワールド座標を取得
	Vector3 GetWorldPosition();

	////ワールド座標を取得(雑魚敵)
	Vector3 GetWinpWorldPosition();

	void OnColision();

	/// <summary>
	/// ポジション
	/// </summary>
	/// <param name="pos"></param>
	void SetPos(Vector3 pos) { fbxObject3d_->wtf.position = pos; };
	void SetCamera(Camera* cam) { camera = cam; };
	void SetPlayer(Player* player) { player_ = player; };

public:
	//音を止める関数
	IXAudio2SourceVoice* pSourceVoice[10] = { 0 };
	//雑魚敵が動き始める
	bool bossGostAt = false;

private:
	const float PI = 3.141592f;
	Input* input_ = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Audio* audio = nullptr;
	Player* player_ = nullptr;
	Camera* camera = nullptr;
	SplinePosition* splinePosition_ = nullptr;

	Collision coll;
	//待機(消えるBoss)
	FBXModel* fbxModel_ = nullptr;
	FBXObject3d* fbxObject3d_ = nullptr;
	int bossGostMove = 0;

	//雑魚敵
	FBXModel* fbxWinpModel_ = nullptr;
	FBXObject3d* fbxWinpObject3d_[10] = {0};
	bool isAliveFlag = true;
	//生きているかどうか(0生きる,1死亡)
	int isWinpAliveFlag_[10] = { 0 };

	//雑魚敵出現タイマー
	int winpArrivalTimer = 0;

	//ローカル移動座標
	Vector3 enemyWinplocalpos0 = { 10.0f,-1.0f,10.0f };//{ 0.0f,-1.0f,10.0f };
	Vector3 enemyWinplocalpos1 = { 10.0f,0.0f,10.0f };//{ 3.0f,0.0f,10.0f }
	Vector3 enemyWinplocalpos2 = { -10.0f,0.0f,10.0f };//{ -3.0f,0.0f,10.0f }
	Vector3 enemyWinplocalpos3 = { -10.0f,1.0f,10.0f };//{ 0.0f,1.0f,10.0f 
	
	Vector3 enemyWinplocalpos4 = {  12.0f, 2.0f,10.0f };//{  2.0f, 2.0f,10.0f };
	Vector3 enemyWinplocalpos5 = { -12.0f, 2.0f,10.0f };//{ -2.0f, 2.0f,10.0f };
	Vector3 enemyWinplocalpos6 = {  12.0f,-2.0f,10.0f };//{  2.0f,-2.0f,10.0f };
	Vector3 enemyWinplocalpos7 = { -12.0f,-2.0f,10.0f };//{ -2.0f,-2.0f,10.0f };
	
	Vector3 enemyWinplocalpos8 = { 2.0f,0.0f,150.0f };//{ 2.0f,0.0f,10.0f }
	Vector3 enemyWinplocalpos9 = { -2.0f,0.0f,150.0f };//{ -2.0f,0.0f,10.0f }

	//止めるとき
	Vector3 Start = { 0.0f,0.0f,0.0f };
	Vector3 end = { 0.0f,10.0f,10.0f };
	std::vector<Vector3> points{ Start, Start,end, end };

	//弾発射(直線)
	Object3d* shootObj_ = nullptr;
	Model* shootModel_ = nullptr;
	Object3d* shootObj2_ = nullptr;
	Model* shootModel2_ = nullptr;
	int isShootFlag_[2] = {0};
	int isShootexistTimer_[2] = {0};
	int isShootCoolTimer_[2] = { 0 };

	//パーティクル
	std::unique_ptr<ParticleManager> DamageParticle;
	int EffTimer_[10] = {0};
	int isEffFlag_[10] = {0};

	const float moveSpeed_ = 0.1f;
	const float rotaSpeed_ = 0.1f;

	

	//ワールド座標を入れる変数
	Vector3 worldPos;

};

