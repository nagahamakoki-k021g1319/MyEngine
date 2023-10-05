#pragma once
#pragma warning(push)
#include <Windows.h>
#pragma warning(pop)

#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <string>

#include <DirectXMath.h>
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Affin.h"

#include "FbxLoader.h"
#include "FBXModel.h"

#include "Camera.h"
#include "Transform.h"

class FBXObject3d
{
protected: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferDataB0
	{
		Matrix4 mat;	// �R�c�ϊ��s��
	};

	//�{�[���̍ő吔
	static const int MAX_BONES = 32;

	//�萔�o�b�t�@�p�̃f�[�^�\����(�X�L�j���O)
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};


public: // �ÓI�����o�֐�
	/// <summary>
	/// �O���t�B�b�N�p�C�v���C���̐���
	/// </summary>
	static void CreateGraphicsPipeline();

	/// <summary>
	/// �O���t�B�b�N�p�C�v���C���̐���
	/// </summary>
	// setter
	static void SetDevice(ID3D12Device* device_) { FBXObject3d::device = device_; }
	static void SetCamera(Camera* camera_) { FBXObject3d::camera = camera_; }


private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device;
	// �J����
	static Camera* camera;
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;


public: // �����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	void SetModel(FBXModel* fbxmodel_) { this->fbxmodel = fbxmodel_; }

	/// <summary>
	/// �A�j���[�V�����J�n
	/// </summary>
	void PlayAnimation(float speed_ = 1.0f, bool isLoop_ = true);

	void StopAnimation() { isPlay = false; };

	bool GetAnimationFin() { return isFin; };

	void SetPozition(const Vector3& position_);

protected: // �����o�ϐ�
	ComPtr<ID3D12Resource> constBuffB0; // �萔�o�b�t�@
	// �萔�o�b�t�@(�X�L��)
	ComPtr<ID3D12Resource> constBuffSkin;
	// ���f��
	FBXModel* fbxmodel = nullptr;

	//1�t���[���̎���
	FbxTime frameTime;
	FbxTime animationTime;
	//�A�j���[�V�����J�n����
	FbxTime startTime;
	//�A�j���[�V�����I������
	FbxTime endTime;
	//���ݎ���(�A�j���[�V����)
	FbxTime currentTime;
	int frame = 0;
	//�A�j���[�V�����Đ���
	bool isPlay = false;
	//�A�j���[�V�������[�v
	bool isLoop;
	//�A�j���[�V�����I��
	bool isFin;

public:
	Transform wtf;
};