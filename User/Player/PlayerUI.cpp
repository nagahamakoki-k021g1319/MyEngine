#include "PlayerUI.h"

PlayerUI::PlayerUI()
{
}

PlayerUI::~PlayerUI()
{
	//delete spriteCommon;
	//delete operationUI;

}

void PlayerUI::Initialize()
{
	operationUI = new Sprite();
	operationUI->Initialize(spriteCommon);
	operationUI->SetPozition({ 0,0 });
	operationUI->SetSize({ 1280.0f, 720.0f });

	spriteCommon->LoadTexture(1,"sousa.png");
	operationUI->SetTextureIndex(1);

}

void PlayerUI::Update()
{
}

void PlayerUI::Draw()
{
	operationUI->Draw();
}
