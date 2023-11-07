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

class Player;


class ArmorEnemy{
public:
	ArmorEnemy();
	~ArmorEnemy();

	void Initialize(DirectXCommon* dxCommon,Input* input);
	void Update(Vector3 playerPos,Vector3 playerBpos,bool playerShootFlag);

	void Draw();

	//エフェクトの更新処理
	void EffUpdate();
	//エフェクトの情報(地面のズサ)
	void EffSummary(Vector3 bulletpos,int num);
	//エフェクトの情報(地面のズサ)
	void EffSummary2(Vector3 bulletpos2,int num2);
	//エフェクトの情報(背中の噴射ガス)
	void EffSummary3(Vector3 bulletpos3,int num3);
	//エフェクトの情報(背中の噴射ガス)
	void EffSummary4(Vector3 bulletpos4,int num4);

	//エフェクトの描画
	void EffDraw();

	void SetPlayer(Player* player) {player_ = player;};

	
private:
	const float PI = 3.141592f;
	Input* input_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
	Audio* audio = nullptr;
	Player* player_ = nullptr;
	SpriteCommon* spriteCommon = nullptr;
	Collision coll;

	int isGameStartTimer = 0;

	//待機(魔導兵)
	//maehito usirohito
	Object3d* Obj_[ 2 ] = { nullptr};
	Model* Model_[ 2 ] = { nullptr };
	Model* Modelst_[ 2 ] = { nullptr };

	//敵の生存フラグ
	int isAliveFlag_[2] = {0};

	//当たり判定のモデル
	Object3d* collObj_[ 2 ] = {nullptr};
	Model* collModel_[ 2 ] = { nullptr };
	int isCollFlag_[ 2 ] = {0};
	//ポリゴン爆さん
	int isExpolFlag_[ 2 ] = { 0 };
	float ExpolTimer_[ 2 ] = { 0 };
	float ExpolMT_[ 2 ] = { 40 };
	float polygon_[ 2 ];

	//体力(一旦0で初期化のタイミングで設定する)
	int HP_[2] = {0};

	//大砲の弾
	Object3d* bulletObj_[ 2 ] = {0};
	Model* bulletModel_[ 2 ] = { 0 };
	int isShootFlag_[ 2 ] = { 0 };
	int BulletdurationTime_[ 2 ] = { 0 };
	int BulletCoolTime_[ 2 ] = { 0 };
	Vector3 playerlen_[2];
	Vector3 bitweenlen_[2];

	//魔導兵の移動
	bool isMoveFlag = false;
	bool isbesideFlag = false;

	//自機の当たり判定
	Object3d* collObj3_ = nullptr;

	//パーティクル
	//地面のズサ
	std::unique_ptr<ParticleManager> gasParticle_[2];
	std::unique_ptr<ParticleManager> gasParticle2_[ 2 ];
	//背中の噴射ガス
	std::unique_ptr<ParticleManager> gasParticle3_[ 2 ];
	std::unique_ptr<ParticleManager> gasParticle4_[ 2 ];
	int gasEffTimer_[ 2 ] = { 0 };
	int isgasEffFlag_[ 2 ] = { 0 };

	
};









