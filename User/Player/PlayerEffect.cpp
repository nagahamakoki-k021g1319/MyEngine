#include "PlayerEffect.h"

void PlayerEffect::Initialize()
{
	ParticleSetting set;

	//弾
	set.life = 60;
	set.pos = { 0.03f ,0.03f ,0.03f };
	set.vel = { 0.01f ,0.01f ,0.01f };
	set.num = 5;
	set.acc = 0.000001f;
	set.scale = { 0.5f,0.0f };

	for (int i = 0; i < 3; i++)
	{
		ballisticParticles_[i] = std::make_unique<BasicParticle>();
		ballisticParticles_[i]->Initialize("bulletchage.png", set);
	}
}

void PlayerEffect::BallisticUpdate()
{
	if (isBallisticEffFlag_ == 1)
	{
		ballisticEffTimer_++;
	}
	if (ballisticEffTimer_ <= 10 && ballisticEffTimer_ >= 0)
	{
		for (size_t i = 0; i < 3; i++)
		{
			ballisticParticles_[i]->Add(ballisticParticlesPos_[i]);
		}
	}
	if (ballisticEffTimer_ >= 10)
	{
		isBallisticEffFlag_ = 0;
		ballisticEffTimer_ = 0;
	}
}

void PlayerEffect::SetBallisticPoss(const std::array<Vector3, 3>& ballisticParticlesPos)
{
	ballisticParticlesPos_ = ballisticParticlesPos;
}

void PlayerEffect::BallisticEmit()
{
	isBallisticEffFlag_ = true;
}

void PlayerEffect::BallisticStop()
{
	isBallisticEffFlag_ = false;
}

void PlayerEffect::SetBulletCount(int BulletCount)
{
	bulletCount = BulletCount;
}

void PlayerEffect::Update()
{
	//弾
	BallisticUpdate();
}

void PlayerEffect::Draw()
{
	if (isBallisticEffFlag_ == 1)
	{
		ballisticParticles_[0]->Draw();
		if (bulletCount >= 2)
		{
			ballisticParticles_[1]->Draw();
		}
		if (bulletCount >= 3)
		{
			ballisticParticles_[2]->Draw();
		}
	}
}
