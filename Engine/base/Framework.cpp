#include "Framework.h"

void Framework::Initialize()
{

#pragma region WindowsAPI初期化処理
	//windowsAPIの初期化
	winApp = new WinApp();
	winApp->Initialize();

	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	//入力の初期化　
	input = new Input();
	input->Initialize(winApp);

	// ImGuiの初期化
	imgui = new ImGuiManager();
	imgui->Initialize(winApp,dxCommon);

#pragma endregion

#pragma region DirectX初期化処理
	// 3Dオブジェクト静的初期化
	Object3d::StaticInitialize(dxCommon->GetDevice());
	//パーティクル静的初期化
	ParticleManager::StaticInitialize(dxCommon->GetDevice(),dxCommon->GetCommandList());
	// ライトの静的初期化
	LightGroup::StaticInititalize(dxCommon->GetDevice());
#pragma endregion

#pragma region 描画初期化処理

	////////////////////////////
	//------音声読み込み--------//
	///////////////////////////

	// ゲームシーンの初期化
	gameScene = new GameScene();
	gameScene->Initialize(dxCommon,input);

	//FPS変えたいとき
	fps->SetFrameRate(60);

#pragma endregion
}

void Framework::Finalize()
{
#pragma region  WindowsAPI後始末
	delete gameScene;

	imgui->Finalize();
	//WindowsAPIの終了処理
	winApp->Finalize();
	delete imgui;

	//入力開放
	delete input;
	//WindowsAPI開放
	delete winApp;
	delete dxCommon;

	delete fps;

#pragma endregion
}

void Framework::Run()
{
	// ゲームの初期化
	Initialize();

	//ゲームループ
	while (true) {
#pragma region ウィンドウメッセージ処理

		//アプリケーションが終わる時にmessageがWM_QUITになる
		if (winApp->ProcessMessage()) {
			break;
		}
		if (input->PushKey(DIK_ESCAPE)) {
			break;
		}


		fps->FpsControlBegin();

#pragma endregion

#pragma region DirectX毎フレーム処理
		/////////////////////////////////////////////////////
		//----------DireceX毎フレーム処理　ここから------------//
		///////////////////////////////////////////////////

		//入力の更新
		input->Update();	

		// Imgui受付開始
		imgui->Begin();

		// ゲームシーンの毎フレーム処理
		gameScene->Update();		

		//////////////////////////////////////////////
		//-------DireceX毎フレーム処理　ここまで--------//
		////////////////////////////////////////////

#pragma endregion

#pragma region グラフィックスコマンド

		//4.描画コマンドここから
		dxCommon->PreDraw();

		
		//// デモウィンドウの表示オン
		ImGui::ShowDemoWindow();

		// ゲームシーンの描画
		gameScene->Draw();


		// Imgui受付終了
		imgui->End();
		//// Imgui描画
		/*imgui->Draw();*/

		// 描画終了
		dxCommon->PostDraw();

		fps->FpsControlEnd();
		//4.描画コマンドここまで
	}

	// ゲームの終了
	Finalize();
}
