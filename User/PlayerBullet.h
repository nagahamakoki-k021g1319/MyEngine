#pragma once
#include "Model.h"
#include "Input.h"

#include <cassert>
#include "Object3d.h"

#include "Affin.h"
#include <memory>
#include <list>

class PlayerBullet
{
public:
	~PlayerBullet();

	///< summary>
	///������
	///</summary>
	void Initialize(Model* Model_);
	///< summary>
	///�X�V
	///</summary>
	void CrystalBAttack();

	///< summary>
	///�X�V
	///</summary>
	void Update();

	///< summary>
	///�`��
	///</summary>
	void Draw();




};

