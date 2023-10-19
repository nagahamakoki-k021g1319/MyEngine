#include "ArmorEnemy.h"
#include <imgui.h>

ArmorEnemy::ArmorEnemy()
{
}

ArmorEnemy::~ArmorEnemy()
{
	delete spriteCommon;
	delete Obj_;
	delete Model_;
	delete Modelst_;
}

void ArmorEnemy::Initialize(DirectXCommon* dxCommon,Input* input)
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
	Model_ = Model::LoadFromOBJ("armorenemy");
	//雑魚敵(待機状態)
	Modelst_ = Model::LoadFromOBJ("armorenemyst");
	Obj_ = Object3d::Create();
	Obj_->SetModel(Modelst_);
	Obj_->wtf.scale = { 0.3f,0.3f,0.3f };
	Obj_->wtf.position = { 0.0f,-2.0f,10.0f };
}

void ArmorEnemy::Update()
{
	Obj_->Update();
	isGameStartTimer++;

	//攻撃時モデルが変わる
	if ( input_->PushKey(DIK_6) ){
		Obj_->SetModel(Model_);
	}
	else{
		Obj_->SetModel(Modelst_);
	}

	//ポリゴン爆散
	if ( input_->TriggerKey(DIK_5) ){isExpolFlag = true;}
	if ( isExpolFlag == true){
		ExpolTimer++;

		float polygon = ExpolTimer / ExpolMT;

		Obj_->SetDestruction(polygon);
		Obj_->Setalpha(static_cast< float >( ( ExpolMT - ExpolTimer ) / ExpolMT ));
		if ( ExpolTimer >= ExpolMT )
		{
			isAliveFlag = false;
		}
	}

	ImGui::Begin("ArmorEnemy");

	ImGui::Text("isGameStartTimer:%d",isGameStartTimer);
	
	ImGui::End();

}

void ArmorEnemy::Draw()
{
	if ( isGameStartTimer >= 200){
		if ( isAliveFlag == true ){
			Obj_->Draw();
		}
	}
}

Vector3 ArmorEnemy::GetWorldPosition()
{
	return Vector3();
}
