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
#include "Enemy.h"

class EnemyBoss
{
public:
	EnemyBoss();
	~EnemyBoss();

	void Initialize(DirectXCommon* dxCommon, Input* input);
	void Update();

	void Draw();
	void FbxDraw();

	////���[���h���W���擾
	Vector3 GetWorldPosition();

	void SetPlayer(Player* player) { player_ = player; };
	void SetEnemy(Enemy* enemy) { enemy_ = enemy; };

public:
	//�����~�߂�֐�
	IXAudio2SourceVoice* pSourceVoice[10] = { 0 };

private:
	const float PI = 3.141592f;
	Input* input_ = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Audio* audio = nullptr;
	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;
	Collision coll;

	//�{�X
	FBXModel* fbxModel_ = nullptr;
	FBXObject3d* fbxObject3d_ = nullptr;
	//�����Ă��邩�ǂ���(0������,1���S)
	int isObsAliveFlag_ = 0;

	const float moveSpeed_ = 0.1f;
	const float rotaSpeed_ = 0.1f;



	//���[���h���W������ϐ�
	Vector3 worldPos;



};

