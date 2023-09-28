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
#include "Obstacle.h"
#include "EnemyBoss.h"

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
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;

	SpriteCommon* spriteCommon = nullptr;
	Audio* audio = nullptr;
	

private:	//メンバ変数
	const float PI = 3.141592f;

	//シーン切り替え
	enum class SceneNo {
		Title, //タイトル
		Game //射撃
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
	//障害物
	Obstacle* obstacle_ = nullptr;
	//敵Boss
	EnemyBoss* enemyBoss_ = nullptr;

	//--------------------//

	//背景や床
	Object3d* skydome = nullptr;
	Model* skydomeMD = nullptr;
	Object3d* floor = nullptr;
	Model* floorMD = nullptr;

	//タイトル
	Sprite* TitleSprite = new Sprite();
	Object3d* skydomeTit_ = nullptr;
	Model* skydomeTitMD_ = nullptr;
	Object3d* floorTit_ = nullptr;
	Model* floorTitMD_ = nullptr;

	//タイトルで座ってるプレイヤー
	Object3d* standObj_ = nullptr;
	Model* standModel_ = nullptr;

};