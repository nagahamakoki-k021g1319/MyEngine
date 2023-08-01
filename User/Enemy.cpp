#include "Enemy.h"
#include "Player.h"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
	//FBX�I�u�W�F�N�g���
	delete fbxObject3d_;
	delete fbxModel_;
	for (int i = 0; i < 10; i++) { delete fbxWinpObject3d_[i]; }
	delete fbxWinpModel_;
	delete shootModel_;
	delete shootObj_;

	delete shootModel2_;
	delete shootObj2_;

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
	for (int i = 0; i < 10; i++) {
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
	shootObj_->wtf.scale = { 0.4f,0.4f,0.4f };

	shootModel2_ = Model::LoadFromOBJ("eneboll");
	shootObj2_ = Object3d::Create();
	shootObj2_->SetModel(shootModel2_);
	shootObj2_->wtf.position = { fbxWinpObject3d_[9]->wtf.position.x,fbxWinpObject3d_[9]->wtf.position.y, fbxWinpObject3d_[9]->wtf.position.z };
	shootObj2_->wtf.scale = { 0.4f,0.4f,0.4f };

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

	enemyWinplocalpos0.x -= WinpSpeedX;
	enemyWinplocalpos1.x -= WinpSpeedX;
	enemyWinplocalpos2.x += WinpSpeedX;
	enemyWinplocalpos3.x += WinpSpeedX;
	if (enemyWinplocalpos0.x <= 0.0f) { enemyWinplocalpos0.x = 0.0f; }
	if (enemyWinplocalpos1.x <= 3.0f) { enemyWinplocalpos1.x = 3.0f; }
	if (enemyWinplocalpos2.x >= -3.0f) { enemyWinplocalpos2.x = -3.0f; }
	if (enemyWinplocalpos3.x >= 0.0f) { enemyWinplocalpos3.x = 0.0f; }

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

	if (winpArrivalTimer >= 300) {
		enemyWinplocalpos4.x -= WinpSpeedX;
		enemyWinplocalpos5.x += WinpSpeedX;
		enemyWinplocalpos6.x -= WinpSpeedX;
		enemyWinplocalpos7.x += WinpSpeedX;

		if (enemyWinplocalpos4.x <= 2.0f) { enemyWinplocalpos4.x = 2.0f; }
		if (enemyWinplocalpos5.x >= -2.0f) { enemyWinplocalpos5.x = -2.0f; }
		if (enemyWinplocalpos6.x <= 2.0f) { enemyWinplocalpos6.x = 2.0f; }
		if (enemyWinplocalpos7.x >= -2.0f) { enemyWinplocalpos7.x = -2.0f; }

	}

	//�G���G�̏����ʒu(��O�E�F�[�u)
	// ���������l                  �E���ŏI���B�_
	//{ 2.0f,0.0f,50.0f };//{ 2.0f,0.0f,10.0f }
	fbxWinpObject3d_[8]->wtf.position = fbxWinpObject3d_[8]->wtf.position + enemyWinplocalpos8;
	//{ -2.0f,0.0f,50.0f };//{ -2.0f,0.0f,10.0f }
	fbxWinpObject3d_[9]->wtf.position = fbxWinpObject3d_[9]->wtf.position + enemyWinplocalpos9;
	if (winpArrivalTimer >= 400) {
		enemyWinplocalpos8.z -= WinpSpeedZ;
		enemyWinplocalpos9.z -= WinpSpeedZ;

		if (enemyWinplocalpos8.z <= 10.0f) { enemyWinplocalpos8.z = 10.0f; }
		if (enemyWinplocalpos9.z <= 10.0f) { enemyWinplocalpos9.z = 10.0f; }
	}


}

void Enemy::Update(SplinePosition* spPosition_)
{
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
	for (int i = 0; i < 10; i++) {
		fbxWinpObject3d_[i]->Update();
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
		camera->wtf.position = splinePosition_->NowPos;
		for (int i = 0; i < 10; i++) {
			fbxWinpObject3d_[i]->wtf.position = splinePosition_->NowPos;
		}

		//�G���G�̔����ƈړ�
		WinpUpdate();

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
		if (isShootexistTimer_[0] >= 50) {
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
		if (isShootexistTimer_[1] >= 50) {
			isShootFlag_[1] = 0;
			isShootCoolTimer_[1] = 0;
			isShootexistTimer_[1] = 0;
		}


	}



	//�����蔻��(���@�e�ƎG���G)
	for (int i = 0; i < 10; i++) {
		if (isWinpAliveFlag_[i] == 0) {
			if (coll.CircleCollision(player_->GetBulletWorldPosition(), fbxWinpObject3d_[i]->wtf.position, 1.0f, 0.2f)) {
				isEffFlag_[i] = 1;
				isWinpAliveFlag_[i] = 1;
			};
		}
	}

	if (coll.CircleCollision(player_->GetBulletStWorldPosition(), shootObj_->wtf.position, 1.0f, 0.2f)) {
	
	};


}



void Enemy::Draw()
{
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
		for (int i = 0; i < 10; i++) {
			if (isWinpAliveFlag_[i] == 0) {
				fbxWinpObject3d_[i]->Draw(dxCommon->GetCommandList());
			}
		}
	}



}

void Enemy::EffUpdate()
{
	for (int i = 0; i < 10; i++) {
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
	for (int i = 0; i < 10; i++) {
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

Vector3 Enemy::GetWinpWorldPosition()
{
	//���[���h���W������ϐ�
	Vector3 WinpWorldPos;
	fbxWinpObject3d_[1]->wtf.UpdateMat();
	//���[���h�s��̕��s�ړ�����
	WinpWorldPos.x = fbxWinpObject3d_[1]->wtf.matWorld.m[3][0];
	WinpWorldPos.y = fbxWinpObject3d_[1]->wtf.matWorld.m[3][1];
	WinpWorldPos.z = fbxWinpObject3d_[1]->wtf.matWorld.m[3][2];

	return WinpWorldPos;
}

void Enemy::OnColision()
{
	isAliveFlag = false;
}

