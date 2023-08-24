#include "Enemy.h"
#include "Player.h"
#include <imgui.h>

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
	//FBX�I�u�W�F�N�g���
	delete fbxObject3d_;
	delete fbxModel_;
	for (int i = 0; i < 13; i++) { delete fbxWinpObject3d_[i]; }
	delete fbxWinpModel_;
	delete shootModel_;
	delete shootObj_;

	delete shootModel2_;
	delete shootObj2_;

	delete enearchModel_;
	for (int i = 0; i < 2; i++) { delete enearchObj_[i]; }

	delete inductionModel_;
	for (int i = 0; i < 2; i++) { delete inductionObj_[i]; }

	delete retModel_;
	for (int i = 0; i < 2; i++) { delete retObj_[i]; }


}

void Enemy::Initialize(DirectXCommon* dxCommon, Input* input)
{
	// nullptr�`�F�b�N
	assert(dxCommon);
	assert(input);


	this->dxCommon = dxCommon;
	input_ = input;

	camera = new Camera(WinApp::window_width, WinApp::window_height);
	splinePosition_ = new SplinePosition(points);

	fbxModel_ = FbxLoader::GetInstance()->LoadModelFromFile("kuma");
	fbxWinpModel_ = FbxLoader::GetInstance()->LoadModelFromFile("enemyfanneru");
	// �f�o�C�X���Z�b�g
	FBXObject3d::SetDevice(dxCommon->GetDevice());
	// �O���t�B�b�N�X�p�C�v���C������
	FBXObject3d::CreateGraphicsPipeline();

	//�ҋ@(Boss)
	fbxObject3d_ = new FBXObject3d;
	fbxObject3d_->Initialize();
	fbxObject3d_->SetModel(fbxModel_);
	fbxObject3d_->wtf.position = { 0.0f,0.1f,0.0f };
	fbxObject3d_->wtf.scale = { 0.04f,0.04f,0.04f };
	fbxObject3d_->PlayAnimation(0.5f, false);

	//�G���G
	for (int i = 0; i < 13; i++) {
		fbxWinpObject3d_[i] = new FBXObject3d;
		fbxWinpObject3d_[i]->Initialize();
		fbxWinpObject3d_[i]->SetModel(fbxWinpModel_);
		fbxWinpObject3d_[i]->wtf.scale = { 0.1f,0.1f,0.1f };
		fbxWinpObject3d_[i]->wtf.position.z = 30.0f;
		fbxWinpObject3d_[i]->PlayAnimation(1.0f, true);
	}


	//�G���G�̍U��
	shootModel_ = Model::LoadFromOBJ("eneboll");
	shootObj_ = Object3d::Create();
	shootObj_->SetModel(shootModel_);
	shootObj_->wtf.position = { fbxWinpObject3d_[8]->wtf.position.x,fbxWinpObject3d_[8]->wtf.position.y, fbxWinpObject3d_[8]->wtf.position.z };

	shootModel2_ = Model::LoadFromOBJ("eneboll");
	shootObj2_ = Object3d::Create();
	shootObj2_->SetModel(shootModel2_);
	shootObj2_->wtf.position = { fbxWinpObject3d_[9]->wtf.position.x,fbxWinpObject3d_[9]->wtf.position.y, fbxWinpObject3d_[9]->wtf.position.z };
	shootObj2_->wtf.scale = { 0.4f,0.4f,0.4f };


	//Boss�̃t�@���l��
	enearchModel_ = Model::LoadFromOBJ("enearch");
	for (int i = 0; i < 2; i++) {
		enearchObj_[i] = Object3d::Create();
		enearchObj_[i]->SetModel(enearchModel_);
	}

	//�U���e
	inductionModel_ = Model::LoadFromOBJ("boll");
	for (int i = 0; i < 2; i++) {
		inductionObj_[i] = Object3d::Create();
		inductionObj_[i]->SetModel(inductionModel_);
		inductionObj_[i]->wtf.scale = { 0.2f,0.2f,0.2f };
	}

	//���e�B�N��
	retModel_ = Model::LoadFromOBJ("retboll");
	for (int i = 0; i < 2; i++) {
		retObj_[i] = Object3d::Create();
		retObj_[i]->SetModel(retModel_);
	}

	//�p�[�e�B�N������
	DamageParticle = std::make_unique<ParticleManager>();
	DamageParticle.get()->Initialize();
	DamageParticle->LoadTexture("doge.png");
	DamageParticle->Update();

}

void Enemy::WinpUpdate()
{
	winpArrivalTimer++;
	//�G���G�̏o�����x
	float WinpSpeedX = 0.1f;
	float WinpSpeedZ = 1.0f;

	//�G���G�̏����ʒu(���E�F�[�u)
	// ���������l                  �E���ŏI���B�_
	//{ 10.0f, -1.0f, 10.0f };{ 0.0f,-1.0f,10.0f };
	fbxWinpObject3d_[0]->wtf.position = fbxWinpObject3d_[0]->wtf.position + enemyWinplocalpos0;
	//{ 10.0f, 0.0f, 10.0f };{ 3.0f,0.0f,10.0f }
	fbxWinpObject3d_[1]->wtf.position = fbxWinpObject3d_[1]->wtf.position + enemyWinplocalpos1;
	//{ -10.0f, 0.0f, 10.0f };{ -3.0f,0.0f,10.0f }
	fbxWinpObject3d_[2]->wtf.position = fbxWinpObject3d_[2]->wtf.position + enemyWinplocalpos2;
	//{ -10.0f, 1.0f, 10.0f };{ 0.0f,1.0f,10.0f 
	fbxWinpObject3d_[3]->wtf.position = fbxWinpObject3d_[3]->wtf.position + enemyWinplocalpos3;

	if (winpArrivalTimer >= 0 && winpArrivalTimer < 250) {
		//�o��
		enemyWinplocalpos0.x -= WinpSpeedX;
		enemyWinplocalpos1.x -= WinpSpeedX;
		enemyWinplocalpos2.x += WinpSpeedX;
		enemyWinplocalpos3.x += WinpSpeedX;
		if (enemyWinplocalpos0.x <= 0.0f) { enemyWinplocalpos0.x = 0.0f; }
		if (enemyWinplocalpos1.x <= 3.0f) { enemyWinplocalpos1.x = 3.0f; }
		if (enemyWinplocalpos2.x >= -3.0f) { enemyWinplocalpos2.x = -3.0f; }
		if (enemyWinplocalpos3.x >= 0.0f) { enemyWinplocalpos3.x = 0.0f; }
	}
	else if (winpArrivalTimer >= 250) {
		//����
		enemyWinplocalpos0.x += WinpSpeedX;
		enemyWinplocalpos1.x += WinpSpeedX;
		enemyWinplocalpos2.x -= WinpSpeedX;
		enemyWinplocalpos3.x -= WinpSpeedX;
		if (enemyWinplocalpos0.x >= 15.0f) { isWinpAliveFlag_[0] = 1; }
		if (enemyWinplocalpos1.x >= 15.0f) { isWinpAliveFlag_[1] = 1; }
		if (enemyWinplocalpos2.x <= -15.0f) { isWinpAliveFlag_[2] = 1; }
		if (enemyWinplocalpos3.x <= -15.0f) { isWinpAliveFlag_[3] = 1; }
	}

	//�G���G�̏����ʒu(���E�F�[�u)
	// ���������l                  �E���ŏI���B�_
	//{ 12.0f, 2.0f, 10.0f };{  2.0f, 2.0f,10.0f };
	fbxWinpObject3d_[4]->wtf.position = fbxWinpObject3d_[4]->wtf.position + enemyWinplocalpos4;
	//{ -12.0f, 2.0f, 10.0f };{  -2.0f, 2.0f,10.0f };
	fbxWinpObject3d_[5]->wtf.position = fbxWinpObject3d_[5]->wtf.position + enemyWinplocalpos5;
	//{ 12.0f, 2.0f, 10.0f };{  2.0f, -2.0f,10.0f };
	fbxWinpObject3d_[6]->wtf.position = fbxWinpObject3d_[6]->wtf.position + enemyWinplocalpos6;
	//{ -12.0f, 2.0f, 10.0f };{  -2.0f, -2.0f,10.0f };
	fbxWinpObject3d_[7]->wtf.position = fbxWinpObject3d_[7]->wtf.position + enemyWinplocalpos7;

	if (winpArrivalTimer >= 300 && winpArrivalTimer < 550) {
		//�o��
		enemyWinplocalpos4.x -= WinpSpeedX;
		enemyWinplocalpos5.x += WinpSpeedX;
		enemyWinplocalpos6.x -= WinpSpeedX;
		enemyWinplocalpos7.x += WinpSpeedX;

		if (enemyWinplocalpos4.x <= 2.0f) { enemyWinplocalpos4.x = 2.0f; }
		if (enemyWinplocalpos5.x >= -2.0f) { enemyWinplocalpos5.x = -2.0f; }
		if (enemyWinplocalpos6.x <= 2.0f) { enemyWinplocalpos6.x = 2.0f; }
		if (enemyWinplocalpos7.x >= -2.0f) { enemyWinplocalpos7.x = -2.0f; }
	}
	else if (winpArrivalTimer >= 550) {
		//����
		enemyWinplocalpos4.x += WinpSpeedX;
		enemyWinplocalpos5.x -= WinpSpeedX;
		enemyWinplocalpos6.x += WinpSpeedX;
		enemyWinplocalpos7.x -= WinpSpeedX;

		if (enemyWinplocalpos4.x >= 15.0f) { isWinpAliveFlag_[4] = 1; }
		if (enemyWinplocalpos5.x <= -15.0f) { isWinpAliveFlag_[5] = 1; }
		if (enemyWinplocalpos6.x >= 15.0f) { isWinpAliveFlag_[6] = 1; }
		if (enemyWinplocalpos7.x <= -15.0f) { isWinpAliveFlag_[7] = 1; }
	}

	//�G���G�̏����ʒu(��O�E�F�[�u)
	// ���������l                  �E���ŏI���B�_
	//{ 2.0f,0.0f,50.0f };//{ 2.0f,0.0f,10.0f }
	fbxWinpObject3d_[8]->wtf.position = fbxWinpObject3d_[8]->wtf.position + enemyWinplocalpos8;
	//{ -2.0f,0.0f,50.0f };//{ -2.0f,0.0f,10.0f }
	fbxWinpObject3d_[9]->wtf.position = fbxWinpObject3d_[9]->wtf.position + enemyWinplocalpos9;

	if (winpArrivalTimer == 550) { isWinpAliveFlag_[8] = 0; isWinpAliveFlag_[9] = 0; }
	if (winpArrivalTimer < 550) { isWinpAliveFlag_[8] = 1; isWinpAliveFlag_[9] = 1; }
	else if (winpArrivalTimer >= 550 && winpArrivalTimer < 880) {
		enemyWinplocalpos8.z -= WinpSpeedZ;
		enemyWinplocalpos9.z -= WinpSpeedZ;

		if (enemyWinplocalpos8.z <= 10.0f) { enemyWinplocalpos8.z = 10.0f; }
		if (enemyWinplocalpos9.z <= 10.0f) { enemyWinplocalpos9.z = 10.0f; }

		if (enemyWinplocalpos8.x >= 5.0f) { enemyWinpAction8 = 1; }
		else if (enemyWinplocalpos8.x <= -5.0f) { enemyWinpAction8 = 0; }

		if (enemyWinplocalpos9.x >= 5.0f) { enemyWinpAction9 = 1; }
		else if (enemyWinplocalpos9.x <= -5.0f) { enemyWinpAction9 = 0; }

		if (enemyWinplocalpos8.z == 10.0f) {
			if (enemyWinpAction8 == 0) {
				enemyWinplocalpos8.x += 0.02f;
			}
			else if (enemyWinpAction8 == 1) {
				enemyWinplocalpos8.x -= 0.02f;
			}
		}

		if (enemyWinplocalpos9.z == 10.0f) {
			if (enemyWinpAction9 == 0) {
				enemyWinplocalpos9.x += 0.02f;
			}
			else if (enemyWinpAction9 == 1) {
				enemyWinplocalpos9.x -= 0.02f;
			}
		}

	}
	else if (winpArrivalTimer >= 880) { isWinpAliveFlag_[8] = 1; isWinpAliveFlag_[9] = 1; }

	//�G���G�̏����ʒu(��l�E�F�[�u)
	// ���������l                  �E���ŏI���B�_
	//{ 8.0f, 2.0f,-7.0f };   { 8.0f, 2.0f,7.0f };
	fbxWinpObject3d_[10]->wtf.position = fbxWinpObject3d_[10]->wtf.position + enemyWinplocalpos10;
	//{ 8.0f, 0.0f,-7.0f };   { 8.0f, 2.0f,5.0f };
	fbxWinpObject3d_[11]->wtf.position = fbxWinpObject3d_[11]->wtf.position + enemyWinplocalpos11;
	//{ 8.0f, -2.0f,-7.0f };  { 8.0f, 2.0f,3.0f };
	fbxWinpObject3d_[12]->wtf.position = fbxWinpObject3d_[12]->wtf.position + enemyWinplocalpos12;

	if (winpArrivalTimer < 850) { isWinpAliveFlag_[10] = 1; isWinpAliveFlag_[11] = 1; isWinpAliveFlag_[12] = 1; }
	if (winpArrivalTimer == 850) { isWinpAliveFlag_[10] = 0; isWinpAliveFlag_[11] = 0; isWinpAliveFlag_[12] = 0; }
	if (winpArrivalTimer >= 850 && winpArrivalTimer < 1250) {
		//�o��
		enemyWinplocalpos10.z += WinpSpeedX;
		enemyWinplocalpos11.z += WinpSpeedX;
		enemyWinplocalpos12.z += WinpSpeedX;

		if (enemyWinplocalpos10.z >= 7.0f) { enemyWinplocalpos10.z = 7.0f; }
		if (enemyWinplocalpos11.z >= 5.0f) { enemyWinplocalpos11.z = 5.0f; }
		if (enemyWinplocalpos12.z >= 3.0f) { enemyWinplocalpos12.z = 3.0f; }
	}
	else if (winpArrivalTimer >= 1250) {
		//����
		enemyWinplocalpos10.y += WinpSpeedX;
		enemyWinplocalpos11.y += WinpSpeedX;
		enemyWinplocalpos12.y -= WinpSpeedX;

		if (enemyWinplocalpos10.y >= 7.0f) { isWinpAliveFlag_[10] = 1; }
		if (enemyWinplocalpos11.y >= 7.0f) { isWinpAliveFlag_[11] = 1; }
		if (enemyWinplocalpos12.y <= -7.0f) { isWinpAliveFlag_[12] = 1; }
	}



}

void Enemy::BossWinpUpdate()
{	//���e�B�N���̑��x
	float eneRetSpeedY = 0.02f;
	float eneRetSpeedX = 0.02f;

	if (isMoveAction == true) {
		for (int i = 0; i < 2; i++) {
			storeStBulletTime_[i]++;
		}
	}

	// ���������l                  �E���ŏI���B�_
	// { 0.0f,0.0f,15.0f };      { 2.0f, 2.0f, 10.0f };
	enearchObj_[0]->wtf.position = enearchObj_[0]->wtf.position + enearchlocalpos0;
	// { 0.0f,0.0f,15.0f };      { -2.0f, 2.0f, 10.0f };
	enearchObj_[1]->wtf.position = enearchObj_[1]->wtf.position + enearchlocalpos1;
	retObj_[0]->wtf.position = retObj_[0]->wtf.position + retlocalpos0;
	retObj_[1]->wtf.position = retObj_[1]->wtf.position + retlocalpos1;

	if (winpArrivalTimer < 1900) {
		isEnearchAliveFlag_[0] = 1;
		isEnearchAliveFlag_[1] = 1;
	}
	if (winpArrivalTimer == 1900) {
		isEnearchAliveFlag_[0] = 0;
		isEnearchAliveFlag_[1] = 0;
	}
	if (winpArrivalTimer >= 1900) {
		enearchlocalpos0.x += 0.01f;
		enearchlocalpos0.y += 0.01f;
		enearchlocalpos0.z -= 0.04f;

		enearchlocalpos1.x -= 0.01f;
		enearchlocalpos1.y += 0.01f;
		enearchlocalpos1.z -= 0.04f;

		if (enearchlocalpos0.x >= 2.0f) { enearchlocalpos0.x = 2.0f; }
		if (enearchlocalpos0.y >= 2.0f) { enearchlocalpos0.y = 2.0f; }
		if (enearchlocalpos0.z <= 10.0f) { enearchlocalpos0.z = 10.0f; isMoveAction = true; }

		if (enearchlocalpos1.x <= -2.0f) { enearchlocalpos1.x = -2.0f; }
		if (enearchlocalpos1.y >= 2.0f) { enearchlocalpos1.y = 2.0f; }
		if (enearchlocalpos1.z <= 10.0f) { enearchlocalpos1.z = 10.0f; }
	}

	//�{�X�̃t�@���l���̗U���e
	for (int i = 0; i < 2; i++) {

		if (storeStBulletTime_[i] >= 40 + i * 20) {
			if (isShootStFlag_[i] == false) {
				isShootStFlag_[i] = true;
			}
		}
		if (StBulletCoolTime_[i] >= 60.0f) {
			storeStBulletTime_[i] = 0;
			StBulletCoolTime_[i] = 0;
			isShootStFlag_[i] = false;
		}
	}


	float ShortStSpeed = 0.001f;
	if (isShootStFlag_[0] == true) {
		StBulletCoolTime_[0]++;
		inductionObj_[0]->wtf.position += playerlen0;
		len = playerlen0;
		len *= ShortStSpeed;
	}
	else { inductionObj_[0]->wtf.position = { enearchObj_[0]->wtf.position.x + 6.0f,enearchObj_[0]->wtf.position.y + 7.0f, enearchObj_[0]->wtf.position.z + 30.0f }; }

	if (isShootStFlag_[1] == true) {
		StBulletCoolTime_[1]++;
		inductionObj_[1]->wtf.position += playerlen1;
		len1 = playerlen1;
		len1 *= ShortStSpeed;
	}
	else { inductionObj_[1]->wtf.position = { enearchObj_[1]->wtf.position.x - 5.0f,enearchObj_[1]->wtf.position.y + 7.0f, enearchObj_[1]->wtf.position.z + 30.0f }; }




	//�ړ�(���e�B�N��)
	if (splineTimer >= 100) {
		if (input_->PushKey(DIK_W) || input_->StickInput(L_UP)) {
			retlocalpos0.y += eneRetSpeedY;
			retlocalpos1.y += eneRetSpeedY;
			
		}
		if (input_->PushKey(DIK_S) || input_->StickInput(L_DOWN)) {
			retlocalpos0.y -= eneRetSpeedY;
			retlocalpos1.y -= eneRetSpeedY;
		}
		if (input_->PushKey(DIK_A) || input_->StickInput(L_LEFT)) {
			retlocalpos0.x -= eneRetSpeedX;
			retlocalpos1.x -= eneRetSpeedX;
		}
		if (input_->PushKey(DIK_D) || input_->StickInput(L_RIGHT)) {
			retlocalpos0.x += eneRetSpeedX;
			retlocalpos1.x += eneRetSpeedX;
		}
	}
}

void Enemy::Update(SplinePosition* spPosition_)
{
	splineTimer++;
	splinePosition_ = spPosition_;
	camera->Update();
	//�ŏ��̃{�X�̏����ĎG���G���o�Ă���܂ł̋���
	fbxObject3d_->Update();
	shootObj_->Update();
	shootObj2_->Update();


	if (bossGostMove == 0) { fbxObject3d_->wtf.position.y -= 0.003f; }
	if (fbxObject3d_->wtf.position.y <= -0.1f) { bossGostMove = 1; }
	if (bossGostMove == 1) { fbxObject3d_->wtf.position.z += 0.08f; }
	if (fbxObject3d_->wtf.position.z >= 6.0f) { bossGostAt = true; }
	if (fbxObject3d_->wtf.position.z >= 12.0f) { fbxObject3d_->wtf.position.z = 10000.0f; }
	//�G���G���܂Ƃ߂čX�V
	for (int i = 0; i < 13; i++) { if (isWinpAliveFlag_[i] == 0) { fbxWinpObject3d_[i]->Update(); } }
	//�{�X�̃t�@���l���܂Ƃ߂čX�V
	for (int i = 0; i < 2; i++) {
		enearchObj_[i]->Update();
		inductionObj_[i]->Update();
	}

	for (int i = 0; i < 2; i++) { retObj_[i]->Update(); }
	//�{�X�̃t�@���l���̃x�N�g�����K��
	if (storeStBulletTime_[0] < 40) {
		playerlen0 = retObj_[0]->wtf.position - inductionObj_[0]->wtf.position;
		playerlen0.nomalize();
	}
	if (storeStBulletTime_[1] < 40) {
		playerlen1 = retObj_[1]->wtf.position - inductionObj_[1]->wtf.position;
		playerlen1.nomalize();
	}
	EffUpdate();

	//�G���G�������n�߂�
	if (bossGostAt == true) {

		for (int i = 0; i < 2; i++) {
			isShootCoolTimer_[i]++;
		}

		//�X�v���C���Ȑ��̍X�V
		float speed = 0.0f;
		splinePosition_->Update(speed);
		for (int i = 0; i < 2; i++) { retObj_[i]->wtf.position = splinePosition_->NowPos; }
		camera->wtf.position = splinePosition_->NowPos;
		//�G���G���܂Ƃ߂čX�V(�X�v���C���Ȑ�)
		for (int i = 0; i < 13; i++) { fbxWinpObject3d_[i]->wtf.position = splinePosition_->NowPos; }
		//�{�X�̃t�@���l�����܂Ƃ߂čX�V(�X�v���C���Ȑ�)
		for (int i = 0; i < 2; i++) { enearchObj_[i]->wtf.position = splinePosition_->NowPos; }



		//�G���G�̔����ƈړ�
		WinpUpdate();
		//�{�X�̃t�@���l���̋���
		BossWinpUpdate();

		//�G���G�̍U��
		if (isShootCoolTimer_[0] >= 30) {
			isShootFlag_[0] = 1;
		}
		if (isShootFlag_[0] == 1) {
			isShootexistTimer_[0]++;
			shootObj_->wtf.position.z -= 0.1f;
		}
		else {
			shootObj_->wtf.position = { fbxWinpObject3d_[8]->wtf.position.x,fbxWinpObject3d_[8]->wtf.position.y, fbxWinpObject3d_[8]->wtf.position.z };
		}
		if (isShootexistTimer_[0] >= 70) {
			isShootFlag_[0] = 0;
			isShootCoolTimer_[0] = 0;
			isShootexistTimer_[0] = 0;
		}

		//����̍􂠂Ƃł����ƒ���
		if (isShootCoolTimer_[1] >= 30) {
			isShootFlag_[1] = 1;
		}
		if (isShootFlag_[1] == 1) {
			isShootexistTimer_[1]++;
			shootObj2_->wtf.position.z -= 0.1f;
		}
		else {
			shootObj2_->wtf.position = { fbxWinpObject3d_[9]->wtf.position.x,fbxWinpObject3d_[9]->wtf.position.y, fbxWinpObject3d_[9]->wtf.position.z };
		}
		if (isShootexistTimer_[1] >= 70) {
			isShootFlag_[1] = 0;
			isShootCoolTimer_[1] = 0;
			isShootexistTimer_[1] = 0;
		}






	}



	//�����蔻��(���@�e(��)�ƎG���G)
	for (int i = 0; i < 13; i++) {
		if (isWinpAliveFlag_[i] == 0) {
			if (coll.CircleCollision(player_->GetBulletWorldPosition(), fbxWinpObject3d_[i]->wtf.position, 1.0f, 0.2f)) {
				isEffFlag_[i] = 1;
				isWinpAliveFlag_[i] = 1;
				player_->bulletUpCount++;
			};
		}
	}

	//�����蔻��(���@�e(��)�ƎG���G)
	for (int i = 0; i < 13; i++) {
		if (isWinpAliveFlag_[i] == 0) {
			if (coll.CircleCollision(player_->GetBulletStWorldPosition(), fbxWinpObject3d_[i]->wtf.position, 1.0f, 0.2f)) {
				isEffFlag_[i] = 1;
				isWinpAliveFlag_[i] = 1;
				player_->bulletUpCount++;
			};
		}
	}

	//�����蔻��(���@�e(��)�ƃ{�X�̃t�@���l��)
	/*for (int i = 0; i < 2; i++) {
		if (isEnearchAliveFlag_[i] == 0) {
			if (coll.CircleCollision(player_->GetBulletWorldPosition(), enearchObj_[i]->wtf.position, 0.3f, 0.3f)) {
				isEffFlag_[i] = 1;
				isEnearchAliveFlag_[i] = 1;
			};
		}
	}*/


	//�G����萔�|���ƃv���C���[�̒e��������
	if (player_->bulletUpCount == 2) {
		player_->bulletMax++;
		player_->bulletUpCount = 0;
	}

	ImGui::Begin("Enemy");

	ImGui::Text("winpArrivalTimer:%d", winpArrivalTimer);
	ImGui::Text("retPosition0:%f,%f,%f", retObj_[0]->wtf.position.x, retObj_[0]->wtf.position.y, retObj_[0]->wtf.position.z);
	ImGui::Text("retPosition1:%f,%f,%f", retObj_[1]->wtf.position.x, retObj_[1]->wtf.position.y, retObj_[1]->wtf.position.z);

	ImGui::End();





}



void Enemy::Draw()
{
	if (bossGostAt == true) {
		for (int i = 0; i < 2; i++) {
			if (isEnearchAliveFlag_[i] == 0) {
				enearchObj_[i]->Draw();
			}
			retObj_[i]->Draw();
			if (isShootStFlag_[i] == true) {
				if (isEnearchAliveFlag_[i] == 0) {
					inductionObj_[i]->Draw();
				}
			}
		}

	}

	if (isWinpAliveFlag_[8] == 0) {
		if (isShootFlag_[0] == 1 && isOffsetFlag_ == 0) {
			shootObj_->Draw();
		}
	}
	if (isWinpAliveFlag_[9] == 0 && isOffsetFlag_ == 0) {
		if (isShootFlag_[1] == 1) {
			shootObj2_->Draw();
		}
	}

}

void Enemy::FbxDraw()
{

	fbxObject3d_->Draw(dxCommon->GetCommandList());


	if (bossGostAt == true) {
		for (int i = 0; i < 13; i++) {
			if (isWinpAliveFlag_[i] == 0) {
				fbxWinpObject3d_[i]->Draw(dxCommon->GetCommandList());
			}
		}
	}



}

void Enemy::EffUpdate()
{
	for (int i = 0; i < 13; i++) {
		if (isEffFlag_[i] == 1) {
			EffTimer_[i]++;
		}
		if (EffTimer_[i] <= 10 && EffTimer_[i] >= 1) {
			EffSummary(Vector3(fbxWinpObject3d_[i]->wtf.position.x, fbxWinpObject3d_[i]->wtf.position.y, fbxWinpObject3d_[i]->wtf.position.z));
		}
		if (EffTimer_[i] >= 10) {
			isEffFlag_[i] = 0;
			EffTimer_[i] = 0;
		}
	}

}

void Enemy::EffSummary(Vector3 enemyPos)
{
	//�p�[�e�B�N���͈�
	for (int i = 0; i < 5; i++) {
		//X,Y,Z�S��[-5.0f,+5.0f]�Ń����_���ɕ��z
		const float rnd_pos = 5.0f;
		Vector3 pos{};
		pos.x += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos += enemyPos;
		//���x
		//X,Y,Z�S��[-0.05f,+0.05f]�Ń����_���ɕ��z
		const float rnd_vel = 0.1f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		//�d�͂Ɍ����Ă�Y�̂�[-0.001f,0]�Ń����_���ɕ��z
		const float rnd_acc = 0.00001f;
		Vector3 acc{};
		acc.x = (float)rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;
		acc.y = (float)rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;

		//�ǉ�
		DamageParticle->Add(60, pos, vel, acc, 0.3f, 0.0f);

		DamageParticle->Update();

	}

}

void Enemy::EffSimpleSummary()
{
	//�p�[�e�B�N���͈�
	for (int i = 0; i < 5; i++) {
		//X,Y,Z�S��[-5.0f,+5.0f]�Ń����_���ɕ��z
		const float rnd_pos = 5.0f;
		Vector3 pos{};
		pos.x += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		//���x
		//X,Y,Z�S��[-0.05f,+0.05f]�Ń����_���ɕ��z
		const float rnd_vel = 0.1f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		//�d�͂Ɍ����Ă�Y�̂�[-0.001f,0]�Ń����_���ɕ��z
		const float rnd_acc = 0.00001f;
		Vector3 acc{};
		acc.x = (float)rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;
		acc.y = (float)rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;

		//�ǉ�
		DamageParticle->Add(60, pos, vel, acc, 1.0f, 0.0f);

		DamageParticle->Update();

	}

}

void Enemy::EffDraw()
{
	for (int i = 0; i < 13; i++) {
		if (isEffFlag_[i] == 1) {
			// 3D�I�u�N�W�F�N�g�̕`��
			DamageParticle->Draw();
		}
	}
}

Vector3 Enemy::GetWorldPosition()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	fbxObject3d_->wtf.UpdateMat();
	//���[���h�s��̕��s�ړ�����
	worldPos.x = fbxObject3d_->wtf.matWorld.m[3][0];
	worldPos.y = fbxObject3d_->wtf.matWorld.m[3][1];
	worldPos.z = fbxObject3d_->wtf.matWorld.m[3][2];

	return worldPos;
}

Vector3 Enemy::GetWinpWorldPosition(int num)
{
	//���[���h���W������ϐ�
	Vector3 WinpWorldPos;
	fbxWinpObject3d_[num]->wtf.UpdateMat();
	//���[���h�s��̕��s�ړ�����
	WinpWorldPos.x = fbxWinpObject3d_[num]->wtf.matWorld.m[3][0];
	WinpWorldPos.y = fbxWinpObject3d_[num]->wtf.matWorld.m[3][1];
	WinpWorldPos.z = fbxWinpObject3d_[num]->wtf.matWorld.m[3][2];

	return WinpWorldPos;
}

Vector3 Enemy::GetEnearchWorldPosition(int num)
{
	//���[���h���W������ϐ�
	Vector3 enearchWorldPos;
	enearchObj_[num]->wtf.UpdateMat();
	//���[���h�s��̕��s�ړ�����
	enearchWorldPos.x = enearchObj_[num]->wtf.matWorld.m[3][0];
	enearchWorldPos.y = enearchObj_[num]->wtf.matWorld.m[3][1];
	enearchWorldPos.z = enearchObj_[num]->wtf.matWorld.m[3][2];

	return enearchWorldPos;
}

Vector3 Enemy::GetinductionWorldPosition(int num)
{
	//���[���h���W������ϐ�
	Vector3 inductionWorldPos;
	inductionObj_[num]->wtf.UpdateMat();
	//���[���h�s��̕��s�ړ�����
	inductionWorldPos.x = inductionObj_[num]->wtf.matWorld.m[3][0];
	inductionWorldPos.y = inductionObj_[num]->wtf.matWorld.m[3][1];
	inductionWorldPos.z = inductionObj_[num]->wtf.matWorld.m[3][2];

	return inductionWorldPos;
}

void Enemy::OnColision()
{
	isAliveFlag = false;
}

