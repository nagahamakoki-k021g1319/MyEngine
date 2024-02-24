#pragma once

#include "DirectXCommon.h"
#include "Input.h"
#include "Audio.h"
#include <string>
#include "Object3d.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "Model.h"

#include "Matrix4.h"
#include "Camera.h"

#include "ParticleManager.h"

#include "Player.h"
#include "Enemy.h"
#include "Enemy/ArmorEnemy.h"
#include "Enemy/BikeEnemy.h"
#include "Enemy/BossEnemy.h"
#include "ornament/Lamp.h"
#include "ornament/Block.h"
#include "Light/LightGroup.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene
{
private: // 静的メンバ変数
	//static const int debugTextTexNumber = 0;

public: // メンバ関数

	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXCommon* dxCommon, Input* input);

	void Reset();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	Vector3 bVelocity(Vector3& velocity, Transform& worldTransform);

public:
	//音を止める関数
	IXAudio2SourceVoice* pSourceVoice[10] = { 0 };



private: // メンバ変数 (固定)
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;

	SpriteCommon* spriteCommon = nullptr;
	Audio* audio = nullptr;

	//// オブジェクト共通のライトの初期化
	//LightGroup* light = nullptr;

	//// 平行光源
	//bool isActiveDirectional = true;

	//// 点光源のステータス
	//float pointLightPos[ 3 ] = { 0,1,0 };
	//float pointLightColor[ 3 ] = { 1,1,1 };
	//float pointLightAtten[ 3 ] = { 0.3f,0.1f,0.1f };
	//bool isActivePoint = false;



private:	//メンバ変数
	const float PI = 3.141592f;

	//シーン切り替え
	enum class SceneNo {
		Title, //タイトル
		Load, //ロード画面
		Game, //射撃
		Clear, //ゲームクリア
		Over  //ゲームオーバー 
	};
	SceneNo sceneNo_ = SceneNo::Title;



	//カメラ
	Camera* mainCamera = nullptr;
	Camera* camera1 = nullptr;
	Camera* camera2 = nullptr;
	Camera* camera3 = nullptr;

	//プレイヤー
	Player* player_ = nullptr;

	//-------敵関連--------//
	//雑魚敵
	Enemy* enemy_ = nullptr;
	ArmorEnemy* armorEnemy_ = nullptr;
	BikeEnemy* bikeEnemy_ = nullptr;
	BossEnemy* bossEnemy_ = nullptr;

	//左右ランプ
	Lamp* lamp_ = nullptr;

	//障害物
	Block* block_ = nullptr;
	//--------------------//

	//ゲームシーンの背景や床
	Object3d* skydome = nullptr;
	Model* skydomeMD = nullptr;

	Object3d* floor_[ 100 ] = {0};
	Model* floorMD = nullptr;
	Object3d* floor2_[ 100 ] = { 0 };
	Model* floorMD2 = nullptr;
	Object3d* floor3_[ 100 ] = { 0 };
	Model* floorMD3 = nullptr;
	Object3d* floor4_[ 30 ] = { 0 };
	Model* floorMD4 = nullptr;

	Object3d* cloudfloor_ = nullptr;
	Model* cloudfloorMD_ = nullptr;
	Object3d* cloudfloor2_ = nullptr;
	Model* cloudfloorMD2_ = nullptr;
	Object3d* cloudfloor3_ = nullptr;
	Model* cloudfloorMD3_ = nullptr;


	//タイトル
	Sprite* TitleSprite = new Sprite();
	Object3d* skydomeTit_ = nullptr;
	Model* skydomeTitMD_ = nullptr;
	Object3d* floorTit_[ 3 ] = {0};
	Model* floorTitMD_[ 3 ] = { 0 };
	Sprite* st = nullptr;
	int stTimer = 0;
	Sprite* xboxCon = nullptr;

	//タイトルのプレイヤー
	Object3d* standObj_ = nullptr;
	Model* standModel_ = nullptr;
	Model* standModel2_ = nullptr;
	int spintimer = 0;

	//タイトルからゲームシーンへの暗転
	Sprite* bbout1 = nullptr;
	Sprite* bbout2 = nullptr;
	Sprite* bbout3 = nullptr;
	int bboutTimer = 0;
	bool isbboutFlag = false;

	//ロード画面
	Sprite* loadSprite = nullptr;
	int loadTimer = 0;

	//ゲームクリア
	Sprite* ClearSprite = nullptr;
	//クリアのプレイヤー
	Object3d* slipObj_ = nullptr;
	Model* slipModel_ = nullptr;
	//クリア時に背景がとまる
	int clearTimer = 0;

};