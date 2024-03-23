#pragma once
#include "WinApp.h"
#include "Input.h"
#include "DirectXCommon.h"
#include "FPS.h"
#include "ImGuiManager.h"
#include <imgui.h>

#include "GameScene.h"
#include <fbxsdk.h>
#include "Light/LightGroup.h"

class Framework
{
public://メンバ関数

	//初期化
	virtual void Initialize();
	//終了
	virtual void Finalize();
	//終了チェック
	virtual bool IsEndRequest() {return endRequest;}

public:
	virtual ~Framework() = default;

public:
	//実行
	void Run();

protected:

	//ポインタ
	WinApp* winApp = nullptr;
	DirectXCommon* dxCommon = nullptr;
	FPS* fps = new FPS;
	Input* input = nullptr;
	GameScene* gameScene = nullptr;

	ImGuiManager* imgui = nullptr;

private:
	bool endRequest = false;
};

