#pragma once
#include "ParticleManager.h"
#include "PlayerBasicParticle.h"

//enum PlayerBoostFlag
//{
//	NORMAL,
//	ACCELERATION,
//	DECELERATION
//};

class PlayerEffect
{
public:
	PlayerEffect() = default;
	~PlayerEffect() = default;


	void Initialize();

	void summary();

	//弾飛ばし
	void BallisticUpdate();
	void SetBallisticPoss(const std::array< Vector3,3>& ballisticParticlesPos);
	void BallisticEmit();
	void BallisticStop();
	void SetBulletCount(int BulletCount);

	//左スピン攻撃
	void LSpinUpdate(Vector3 pos);
	void SetLSpinPos(const Vector3& pos);
	void LSpinEmit();
	void LSpinStop();
	////右スピン攻撃
	//void RSpinUpdate();
	//void SetRSpinPos(const Vector3& pos);
	//void RSpinEmit();
	//void RSpinStop();

	//void GasParticleUpdate();
	////通常ガス左
	//void SetLGasParticlePos(const Vector3& pos);
	////通常ガス右
	//void SetRGasParticlePos(const Vector3& pos);
	////加速ガス左
	//void SetLGasParticleAccelPos(const Vector3& pos);
	////加速ガス右
	//void SetRGasParticleAccelPos(const Vector3& pos);
	////減速ガス左
	//void SetLGasParticleDecelPos(const Vector3& pos);
	////減速ガス右
	//void SetRGasParticleDecelPos(const Vector3& pos);

	//void GasParticleEmit();
	//void GasParticleStop();

	//void SetNormal();
	//void SetAcceleration();
	//void SetDeceleration();


	void Update();
	void Draw();


	int isLSpinEffFlag_ = 0;
	int LSpinEffTimer_ = 0;
private:

	ParticleSetting set;

	//弾飛ばし
	std::array< std::unique_ptr<PlayerBasicParticle>,3> ballisticParticles_;
	std::array<Vector3,3> ballisticParticlesPos_;
	int ballisticEffTimer_ = 0;
	bool isBallisticEffFlag_ = false;
	int bulletCount = 1;

	//左スピンエフェクト
	std::unique_ptr<PlayerBasicParticle> LSpinParticle;
	Vector3 LSpinPos;
	

	////左スピンエフェクト
	//std::unique_ptr<PlayerBasicParticle> RSpinParticle;
	//Vector3 RSpinPos;
	//int RSpinEffTimer_ = 0;
	//bool isRSpinEffFlag_ = false;

	////ガス(通常)
	//std::array< std::unique_ptr<PlayerBasicParticle>,2> gasParticles;
	//std::array< Vector3,2>gasParticlePoss;
	////ガス(加速)
	//std::unique_ptr<PlayerBasicParticle> gasParticleAccelR;
	//std::unique_ptr<PlayerBasicParticle> gasParticleAccelL;
	//std::array< Vector3,2>gasParticleAccelPoss;
	////ガス(減速)
	//std::unique_ptr<PlayerBasicParticle> gasParticleDecelR;
	//std::unique_ptr<PlayerBasicParticle> gasParticleDecelL;
	//std::array< Vector3,2>gasParticleDecelPoss;
	//int bulletEffTimer_ = 0;
	//bool isbulletEffFlag_ = false;
	////自機のバイクのブーストフラグ(0 通常,1 加速,2 減速)
	//PlayerBoostFlag isboostFlag = NORMAL;

public:
	Vector3 posG{};
	Vector3 velG{};
	Vector3 accG{};
};

