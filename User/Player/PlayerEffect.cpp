#include "PlayerEffect.h"

void PlayerEffect::Initialize()
{
	//弾
	set.life = 60;
	set.pos = { 0.03f ,0.03f ,0.03f };
	set.vel = { 0.01f ,0.01f ,0.01f };
	set.num = 5;
	set.acc = 0.000001f;
	set.scale = { 0.5f,0.0f };

	for (int i = 0; i < 3; i++){
		ballisticParticles_[i] = std::make_unique<PlayerBasicParticle>();
		ballisticParticles_[i]->Initialize("bulletchage.png", set);
	}

	//左スピンエフェクト
	LSpinParticle = std::make_unique<PlayerBasicParticle>();
	set.life = 60;
	set.pos = { 0.03f ,0.03f ,0.03f };
	set.vel = { 0.03f ,0.03f ,0.03f };
	set.num = 5;
	set.acc = 0.000001f;
	set.scale = { 0.5f,0.0f };
	LSpinParticle->Initialize("fire.png",set);

}

void PlayerEffect::summary()
{
	//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
	const float rnd_posG = 0.0f;
	const float rnd_posGy = 0.0f;
	const float rnd_posGz = 0.0f;
	posG.x += ( float ) rand() / RAND_MAX * rnd_posG - rnd_posG / 2.0f;
	posG.y += ( float ) rand() / RAND_MAX * rnd_posGy - rnd_posGy / 2.0f;
	posG.z += ( float ) rand() / RAND_MAX * rnd_posGz - rnd_posGz / 2.0f;
	//速度
	//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
	const float rnd_velG = 0.0f;
	const float rnd_velGy = 0.0f;
	const float rnd_velGz = 0.02f;
	velG.x = ( float ) rand() / RAND_MAX * rnd_velG - rnd_velG / 2.0f;
	velG.y = ( float ) rand() / RAND_MAX * rnd_velGy - rnd_velGy / 2.0f;
	velG.z = ( float ) rand() / RAND_MAX * rnd_velGz - rnd_velGz / 0.2f;
	//重力に見立ててYのみ[-0.001f,0]でランダムに分布
	const float rnd_accG = 0.000001f;
	accG.x = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;
	accG.y = ( float ) rand() / RAND_MAX * rnd_accG - rnd_accG / 2.0f;

}

void PlayerEffect::BallisticUpdate()
{
	if (isBallisticEffFlag_ == true)
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
		isBallisticEffFlag_ = false;
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

void PlayerEffect::LSpinUpdate(Vector3 pos)
{
	if ( isLSpinEffFlag_ == 1 )
	{
		LSpinEffTimer_++;
	}
	if ( LSpinEffTimer_ <= 10 && LSpinEffTimer_ >= 0 )
	{
		LSpinParticle->Add(pos);
	}
	if ( LSpinEffTimer_ >= 10 )
	{
		isLSpinEffFlag_ = 0;
		LSpinEffTimer_ = 0;
	}
}

void PlayerEffect::SetLSpinPos(const Vector3& pos)
{
	LSpinPos = pos;
}

void PlayerEffect::LSpinEmit()
{
	isLSpinEffFlag_ = true;
}

void PlayerEffect::LSpinStop()
{
	isLSpinEffFlag_ = false;
}

void PlayerEffect::Update()
{
	//弾
	BallisticUpdate();
}

void PlayerEffect::Draw()
{
	if (isBallisticEffFlag_ == true)
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
	//左スピン
	if ( isLSpinEffFlag_ == 1 && LSpinEffTimer_ <= 100 && LSpinEffTimer_ >= 1 )
	{
		LSpinParticle->Draw();
	}
	
}
