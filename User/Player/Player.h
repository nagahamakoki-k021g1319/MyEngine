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

#include "PlayerUI.h"


class Enemy;
class ArmorEnemy;

//プレイヤーの行動モデル
enum ActionState
{
	IDEL,//納刀状態1
	IDELBefore,//納刀状態2
	IDELBlade,//抜刀状態1
	IDELBladeBefore,//抜刀状態2
	ModeChangeMid,//納刀から抜刀へ
	ModeChange,//納刀から抜刀へ2
	Accel,//加速1
	AccelBefore,//加速2
	BulletAttack,//遠距離攻撃
	BulletAttackMid,//遠距離攻撃するまでの予備動作
	LSpinAttack,//左スピン攻撃
	RSpinAttack,//右スピン攻撃
	MaxModel //最大
};
//当たり判定用のモデル
enum CollisionObject
{
	PlayerColl,//自機の中心当たり判定
	LAttackColl,//左攻撃の当たり判定
	RAttackColl,//右攻撃の当たり判定
	LPlayerColl,//左側の自機当たり判定
	RPlayerColl,//右側の自機当たり判定
	LSparkColl,//左側から火花出すための場所
	RSparkColl,//右側から火花出すための場所
	MaxColl //最大
};
//弾の数
enum BulletNumber
{
	FirstBullet,
	SecondBullet,
	ThirdBullet,
	MaxBullet
};
//スプライト情報
enum Spritekinds
{
	blood,//被弾時
	bulletgage,//弾倉のUI
	bluegage,//動く青いバー
	gagemax,//maxの小さな文字
	gageblack,//下地の黒
	bulletenp,//エンプティ時
	bullet1,//弾1発
	bullet2,//弾2発
	bullet3,//弾3発
	meta,//メーター部分
	arrow,//メーター矢印
	operation,  //自機の操作説明
	operationbb,//↑↑↑これの視線誘導
	operation2,  //自機の操作説明2
	operation2bb,//↑↑↑これの視線誘導
	MaxSprite //最大
};
//バイクのガスエンジンのフラグとパーティクル
enum bikeGas
{
	GasNormal = 0, //通常状態のガス
	GasAccel, //加速状態のガス
	GasDecel, //減速状態のガス
	NormalRight = 0,//通常状態の右側パーティクル
	NormalLeft, //通常状態の左側パーティクル
	AccelRight, //加速状態の右側パーティクル
	AccelLeft, //加速状態の左側パーティクル
	MaxGasParticle //ガスの最大
};
//バイクスピンのパーティクル
enum bikeSpin
{
	SpinRightParticle,//右側から出る火花のエフェクト
	SpinLeftParticle,//左側から出る火花のエフェクト
	MaxSpinParticle 
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
	void UIDraw();

	//プレイヤーの行動一覧
	void PlayerAction();

	//エフェクトの更新処理
	void EffUpdate();
	//エフェクトの情報(バイクのガス)
	void EffSummary(Vector3 bulletpos,int GasNum,float randVelz,float start_particle);
	//エフェクトの情報(弾)
	void EffSummaryBullet(Vector3 bulletpos, int BulletNum);
	//エフェクトの情報(左右スピン)
	void EffSummarySpin(Vector3 pos,int SpinNum);
	//エフェクトの描画
	void EffDraw();

	////ワールド座標を取得
	Vector3 GetWorldPosition(int CollNumber);
	//ワールド座標を取得(弾)
	Vector3 GetBulletWorldPosition(int BulletNumber);

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
	Object3d* Obj_ = nullptr;
	//自機のモデル一覧
	Model* Model_[ MaxModel ] = {0};
	//自機の当たり判定のオブジェクト一覧
	Object3d* collObj_[ MaxColl ] = {0};
	Model* collModel_ = nullptr;
	//使うスプライト一覧
	Sprite* sprite_[ MaxSprite ] = { 0 };
	//使うスプライトのポジションやローテーション
	Vector2 bulletPosition;
	Vector2 bulletgagePosition;
	Vector2 arrowPosition;
	Vector2 arrowRotation;

	//自機とレティクルの速度マジックナンバーの一覧
	const float addPos = 0.5f;
	const float addPosZ = -1.0f;
	const float addPosZ2 = 1.0f;
	const float addPosX = 0.1f;
	const float playerSpeed = 0.08f;
	const float playerSpeed2 = 0.06f;
	const float retSpeed = 0.08f;
	const float retSpeed2 = 0.20f;
	const float backSpeed = 0.07f;
	const float backSpeedZ = 0.03f;
	float ShortSpeed = 0.01f;
	const float addRetPos = 50.0f;
	const float addRetVPos = 15.0f;
	const float addBulletPos = 0.2f;
	const float addBulletPos2 = 0.4f;
	const float addBulletPosY = 0.8f;
	const float addcameraPosZ = 0.2f;
	const float addcameraPosY = 0.05f;
	const float addcameraPosZ2 = 0.1f;
	const float AtSpeed = 0.32f;
	const float Speed = 0.03f;
	const float bulletgagePos = -187.0f;
	

	//加速するフラグ
	bool isAccelFlag = false;
	//モデル(左側攻撃)
	bool isLeftAtFlag = false;
	//モデル(右側攻撃)
	bool isRightAtFlag = false;
	//自機が回転攻撃したときに動くモデル(左右)
	bool spineLffflag = false;
	bool spineRffflag = false;

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

	float collpos = 0.0f;

	//バイクの残骸
	Model* debrisModel_ = nullptr;
	Object3d* debrisObj_ = nullptr;

	//自機の生存フラグ
	bool isAliveFlag = true;

	//遠距離攻撃のUIとゲージ
	bool isgageUPFlag = false;
	bool isgageStopFlag = false;
	int gageCount = 1;
	int BulletCount = 1;

	//レティクル
	Object3d* retObj_ = nullptr;
	Model* retModel_ = nullptr;
	Object3d* retVisualObj_ = nullptr;
	Model* retVisualModel_ = nullptr;
	int retResetTimer = 0;
	bool retdisplay = false;
	
	//被弾時エフェクト
	int EffTimer = 0;
	int isEffFlag = 0;

	//パーティクル
	//パーティクルで使うマジックナンバーの一覧
	const float gasLPosX = 0.15f;
	const float gasRPosX = 0.13f;
	const float gasAddPosY = 0.2f;
	const float gasLPosZ = 1.5f;
	const float gasRPosZ = 2.5f;
	const float gasNomalRand = 0.2f;
	const float gasNomalStartParticle = 0.05f;
	const float gasAccelRand = 2.5f;
	const float gasAccelStartParticle = 0.07f;
	//ガス(通常)
	std::unique_ptr<ParticleManager> gasParticle_[MaxGasParticle];
	const float rnd_posGas = 0.0f;
	const float rnd_velGasz = 0.02f;
	const float rnd_accGas = 0.000001f;

	int gasEffTimer_ = 0;
	bool isgasEffFlag_ = false;
	//自機のバイクのブーストフラグ(0 通常,1 加速,2 減速)
	int isboostFlag = 0;

	//弾飛ばし
	std::unique_ptr<ParticleManager> ballisticParticle_[MaxBullet];
	const float rnd_posBullet = 0.03f;
	const float rnd_velBullet = 0.01f;
	int ballisticEffTimer_ = 0;
	bool isBallisticEffFlag_ = false;

	//左右スピンエフェクト
	std::unique_ptr<ParticleManager> SpinParticle_[ MaxSpinParticle ];
	int RSpinEffTimer_ = 0;
	int LSpinEffTimer_ = 0;

	//ダメージを受けた時のフラグ
	bool isDamageFlag = false;
	bool hpDeclineFlag = false;
	bool hpFlagReset = false;

	//エンジンメーターのフラグ
	int arrowTimer = 0;
	bool isArrowUpFlag = false;
	bool isArrowDwonFlag = false;
	const float AllowPos = 1.0f;
	const float AllowPos2 = 2.0f;
	const float metaPos = 1.4f;

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

	//ゲームスタート時のムービーで使うマジックナンバーの一覧
	const float PosSpeed = 0.3f;
	const float RotSpeed = 0.05f;
	const float RotSpeed2 = 0.2f;
	const float PosSpeed2 = 0.3f;

	//画面シェイクで使うマジックナンバーの一覧
	const float shakePos = 0.2f;
	const float shakePosY = 0.1f;


public:
	//自機のHP表示
	float playerHP = 20.0f;

	//自機とバイク兵の押し出し処理
	bool limitmove = false;
	bool limitmove2 = false;

	//スピンエフェクト
	bool isRSpinEffFlag_ = false;
	bool isLSpinEffFlag_ = false;
	int leftAtTimer = 0;
	int rightAtTimer = 0;

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
	Object3d* shootObj_[ MaxBullet ] = {0};
	Model* shootModel_ = nullptr;
	Vector3 bulletlen_[ MaxBullet ];
	Vector3 len_[ MaxBullet ];
	bool isShootFlag = false;
	int BulletCoolTime = 0;

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
	bool isOperationFlag = false;
	int OperationbbTimer = 0;
	//2ウェーブ
	bool isOperationFlag2 = false;
	bool isOperationFlag3 = false;
	int waveTimer2 = 0;
	int OperationbbTimer2 = 0;

};