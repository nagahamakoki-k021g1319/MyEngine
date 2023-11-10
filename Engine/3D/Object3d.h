#pragma once
#pragma warning(push)
#include <Windows.h>
#pragma warning(pop)
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string.h>
#include "Model.h"


#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Affin.h"

#include "Transform.h"
#include "Camera.h"
#include "Light/LightGroup.h"

/// <summary>
/// 3Dオブジェクト
/// </summary>
class Object3d
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略

	// 定数バッファ用データ構造体
	struct ConstBufferDataB0
	{
		//XMFLOAT4 color;	// 色 (RGBA)
		Matrix4 mat;	// ３Ｄ変換行列
		Matrix4 viewproj;	// ３Ｄ変換行列
		Matrix4 world;
		Vector3 cameraPos;
		float shininess;//光沢度
		float alpha; //透明度
		Vector3 color;
	};

	// 定数バッファ用データ構造体
	struct ConstBufferPolygonExplosion
	{
		float _Destruction = 0.0f;
		float _ScaleFactor = 1.0f;
		float _RotationFactor = 0.0f;
		float _PositionFactor = 0.0f;
	};

private: // 定数
	static const int division = 50;					// 分割数
	static const float radius;				// 底面の半径
	static const float prizmHeight;			// 柱の高さ
	static const int planeCount = division * 2 + division * 2;		// 面の数
	static const int vertexCount = planeCount * 3;		// 頂点数


public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="window_width">画面幅</param>
	/// <param name="window_height">画面高さ</param>
	static void StaticInitialize(ID3D12Device* device_);

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns></returns>
	static Object3d* Create();

	bool IsDead() const { return  isDead_; }

	

private: // 静的メンバ変数
	// デバイス
	static ComPtr<ID3D12Device> device;
	
	// コマンドリスト
	static ComPtr<ID3D12GraphicsCommandList> cmdList;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;


	

	// ビュー行列
	static Matrix4 matView;
	// 射影行列
	static Matrix4 matProjection;
	// 視点座標
	static Vector3 eye;
	// 注視点座標
	static Vector3 target;
	// 上方向ベクトル
	static Vector3 up;


	static float focalLengs;

private:// 静的メンバ関数

	/// <summary>
	/// カメラ初期化
	/// </summary>
	/// <param name="window_width">画面横幅</param>
	/// <param name="window_height">画面縦幅</param>
	static void InitializeCamera();

	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	/// <returns>成否</returns>
	static void InitializeGraphicsPipeline();

	/// <summary>
	/// ビュー行列を更新
	/// </summary>
	static void UpdateViewMatrix();

public: // メンバ関数

	Object3d();
	~Object3d();
	
	bool Initialize();
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();
	void Update(Transform* parentWtf);

	void UpdateMat();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();	

	Object3d* GetParent() const { return parent; }

	void SetParent(Object3d* parent_) { this->parent = parent_; }
	static void SetCamera(Camera* camera_) { Object3d::camera = camera_; }

	//setter
	void SetModel(Model* model_) { this->model = model_; }

	const ConstBufferPolygonExplosion GetPolygonExplosion() {
		return ConstMapPolygon_;
	}

	void SetPolygonExplosion(const ConstBufferPolygonExplosion& polygonExplosion) {
		ConstMapPolygon_ = polygonExplosion;
	}

	/// <summary>
	/// ポリゴン爆散の進行具合
	/// </summary>
	void SetDestruction(float Destruction) {
		ConstMapPolygon_._Destruction = Destruction;
	}

	Matrix4 GetMatWorld() {return wtf.matWorld;}

	void SetShininess(float shininess) {shininess_ = shininess;}

	void Setalpha(float alpha) {alpha_ = alpha;}

	void SetColor(Vector3 color_) {color = color_;}

	/*static void SetLight(LightGroup* light) {
		Object3d::light = light;
	}*/

	Vector3 GetWorldPosition();

private: // メンバ変数
	ComPtr<ID3D12Resource> constBuffB0; // 定数バッファ
	ComPtr<ID3D12Resource> constBuffB1; // 定数バッファ
	ComPtr<ID3D12Resource> constBuffB2; // 定数バッファ

	bool isDead_ = false;

	//光沢度
	float shininess_ = 10;

	float alpha_ = 1.0f;
	// 色
	Vector3 color ={ 1,1,1};	

	

	// 親オブジェクト
	Object3d* parent = nullptr;

	ConstBufferPolygonExplosion ConstMapPolygon_;

	//// ライト
	//static LightGroup* light;
	//// コマンドリスト
	//ID3D12GraphicsCommandList* ScmdList;

	//モデル
	Model* model = nullptr;
	static Camera* camera;

	static float win_wi, win_hi;
public:
	Transform wtf;

};