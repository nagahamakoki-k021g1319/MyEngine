#include "Lamp.h"

Lamp::Lamp()
{
}

Lamp::~Lamp()
{
	delete spriteCommon;

	for ( int i = 0; i < 80; i++ ){
		delete RightObj_[i];
		delete LeftObj_[ i ];
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
	for ( int i = 0; i < 80; i++ )
	{
		//右ランプ
		RightObj_[i] = Object3d::Create();
		RightObj_[i]->SetModel(Model_);
		RightObj_[i]->wtf.scale = { 3.0f,3.0f,3.0f };
		RightObj_[i]->wtf.position = { 10.0f,6.0f,0.0f + i * 10 };

		//左ランプ
		LeftObj_[ i ] = Object3d::Create();
		LeftObj_[ i ]->SetModel(Model_);
		LeftObj_[ i ]->wtf.scale = { 1.5f,1.5f,1.5f };
		LeftObj_[ i ]->wtf.rotation.y = -3.2f;
		LeftObj_[ i ]->wtf.position = { -5.0f,4.0f,0.0f + i * 10 };
	}




}

void Lamp::Update()
{
	for ( int i = 0; i < 80; i++ ){
		RightObj_[i]->Update();
		RightObj_[ i ]->wtf.position.z -= 0.7f;
		if ( RightObj_[ i ]->wtf.position.z <= -300.0f )
		{
			RightObj_[ i ]->wtf.position.z = 500.0f;
		}

		LeftObj_[ i ]->Update();
		LeftObj_[ i ]->wtf.position.z -= 0.7f;
		if ( LeftObj_[ i ]->wtf.position.z <= -300.0f )
		{
			LeftObj_[ i ]->wtf.position.z = 500.0f;
		}
	}
}

void Lamp::Draw()
{
	for ( int i = 0; i < 80; i++ ){
		RightObj_[i]->Draw();
		LeftObj_[ i ]->Draw();
	}
}
