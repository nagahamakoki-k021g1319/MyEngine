#include "Lamp.h"

Lamp::Lamp()
{
}

Lamp::~Lamp()
{
	delete spriteCommon;

	delete Obj_;
	delete Model_;
}

void Lamp::Initialize(DirectXCommon* dxCommon,Input* input)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);

	this->dxCommon_ = dxCommon;
	input_ = input;
	//スプライト共通部分の初期化
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon);


	//雑魚敵(攻撃状態)
	Model_ = Model::LoadFromOBJ("lamp");
	Obj_ = Object3d::Create();
	Obj_->SetModel(Model_);
	Obj_->wtf.position = { -3.0f,1.0f,0.0f };

}

void Lamp::Update()
{
	Obj_->Update();

}

void Lamp::Draw()
{
	Obj_->Draw();
}
