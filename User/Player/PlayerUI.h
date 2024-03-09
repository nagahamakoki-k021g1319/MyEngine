#pragma once
#include "DirectXCommon.h"
#include "SpriteCommon.h"
#include "Sprite.h"

class PlayerUI
{
public:
	PlayerUI();
	~PlayerUI();

	void Initialize();
	void Update();
	void Draw();



private:
	SpriteCommon* spriteCommon = nullptr;

	Sprite* operationUI = nullptr;
	
};

