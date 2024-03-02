#pragma once
#include "ParticleManager.h"
#include "PlayerBasicParticle.h"

enum PlayerBoostFlag
{
	NORMAL,
	ACCELERATION,
	DECELERATION
};

class PlayerEffect
{
private:
	//弾飛ばし
	std::array< std::unique_ptr<BasicParticle>,3> ballisticParticles_;
	std::array<Vector3,3> ballisticParticlesPos_;
	int ballisticEffTimer_ = 0;
	bool isBallisticEffFlag_ = 0;
	int bulletCount = 0;

public:

	void Initialize();

	void BallisticUpdate();
	void SetBallisticPoss(const std::array< Vector3,3>& ballisticParticlesPos);
	void BallisticEmit();
	void BallisticStop();

	void SetBulletCount(int BulletCount);

	void Update();
	void Draw();

	PlayerEffect() = default;
	~PlayerEffect() = default;
};

