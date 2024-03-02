#include "PlayerBasicParticle.h"

void BasicParticle::Initialize(const std::string& fileName,const ParticleSetting& set)
{
	setting = set;

	particle = std::make_unique<ParticleManager>();
	particle->Initialize();
	particle->LoadTexture(fileName);
}

void BasicParticle::Add(const Vector3& pos)
{
	//パーティクル範囲
	for ( int i = 0; i < setting.num; i++ )
	{
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		Vector3 posG{};
		posG.x += ( float ) rand() / RAND_MAX * setting.pos.x - setting.pos.x / 2.0f;
		posG.y += ( float ) rand() / RAND_MAX * setting.pos.y - setting.pos.y / 2.0f;
		posG.z += ( float ) rand() / RAND_MAX * setting.pos.z - setting.pos.z / 2.0f;
		posG += pos;
		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		Vector3 velG{};
		velG.x = ( float ) rand() / RAND_MAX * setting.vel.x - setting.vel.x / 2.0f;
		velG.y = ( float ) rand() / RAND_MAX * setting.vel.y - setting.vel.y / 2.0f;
		velG.z = ( float ) rand() / RAND_MAX * setting.vel.z - setting.vel.z / 2.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		Vector3 accG{};
		accG.x = ( float ) rand() / RAND_MAX * setting.acc / 2.0f;
		accG.y = ( float ) rand() / RAND_MAX * setting.acc - setting.acc / 2.0f;

		//追加
		particle->Add(setting.life,posG,velG,accG,setting.scale.x,setting.scale.y);
	}
}

void BasicParticle::Update()
{
	particle->Update();
}

void BasicParticle::Draw()
{
	particle->Draw();
}
