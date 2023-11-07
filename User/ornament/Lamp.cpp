#include "Lamp.h"

Lamp::Lamp()
{
}

Lamp::~Lamp()
{
	delete spriteCommon;

	for ( int i = 0; i < 10; i++ )
	{
		delete Obj_[i];
	}
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


	//ランプ
	Model_ = Model::LoadFromOBJ("lamp");
	for ( int i = 0; i < 10; i++ )
	{
		Obj_[i] = Object3d::Create();
		Obj_[i]->SetModel(Model_);
		Obj_[i]->wtf.scale = { 1.5f,1.5f,1.5f };
		Obj_[i]->wtf.position = { 3.0f,2.0f,0.0f + i * 20 };
	}
}

void Lamp::Update()
{
	for ( int i = 0; i < 10; i++ )
	{
		Obj_[i]->Update();
	}
}

void Lamp::Draw()
{
	for ( int i = 0; i < 10; i++ )
	{
		Obj_[i]->Draw();
	}
}
