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

	//右スピンエフェクト
	RSpinParticle = std::make_unique<PlayerBasicParticle>();
	RSpinParticle->Initialize("fire.png",set);

	//通常ガス
	for ( size_t i = 0; i < 2; i++ )
	{
		gasParticles[ i ] = std::make_unique<PlayerBasicParticle>();
		set.life = 60;
		set.pos = { 0.0f ,0.0f ,0.0f };
		set.vel = { 0.0f ,0.0f ,0.02f };
		set.num = 20;
		set.acc = 0.000001f;
		set.scale = { 0.05f,0.0f };
		gasParticles[ i ]->Initialize("gas.png",set);
	}

	//ガス(加速)
	gasParticleAccelR = std::make_unique<PlayerBasicParticle>();
	set.life = 60;
	set.pos = { 0.0f ,0.0f ,0.0f };
	set.vel = { 0.0f ,0.0f ,0.02f };
	set.num = 50;
	set.acc = 0.000001f;
	set.scale = { 0.07f,0.0f };
	gasParticleAccelR->Initialize("gas1.png",set);

	gasParticleAccelL = std::make_unique<PlayerBasicParticle>();
	gasParticleAccelL->Initialize("gas1.png",set);

	gasParticleDecelR = std::make_unique<PlayerBasicParticle>();
	set.life = 60;
	set.pos = { 0.0f ,0.0f ,0.0f };
	set.vel = { 0.0f ,0.0f ,0.05f };
	set.num = 5;
	set.acc = 0.000001f;
	set.scale = { 0.05f,0.0f };
	gasParticleDecelR->Initialize("gas.png",set);

	gasParticleDecelL = std::make_unique<PlayerBasicParticle>();
	gasParticleDecelL->Initialize("gas.png",set);


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

void PlayerEffect::LSpinUpdate()
{
	if ( isLSpinEffFlag_ )
	{
		LSpinEffTimer_++;
	}
	if ( LSpinEffTimer_ <= 10 && LSpinEffTimer_ >= 0 )
	{
		LSpinParticle->Add(LSpinPos);
	}
	if ( LSpinEffTimer_ >= 10 )
	{
		isLSpinEffFlag_ = false;
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

void PlayerEffect::RSpinUpdate()
{
	if ( isRSpinEffFlag_ == 1)
	{
		RSpinEffTimer_++;
	}
	if ( RSpinEffTimer_ <= 10 && RSpinEffTimer_ >= 0 )
	{
		RSpinParticle->Add(LSpinPos);
	}
	if ( RSpinEffTimer_ >= 10 )
	{
		isRSpinEffFlag_ = false;
		RSpinEffTimer_ = 0;
	}
}

void PlayerEffect::SetRSpinPos(const Vector3& pos)
{
	RSpinPos = pos;
}

void PlayerEffect::RSpinEmit()
{
	isRSpinEffFlag_ = true;
}

void PlayerEffect::RSpinStop()
{
	isRSpinEffFlag_ = false;
}

void PlayerEffect::GasParticleUpdate()
{
	//バイクのエンジン
	if ( isbulletEffFlag_ == 1)
	{
		bulletEffTimer_++;
	}
	if ( bulletEffTimer_ <= 20 && bulletEffTimer_ >= 1 )
	{
		for ( size_t i = 0; i < 2; i++ )
		{
			gasParticles[ i ]->Add(gasParticlePoss[ 0 ]);
		}

		gasParticleAccelL->Add(gasParticleAccelPoss[ 0 ]);
		gasParticleAccelR->Add(gasParticleAccelPoss[ 1 ]);

		gasParticleDecelL->Add(gasParticleDecelPoss[ 0 ]);
		gasParticleDecelL->Add(gasParticleDecelPoss[ 1 ]);
	}
	if ( bulletEffTimer_ >= 20 )
	{
		isbulletEffFlag_ = 0;
		bulletEffTimer_ = 0;
	}
}

void PlayerEffect::SetLGasParticlePos(const Vector3& pos)
{
	gasParticlePoss[ 0 ] = pos;
}

void PlayerEffect::SetRGasParticlePos(const Vector3& pos)
{
	gasParticlePoss[ 1 ] = pos;
}

void PlayerEffect::SetLGasParticleAccelPos(const Vector3& pos)
{
	gasParticleAccelPoss[ 0 ] = pos;
}

void PlayerEffect::SetRGasParticleAccelPos(const Vector3& pos)
{
	gasParticleAccelPoss[ 1 ] = pos;
}

void PlayerEffect::SetLGasParticleDecelPos(const Vector3& pos)
{
	gasParticleDecelPoss[ 0 ] = pos;
}

void PlayerEffect::SetRGasParticleDecelPos(const Vector3& pos)
{
	gasParticleDecelPoss[ 1 ] = pos;
}

void PlayerEffect::GasParticleEmit()
{
	isbulletEffFlag_ = true;
}

void PlayerEffect::GasParticleStop()
{
	isbulletEffFlag_ = false;
	bulletEffTimer_ = 0;
}

void PlayerEffect::SetNormal()
{
	isboostFlag = NORMAL;
}

void PlayerEffect::SetAcceleration()
{
	isboostFlag = ACCELERATION;
}

void PlayerEffect::SetDeceleration()
{
	isboostFlag = DECELERATION;
}

void PlayerEffect::Update()
{
	//弾
	BallisticUpdate();
	//左スピン
	LSpinUpdate();

	//右スピン
	RSpinUpdate();

	//エフェクトの情報(通常ガス右)
	//エフェクトの情報(通常ガス左)
	GasParticleUpdate();

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

	//左スピン
	if ( isLSpinEffFlag_ && LSpinEffTimer_ <= 10 && LSpinEffTimer_ >= 1 )
	{
		LSpinParticle->Draw();
	}

	//右スピンエフェクト
	if ( isRSpinEffFlag_ && RSpinEffTimer_ <= 10 && RSpinEffTimer_ >= 1 )
	{
		RSpinParticle->Draw();
	}

	if ( isbulletEffFlag_ == 1 )
	{
		if ( isboostFlag == 0 )
		{
			for ( size_t i = 0; i < 2; i++ )
			{
				gasParticles[ i ]->Draw();
			}
		}
		else if ( isboostFlag == 1 )
		{
			gasParticleAccelR->Draw();
			gasParticleAccelL->Draw();
		}
		else if ( isboostFlag == 2 )
		{
			/*gasParticleDecelR->Draw();
			gasParticleDecelL->Draw();*/
		}
	}

}
