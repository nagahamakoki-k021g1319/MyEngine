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

#include "PlayerEffect.h"
#include "PlayerUI.h"


class Enemy;
class ArmorEnemy;

enum ActionState
{
	IDELBlade = 0,
	IDELBladeBefore,

};

class Player
{
public:
	Player();
	~Player();

	

	void Reset();

	void Initialize(DirectXCommon* dxCommon,Input* input);
	void UIInitialize();

	void Update();

	void Draw();
	void FbxDraw();
	void UIDraw();

	//プレイヤーの行動一覧
	void PlayerAction();

	//エフェクトの更新処理
	void EffUpdate();
	//エフェクトの情報(通常ガス右)
	void EffSummary(Vector3 bulletpos);
	//エフェクトの情報(通常ガス左)
	void EffSummary2(Vector3 bulletpos2);
	//エフェクトの情報(加速ガス右)
	void EffSummaryAccelR(Vector3 bulletpos3);
	//エフェクトの情報(加速ガス左)
	void EffSummaryAccelL(Vector3 bulletpos4);
	//エフェクトの情報(減速ガス右)
	void EffSummaryDecelR(Vector3 bulletpos3);
	//エフェクトの情報(減速ガス左)
	void EffSummaryDecelL(Vector3 bulletpos4);
	//エフェクトの情報(剣チャージ)
	void EffSummarySwordchage(Vector3 pos);
	//エフェクトの情報(弾)
	void EffSummaryBullet(Vector3 bulletpos);
	void EffSummaryBullet2(Vector3 bulletpos);
	void EffSummaryBullet3(Vector3 bulletpos);
	//エフェクトの情報(右スピン)
	void EffSummaryRSpin(Vector3 pos);
	//エフェクトの情報(左スピン)
	void EffSummaryLSpin(Vector3 pos);
	//エフェクトの描画
	void EffDraw();



	Vector3 bVelocity(Vector3& velocity,Transform& worldTransform);

	////ワールド座標を取得
	Vector3 GetWorldPosition();
	//ワールド座標を取得(弾)
	Vector3 GetBulletWorldPosition();
	Vector3 GetBulletWorldPosition2();
	Vector3 GetBulletWorldPosition3();
	//ワールド座標を取得(近接攻撃)
	Vector3 GetSwordLeftWorldPosition();
	Vector3 GetSwordRightWorldPosition();
	//ワールド座標を取得(左右の押し出し)
	Vector3 GetCollLeftWorldPosition();
	Vector3 GetCollRightWorldPosition();

	//ワールド座標を取得(レティクル)
	Vector3 GetRetWorldPosition();


	Vector3 GetPos() {
		return Obj_->wtf.position;
	};
	Vector3 GetCamShake() {
		return camShakeVec;
	};


//ゲームが始まるときのムービー
	void GameStartMovie();

	//被弾時のカメラシェイク
	void DamageCamShake();

	/// <summary>
	/// ポジション
	/// </summary>
	/// <param name="pos"></param>
	void SetPos(Vector3 pos) {
		Obj_->wtf.position = pos;
	};
	void SetCamera(Camera* cam) {
		camera = cam;
	};

public:
	//音を止める関数
	IXAudio2SourceVoice* pSourceVoice[ 10 ] = { 0 };

private:
	const float PI = 3.141592f;
	Input* input_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
	Audio* audio = nullptr;
	SpriteCommon* spriteCommon = nullptr;
	Collision coll;
	PlayerUI* playerUI_ = nullptr;

	//ゲームが始まるフラグ
	bool isGameStartFlag = false;
	int isGameStartTimer = 0;
	bool acflag = false;
	bool rotaflag = false;
	bool camerasetFlag = false;

	//待機
	//maehito usirohito
	Object3d* Obj_ = nullptr;
	//自機のモデル一覧
	Model* Model_[ 2 ] = {0};
	Model* Model2_ = nullptr;
	Model* ModelAt_ = nullptr;
	Model* ModelBack_ = nullptr;
	Model* Modelst_ = nullptr;
	Model* Modelst2_ = nullptr;
	Model* ModelAc_ = nullptr;
	Model* ModelAc2_ = nullptr;
	//加速するフラグ
	bool isAccelFlag = false;
	//モデル(納刀から抜刀)
	Model* ModelBikswordsty_ = nullptr;
	Model* ModelBikswordsty2_ = nullptr;
	//モデル(左側攻撃)
	Model* Modelbiksword0_ = nullptr;
	int leftAtTimer = 0;
	bool isLeftAtFlag = false;
	Object3d* collSWObj_ = nullptr;
	Model* collSWModel_ = nullptr;
	//モデル(右側攻撃)
	Model* Modelbiksword1_ = nullptr;
	int rightAtTimer = 0;
	bool isRightAtFlag = false;
	Object3d* collSWRightObj_ = nullptr;
	Model* collSWRightModel_ = nullptr;

	//自機が衝突した時用のモデル
	//右
	Model* extrusionRightModel_ = nullptr;
	Object3d* extrusionRightObj_ = nullptr;
	//左
	Model* extrusionLeftModel_ = nullptr;
	Object3d* extrusionLeftObj_ = nullptr;

	//自機が回転攻撃したときに動くモデル(エフェクト用)
	Model* slashModel_ = nullptr;
	//右
	Object3d* slashRObj_ = nullptr;
	bool spineRffflag = false;
	//左
	Object3d* slashLObj_ = nullptr;
	bool spineLffflag = false;



	//自機の納刀モデルから抜刀モデルに切り替え
	int isBikswordstyFlag = 0;
	int BikswordstyTimer = 0;
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
	//バイクの車輪動かす(抜刀後)
	int bikSpinTimer = 0;
	//バイクの車輪動かす(納刀時)
	int bikstSpinTimer = 0;

	//自機の当たり判定のモデル
	Object3d* collObj_ = nullptr;
	Model* collModel_ = nullptr;
	float collpos = 0.0f;

	//バイクの残骸
	Model* debrisModel_ = nullptr;
	Object3d* debrisObj_ = nullptr;


	//自機の生存フラグ
	bool isAliveFlag = true;


	//遠距離攻撃のUIとゲージ
	Sprite* bulletUI = nullptr;
	Vector2 bulletPosition;
	Sprite* bulletgageUI = nullptr;
	Vector2 bulletgagePosition;
	Sprite* gageMaxUI = nullptr;
	Sprite* underBlackUI = nullptr;
	bool isgageUPFlag = false;
	bool isgageStopFlag = false;
	int gageCount = 1;
	int BulletCount = 1;

	//自機のHP表示(スプライト)
	Sprite* bulletEnptyUI = nullptr;
	Sprite* bullet1UI = nullptr;
	Sprite* bullet2UI = nullptr;
	Sprite* bullet3UI = nullptr;

	//レティクル
	Object3d* retObj_ = nullptr;
	Model* retModel_ = nullptr;
	Model* ret1Model_ = nullptr;
	Model* ret2Model_ = nullptr;
	int retResetTimer = 0;
	bool retdisplay = false;
	Object3d* retVisualObj_ = nullptr;
	Model* retVisualModel_ = nullptr;



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
	//ガス(通常)
	std::unique_ptr<ParticleManager> gasParticle;
	std::unique_ptr<ParticleManager> gasParticle2;
	const float rnd_posGas = 0.0f;
	const float rnd_velGasz = 0.02f;
	const float rnd_accGas = 0.000001f;

	//ガス(加速)
	std::unique_ptr<ParticleManager> gasParticleAccelR;
	std::unique_ptr<ParticleManager> gasParticleAccelL;
	//ガス(減速)
	std::unique_ptr<ParticleManager> gasParticleDecelR;
	std::unique_ptr<ParticleManager> gasParticleDecelL;
	int bulletEffTimer_ = 0;
	int isbulletEffFlag_ = 0;
	//自機のバイクのブーストフラグ(0 通常,1 加速,2 減速)
	int isboostFlag = 0;
	//剣(チャージ)
	std::unique_ptr<ParticleManager> swordchageParticle;
	int swordchageEffTimer_ = 0;
	int isswordchageEffFlag_ = 0;
	//弾飛ばし
	std::unique_ptr<ParticleManager> ballisticParticle_[3];
	const float rnd_posBullet = 0.03f;
	const float rnd_velBullet = 0.01f;
	int ballisticEffTimer_ = 0;
	int isBallisticEffFlag_ = 0;
	//右スピンエフェクト
	std::unique_ptr<ParticleManager> RSpinParticle;
	int RSpinEffTimer_ = 0;
	int isRSpinEffFlag_ = 0;
	//左スピンエフェクト
	std::unique_ptr<ParticleManager> LSpinParticle;
	int LSpinEffTimer_ = 0;
	int isLSpinEffFlag_ = 0;


	std::unique_ptr<PlayerEffect>playerEffect;

	//ダメージを受けた時のフラグ
	bool isDamageFlag = false;
	bool hpDeclineFlag = false;
	bool hpFlagReset = false;

	Sprite* metaUI = nullptr;
	Sprite* arrowUI = nullptr;
	Vector2 arrowPosition;
	Vector2 arrowRotation;
	int arrowTimer = 0;

	//エンジンメーターのフラグ
	bool isArrowUpFlag = false;
	bool isArrowDwonFlag = false;

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

	Vector2 camRotaSpeed = { PI / 1800, PI / 1800 };


public:
	//自機のHP表示
	float playerHP = 20.0f;

	//自機とバイク兵の押し出し処理
	bool limitmove = false;
	bool limitmove2 = false;

	//バイク兵との突進でノックバックするフラグ
	//右から敵が突進
	bool isKnockbackFlag = false;
	int knockbackTimer = 0;

	//左から敵が突進
	bool isKnockbackFlagL = false;
	int knockbackTimerL = 0;

	//バイクの残骸が後ろに散らばる
	bool isScatterFlag = false;

	//弾発射(3発)
	Object3d* shootObj_[ 3 ] = {0};
	Model* shootModel_ = nullptr;
	bool isShootFlag = false;
	int BulletCoolTime = 0;
	Vector3 bulletlen_[3];
	Vector3 len_[3];




	//自機の近接攻撃判定がでるフラグ
	//左
	bool isCollSWFlag = false;
	//右
	bool isCollSWRightFlag = false;

	//ゲームクリアするときのバイク移動
	bool isClearFlag = false;
	int isclearFlagTimer = 0;

	//ボス登場でカメラの向きをかえるフラグ
	int isCameraBehavior = 0;
	int CameraBehaviorTimer = 0;
	int CameraBehaviorTimer2 = 0;

	//画面シェイク
	int isCamShake = 0;
	const int camShakeLimit = 20;
	int camShakeTimer = camShakeLimit;
	Vector3 camShakeVec;
	Vector3 moveBack;

	//ラウンド制御(プレイヤー側で設定する)
	int isRoundFlag = 0;
	int isDeadEnemy = 0;
	//ラウンドが変わるたびカメラが一旦引く
	int incidenceCamera = 0;
	int incidenceCamera2 = 0;
	int incidenceCamera3 = 0;
	
	//障害物に当たって減速
	bool isDecelerationFlag = false;
	int DecelerationTimer = 0;
	int backTimer = 0;

	//敵が自機より後ろにいるときカメラを少し下げる
	int standardCamera = 0;
	int moveCamera = 0;
	int cameraResetTimer = 0;
	int BossCameraResetTimer = 0;
	//操作説明
	//1ウェーブ
	Sprite* operationUI = nullptr;
	Sprite* operationbbUI = nullptr;
	bool isOperationFlag = false;
	int OperationbbTimer = 0;

	//2ウェーブ
	Sprite* operation2UI = nullptr;
	Sprite* operationbb2UI = nullptr;
	bool isOperationFlag2 = false;
	bool isOperationFlag3 = false;
	int waveTimer2 = 0;
	int OperationbbTimer2 = 0;

};