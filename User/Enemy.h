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
#include "SplinePosition.h"

class Enemy
{
public:
	Enemy();
	~Enemy();

	void Initialize(DirectXCommon* dxCommon, Input* input);
	void Update(SplinePosition* spPosition_);
	void WinpUpdate();
	void Draw();
	void FbxDraw();

	//�G�t�F�N�g�̍X�V����
	void EffUpdate();
	//�G�t�F�N�g�̏��
	void EffSummary(Vector3 enemyPos);
	void EffSimpleSummary();
	//�G�t�F�N�g�̕`��
	void EffDraw();

	////���[���h���W���擾
	Vector3 GetWorldPosition();

	////���[���h���W���擾(�G���G)
	Vector3 GetWinpWorldPosition();

	void OnColision();

	/// <summary>
	/// �|�W�V����
	/// </summary>
	/// <param name="pos"></param>
	void SetPos(Vector3 pos) { fbxObject3d_->wtf.position = pos; };
	void SetCamera(Camera* cam) { camera = cam; };
	void SetPlayer(Player* player) { player_ = player; };

public:
	//�����~�߂�֐�
	IXAudio2SourceVoice* pSourceVoice[10] = { 0 };
	//�G���G�������n�߂�
	bool bossGostAt = false;

private:
	const float PI = 3.141592f;
	Input* input_ = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Audio* audio = nullptr;
	Player* player_ = nullptr;
	Camera* camera = nullptr;
	SplinePosition* splinePosition_ = nullptr;

	Collision coll;
	//�ҋ@(������Boss)
	FBXModel* fbxModel_ = nullptr;
	FBXObject3d* fbxObject3d_ = nullptr;
	int bossGostMove = 0;

	//�G���G
	FBXModel* fbxWinpModel_ = nullptr;
	FBXObject3d* fbxWinpObject3d_[10] = {0};
	bool isAliveFlag = true;
	//�����Ă��邩�ǂ���(0������,1���S)
	int isWinpAliveFlag_[10] = { 0 };

	//�G���G�o���^�C�}�[
	int winpArrivalTimer = 0;

	//���[�J���ړ����W
	Vector3 enemyWinplocalpos0 = { 10.0f,-1.0f,10.0f };//{ 0.0f,-1.0f,10.0f };
	Vector3 enemyWinplocalpos1 = { 10.0f,0.0f,10.0f };//{ 3.0f,0.0f,10.0f }
	Vector3 enemyWinplocalpos2 = { -10.0f,0.0f,10.0f };//{ -3.0f,0.0f,10.0f }
	Vector3 enemyWinplocalpos3 = { -10.0f,1.0f,10.0f };//{ 0.0f,1.0f,10.0f 
	
	Vector3 enemyWinplocalpos4 = {  12.0f, 2.0f,10.0f };//{  2.0f, 2.0f,10.0f };
	Vector3 enemyWinplocalpos5 = { -12.0f, 2.0f,10.0f };//{ -2.0f, 2.0f,10.0f };
	Vector3 enemyWinplocalpos6 = {  12.0f,-2.0f,10.0f };//{  2.0f,-2.0f,10.0f };
	Vector3 enemyWinplocalpos7 = { -12.0f,-2.0f,10.0f };//{ -2.0f,-2.0f,10.0f };
	
	Vector3 enemyWinplocalpos8 = { 2.0f,0.0f,150.0f };//{ 2.0f,0.0f,10.0f }
	Vector3 enemyWinplocalpos9 = { -2.0f,0.0f,150.0f };//{ -2.0f,0.0f,10.0f }

	//�~�߂�Ƃ�
	Vector3 Start = { 0.0f,0.0f,0.0f };
	Vector3 end = { 0.0f,10.0f,10.0f };
	std::vector<Vector3> points{ Start, Start,end, end };

	//�e����(����)
	Object3d* shootObj_ = nullptr;
	Model* shootModel_ = nullptr;
	Object3d* shootObj2_ = nullptr;
	Model* shootModel2_ = nullptr;
	int isShootFlag_[2] = {0};
	int isShootexistTimer_[2] = {0};
	int isShootCoolTimer_[2] = { 0 };

	//�p�[�e�B�N��
	std::unique_ptr<ParticleManager> DamageParticle;
	int EffTimer_[10] = {0};
	int isEffFlag_[10] = {0};

	const float moveSpeed_ = 0.1f;
	const float rotaSpeed_ = 0.1f;

	

	//���[���h���W������ϐ�
	Vector3 worldPos;

};

