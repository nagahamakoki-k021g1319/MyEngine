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

	//実行
	void Run();


	virtual ~Framework() = default;

	//初期化
	virtual void Initialize();

	//終了
	virtual void Finalize();

	//毎フレーム処理
	virtual void Update();

	//描画
	virtual void Draw() = 0;

	////終了チェック
	//virtual bool IsEndRequst() {
	//	return endRequest_;
	//}


};

