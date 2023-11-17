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

class Player;


class ArmorEnemy{
public:
	ArmorEnemy();
	~ArmorEnemy();

	void Initialize(DirectXCommon* dxCommon,Input* input);
	void Update(Vector3 playerPos,Vector3 playerBpos,bool playerShootFlag);

	void Draw();

	void Reset();

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
	Object3d* Obj_[ 4 ] = { nullptr};
	Model* Model_[ 4 ] = { nullptr };
	Model* Modelst_[ 4 ] = { nullptr };

	//敵の生存フラグ(0 生きる,1 死亡)
	int isAliveFlag_[4] = {0,0,1,1};
	int AliveR2Timer = 0;
	int AliveR3Timer = 0;

	//当たり判定のモデル
	Object3d* collObj_[ 4 ] = {nullptr};
	Model* collModel_[ 4 ] = { nullptr };
	int isCollFlag_[ 4 ] = {0};
	//ポリゴン爆さん
	int isExpolFlag_[ 4 ] = { 0 };
	float ExpolTimer_[ 4 ] = { 0 };
	float ExpolMT_[ 4 ] = { 40,40,40,40};
	float polygon_[ 4 ];

	//体力(一旦0で初期化のタイミングで設定する)
	int HP_[4] = {0};

	//大砲の弾
	Object3d* bulletObj_[ 4 ] = {0};
	Model* bulletModel_[ 4 ] = { 0 };
	int isShootFlag_[ 4 ] = { 0 };
	int BulletdurationTime_[ 4 ] = { 0 };
	int BulletCoolTime_[ 4 ] = { 0 };
	Vector3 playerlen_[4];
	Vector3 bitweenlen_[4];

	//魔導兵の移動
	int isMoveFlag_[ 4] = {0};
	int isbesideFlag_[ 4 ] = {0};

	//移動制限
	//右
	int moveRLimit_[ 4 ] = {10,0,10,0};
	//左
	int moveLLimit_[ 4 ] = {3,-5,-3,-5};

	//自機の当たり判定
	Object3d* collObj3_ = nullptr;

	//パーティクル
	//地面のズサ
	std::unique_ptr<ParticleManager> gasParticle_[4];
	std::unique_ptr<ParticleManager> gasParticle2_[4];
	//背中の噴射ガス
	std::unique_ptr<ParticleManager> gasParticle3_[4];
	std::unique_ptr<ParticleManager> gasParticle4_[4];
	int gasEffTimer_[4] = { 0 };
	int isgasEffFlag_[4] = { 0 };

	
};









