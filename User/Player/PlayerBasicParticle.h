#pragma once
#include "ParticleManager.h"
#include"Vector2.h"

struct ParticleSetting
{
	Vector3 pos;
	Vector3 vel;
	float acc;
	int life;
	Vector2 scale;
	int num;
};

class PlayerBasicParticle
{
private:

	ParticleSetting setting;
	std::unique_ptr<ParticleManager> particle;

public:

	void Initialize(const std::string& fileName,const ParticleSetting& set);

	void Add(const Vector3& pos);

	void Update();

	void Draw();

	PlayerBasicParticle() = default;
	~PlayerBasicParticle() = default;
};

