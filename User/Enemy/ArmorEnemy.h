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
#include "Enemy.h"

class Player;

//敵の数(ここいじれば敵の数やエフェクトなどもろもろの上げ下げ出来る)
enum EnemyNumber
{
	MaxNumber = 4,//敵の最大数
	MinNumber = 0//一番はじめ
};



class ArmorEnemy : public Enemy
{
public:
	ArmorEnemy();
	~ArmorEnemy();
	void AEReset();

	void Initialize(DirectXCommon* dxCommon,Input* input);
	void Update(Vector3 playerPos,Vector3 playerBpos,Vector3 playerB2pos,Vector3 playerB3pos, bool playerShootFlag);

	void Draw();

	void Reset();

	//エフェクトの更新処理
	void EffUpdate();
	//エフェクトの情報(地面のズサ)
	void EffSummary(Vector3 bulletpos,const int& num);
	//エフェクトの情報(地面のズサ)
	void EffSummary2(Vector3 bulletpos2,const int& num2);
	//エフェクトの情報(背中の噴射ガス)
	void EffSummary3(Vector3 bulletpos3,const int& num3);
	//エフェクトの情報(背中の噴射ガス)
	void EffSummary4(Vector3 bulletpos4,const int& num4);
	//エフェクトの情報(ダメージ受けた時の火花)
	void DamageSummary(Vector3 EnePos,const int& eneNum);
	//エフェクトの情報(発砲するときの硝煙)
	void smokeSummary(Vector3 EnePos,const int& eneNum);
	//エフェクトの情報(体力半分になった時にでる煙)
	void DamagefumeSummary(Vector3 fumepos,const int& num);
	//発砲時の弾速エフェクト
	void ballisticfumeSummary(Vector3 bulletpos,const int& num);

	//エフェクトの描画
	void EffDraw();

	void SetPlayer(Player* player) {
		player_ = player;
	};


private:
	const float PI = 3;
	Input* input_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
	Audio* audio = nullptr;
	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;
	SpriteCommon* spriteCommon = nullptr;
	Collision coll;


	int isGameStartTimer = 0;

	//待機(魔導兵)
	//maehito usirohito
	Object3d* Obj_[ MaxNumber ] = { nullptr };
	Model* Model_[ MaxNumber ] = { nullptr };
	Model* Modelst_[ MaxNumber ] = { nullptr };



	//敵の生存フラグ(0 生きる,1 死亡)
	int isAliveFlag_[ MaxNumber ] = { 1,1,1,1 };
	int AliveR2Timer = 0;
	int AliveR3Timer = 0;
	int AliveR4Timer = 0;

	//死んだときにタイマー動す
	int deathTimer_[ MaxNumber ] = { 0 };

	//定位置についてからこっちをむくフラグ
	int isRotFlag_[ MaxNumber ] = { 0 };
	int RotTimer_[ MaxNumber ] = { 0 };

	//順番に登場
	int entryTimer = 0;

	//当たり判定のモデル
	Object3d* collObj_[ MaxNumber ] = { nullptr };
	Model* collModel_[ MaxNumber ] = { nullptr };
	int isCollFlag_[ MaxNumber ] = { 0 };
	//ポリゴン爆さん
	int isExpolFlag_[ MaxNumber ] = { 0 };
	float ExpolTimer_[ MaxNumber ] = { 0 };
	float ExpolMT_[ MaxNumber ] = { 40,40,40,40 };
	float polygon_[ MaxNumber ];

	//体力(一旦0で初期化のタイミングで設定する)
	int HP_[MaxNumber ] = { 0 };

	//大砲の弾
	Object3d* bulletObj_[ MaxNumber ] = { 0 };
	Model* bulletModel_[ MaxNumber ] = { 0 };
	int isShootFlag_[ MaxNumber ] = { 0 };
	int BulletdurationTime_[ MaxNumber ] = { 0 };
	int BulletCoolTime_[ MaxNumber ] = { 0 };
	Vector3 playerlen_[ MaxNumber ];
	Vector3 bitweenlen_[ MaxNumber ];

	//魔導兵の移動
	int isMoveFlag_[ MaxNumber ] = { 0 };
	int isbesideFlag_[ MaxNumber ] = { 0 };

	//移動制限
	//右
	int moveRLimit_[ MaxNumber ] = { 10,0,10,0 };
	//左
	int moveLLimit_[ MaxNumber ] = { 3,-5,-3,-5 };

	//自機の当たり判定
	Object3d* collObj3_ = nullptr;

	//パーティクル
	//地面のズサ
	std::unique_ptr<ParticleManager> gasParticle_[ MaxNumber ];
	std::unique_ptr<ParticleManager> gasParticle2_[ MaxNumber ];
	const float rnd_posGas= 0.0f;
	const float rnd_velGas = 0.1f;
	const float rnd_accG = 0.000001f;
	//背中の噴射ガス
	std::unique_ptr<ParticleManager> gasParticle3_[ MaxNumber ];
	std::unique_ptr<ParticleManager> gasParticle4_[ MaxNumber ];
	const float rnd_velBGas = -0.07f;
	int gasEffTimer_[ MaxNumber ] = { 0 };
	int isgasEffFlag_[ MaxNumber ] = { 0 };
	//攻撃受けた時の火花のパーティクル
	std::unique_ptr<ParticleManager> DamageParticle_[ MaxNumber ];
	const float rnd_posDamage = 5.0f;
	const float rnd_velDamage = 0.5f;
	int damEffTimer_[ 4 ] = { 0 };
	int isdamEffFlag_[ 4 ] = { 0 };
	//発砲するときの硝煙
	std::unique_ptr<ParticleManager> smokeParticle_[ MaxNumber ];
	const float rnd_smokepos = 0.001f;
	const float rnd_smokevel = 0.05f;
	int smoEffTimer_[ MaxNumber ] = { 0 };
	int isSmoEffFlag_[ MaxNumber ] = { 0 };
	//体力少ないときに煙出る(体力半分)
	std::unique_ptr<ParticleManager> fumeParticle_[ MaxNumber ];
	const float rnd_posDamagefume = 0.1f;
	const float rnd_velDamagefume = 0.0f;
	const float rnd_velGy = -0.1f;
	int fumeEffTimer_[ MaxNumber ] = { 0 };
	int isFumeEffFlag_[ MaxNumber ] = { 0 };
	//発砲時の弾速エフェクト
	std::unique_ptr<ParticleManager> ballisticParticle_[MaxNumber ];
	const float rnd_posBallistic = 0.03f;
	const float rnd_velBallistic = 0.01f;
	int ballisticEffTimer_[ MaxNumber ] = { 0 };
	int isballisticEffFlag_[ MaxNumber ] = { 0 };

};









