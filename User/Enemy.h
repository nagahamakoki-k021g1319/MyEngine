#pragma once
#include "DirectXCommon.h"
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


class Player;

class Enemy
{
public:
	Enemy();
	~Enemy();

	void Initialize(DirectXCommon* dxCommon, Input* input);
	void UIInitialize();
	void Update(SplinePosition* spPosition_);
	void WinpUpdate();
	void BossWinpUpdate();
	void Draw();
	void FbxDraw();
	void UIDraw();

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
	Vector3 GetWinpWorldPosition(int num);

	////ワールド座標を取得(ボスのファンネル)
	Vector3 GetEnearchWorldPosition(int num);

	////ワールド座標を取得(ボスのファンネル誘導弾)
	Vector3 GetinductionWorldPosition(int num);

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
	//雑魚敵出現タイマー
	int winpArrivalTimer = 0;
	//弾発射(誘導)
	bool isShootStFlag_[2] = { false };

private:
	const float PI = 3.141592f;
	Input* input_ = nullptr;
	DirectXCommon* dxCommon = nullptr;
	SpriteCommon* spriteCommon = nullptr;
	Audio* audio = nullptr;
	Player* player_ = nullptr;
	Camera* camera = nullptr;
	SplinePosition* splinePosition_ = nullptr;

	Collision coll;
	//待機(消えるBoss)
	FBXModel* fbxModel_ = nullptr;
	FBXObject3d* fbxObject3d_ = nullptr;
	bool bossGostMove = false;

	//雑魚敵
	FBXModel* fbxWinpModel_ = nullptr;
	FBXObject3d* fbxWinpObject3d_[13] = {0};
	bool isAliveFlag = true;
	//生きているかどうか(0生きる,1死亡)
	int isWinpAliveFlag_[13] = { 0 };
	//雑魚敵の動き(0右,1左)
	int enemyWinpAction8 = 0;
	int enemyWinpAction9 = 1;
	

	//ローカル移動座標(雑魚敵)
	//雑魚敵の初期位置(第1ウェーブ)
	Vector3 enemyWinplocalpos0 = { 10.0f,-1.0f,10.0f };//{ 0.0f,-1.0f,10.0f };
	Vector3 enemyWinplocalpos1 = { 10.0f,0.0f,10.0f };//{ 3.0f,0.0f,10.0f }
	Vector3 enemyWinplocalpos2 = { -10.0f,0.0f,10.0f };//{ -3.0f,0.0f,10.0f }
	Vector3 enemyWinplocalpos3 = { -10.0f,1.0f,10.0f };//{ 0.0f,1.0f,10.0f 
	//雑魚敵の初期位置(第2ウェーブ)
	Vector3 enemyWinplocalpos4 = {  12.0f, 2.0f,10.0f };//{  2.0f, 2.0f,10.0f };
	Vector3 enemyWinplocalpos5 = { -12.0f, 2.0f,10.0f };//{ -2.0f, 2.0f,10.0f };
	Vector3 enemyWinplocalpos6 = {  12.0f,-2.0f,10.0f };//{  2.0f,-2.0f,10.0f };
	Vector3 enemyWinplocalpos7 = { -12.0f,-2.0f,10.0f };//{ -2.0f,-2.0f,10.0f };
	//雑魚敵の初期位置(第3ウェーブ)
	Vector3 enemyWinplocalpos8 = { 2.0f,-1.0f,150.0f };//{ 2.0f,0.0f,10.0f }
	Vector3 enemyWinplocalpos9 = { -2.0f,1.0f,150.0f };//{ -2.0f,0.0f,10.0f }
	//雑魚敵の初期位置(第4ウェーブ)
	Vector3 enemyWinplocalpos10 = { 8.0f, 2.0f,-10.0f };//{ 8.0f, 2.0f,7.0f };
	Vector3 enemyWinplocalpos11 = { 8.0f, 0.0f,-10.0f };//{ 8.0f,0.0f,5.0f  };
	Vector3 enemyWinplocalpos12 = { 8.0f,-2.0f,-10.0f };//{ 8.0f,-2.0f,3.0f };

	//止めるとき
	Vector3 Start = { 0.0f,0.0f,0.0f };
	Vector3 end = { 0.0f,10.0f,10.0f };
	std::vector<Vector3> points{ Start, Start,end, end };
	int splineTimer = 0;

	//弾発射(直線)
	Object3d* shootObj_ = nullptr;
	Model* shootModel_ = nullptr;
	Object3d* shootObj2_ = nullptr;
	Model* shootModel2_ = nullptr;
	int isShootFlag_[2] = {0};
	int isOffsetFlag_ = 0;
	int isShootexistTimer_[2] = {0};
	int isShootCoolTimer_[2] = { 0 };

	//Bossのファンネル
	Object3d* enearchObj_[2] = {0};
	Model* enearchModel_ = nullptr;
	//生きているかどうか(0生きる,1死亡)
	int isEnearchAliveFlag_[2] = { 0 };
	Vector3 enearchlocalpos0 = { 0.0f,0.0f,15.0f }; //{ 2.0f,2.0f,10.0f };
	Vector3 enearchlocalpos1 = { 0.0f,0.0f,15.0f }; //{ -2.0f,2.0f,10.0f };
	/*Vector3 enearchlocalpos2 = { 2.0f,2.0f,10.0f };
	Vector3 enearchlocalpos3 = { -2.0f,-2.0f,10.0f };*/
	Vector3 playerlen0;
	Vector3 len;
	Vector3 playerlen1;
	Vector3 len1;
	/*Vector3 playerlen2;
	Vector3 playerlen3;*/
	

	//規定の位置まで来たら誘導弾のタイマーを開始
	bool isMoveAction = false;

	//弾発射(誘導)
	Object3d* retObj_[2] = { 0 };
	Model* retModel_ = nullptr;
	Object3d* inductionObj_[2] = { 0 };
	Model* inductionModel_ = nullptr ;
	Vector3 retlocalpos0 = { -1.0f,-2.0f,-5.0f };
	Vector3 retlocalpos1 = {  1.0f,-2.0f,-5.0f };
	int StBulletCoolTime_[2] = {0};
	int storeStBulletTime_[2] = { 0 };




	//パーティクル
	std::unique_ptr<ParticleManager> DamageParticle;
	int EffTimer_[13] = {0};
	int isEffFlag_[13] = {0};

	const float moveSpeed_ = 0.1f;
	const float rotaSpeed_ = 0.1f;

	//ボス前のwarningのUI
	Sprite* warnUI = nullptr;
	Sprite* warnani1UI = nullptr;
	Vector2 warnani1Position;
	Sprite* warnani2UI = nullptr;
	Vector2 warnani2Position;
	int warnaniFlag = 0;
	int warnTimer = 0;

	//ワールド座標を入れる変数
	Vector3 worldPos;

};

