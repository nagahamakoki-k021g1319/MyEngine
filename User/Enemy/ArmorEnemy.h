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
	void EffSummary(Vector3 bulletpos);
	//エフェクトの情報(地面のズサ)
	void EffSummary2(Vector3 bulletpos2);
	//エフェクトの情報(背中の噴射ガス)
	void EffSummary3(Vector3 bulletpos3);
	//エフェクトの情報(背中の噴射ガス)
	void EffSummary4(Vector3 bulletpos4);

	//エフェクトの情報(地面のズサ)
	void EffSummary_2(Vector3 bulletpos);
	//エフェクトの情報(地面のズサ)
	void EffSummary2_2(Vector3 bulletpos2);
	//エフェクトの情報(背中の噴射ガス)
	void EffSummary3_2(Vector3 bulletpos3);
	//エフェクトの情報(背中の噴射ガス)
	void EffSummary4_2(Vector3 bulletpos4);

	//エフェクトの描画
	void EffDraw();

	////ワールド座標を取得
	Vector3 GetWorldPosition();
	Vector3 GetWorldPosition2();

	Vector3 GetWorldBulletPosition();
	Vector3 GetWorldBulletPosition2();

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
	Object3d* Obj_ = nullptr;
	Object3d* Obj2_ = nullptr;
	Model* Model_ = nullptr;
	Model* Modelst_ = nullptr;
	bool isAliveFlag = true;
	bool isAliveFlag2= true;

	//体力
	int HP = 12;
	int HP2 = 12;

	//大砲の弾
	Object3d* bulletObj_ = nullptr;
	Model* bulletModel_ = nullptr;
	bool isShootFlag = false;
	int BulletdurationTime = 0;
	int BulletCoolTime = -60;
	Vector3 playerlen;
	Vector3 bitweenlen;

	Object3d* bulletObj2_ = nullptr;
	bool isShootFlag2 = false;
	int BulletdurationTime2 = 0;
	int BulletCoolTime2 = -80;
	Vector3 playerlen2;
	Vector3 bitweenlen2;

	//魔導兵の移動
	bool isMoveFlag = false;
	bool isbesideFlag = false;

	bool isMoveFlag2 = false;
	bool isbesideFlag2 = false;

	//当たり判定のモデル
	Object3d* collObj_ = nullptr;
	Model* collModel_ = nullptr;
	bool isCollFlag = false;

	Object3d* collObj2_ = nullptr;
	bool isCollFlag2 = false;

	//パーティクル
	//地面のズサ
	std::unique_ptr<ParticleManager> gasParticle;
	std::unique_ptr<ParticleManager> gasParticle2;
	//背中の噴射ガス
	std::unique_ptr<ParticleManager> gasParticle3;
	std::unique_ptr<ParticleManager> gasParticle4;
	int bulletEffTimer_ = 0;
	int isbulletEffFlag_ = 0;

	//地面のズサ
	std::unique_ptr<ParticleManager> gasParticle_2;
	std::unique_ptr<ParticleManager> gasParticle2_2;
	//背中の噴射ガス
	std::unique_ptr<ParticleManager> gasParticle3_2;
	std::unique_ptr<ParticleManager> gasParticle4_2;
	int bulletEffTimer2_ = 0;
	int isbulletEffFlag2_ = 0;

	//ポリゴン爆さん
	bool isExpolFlag = false;
	float ExpolTimer = 0;
	float ExpolMT = 40;
	
	bool isExpolFlag2 = false;
	float ExpolTimer2 = 0;
	float ExpolMT2 = 40;

};









