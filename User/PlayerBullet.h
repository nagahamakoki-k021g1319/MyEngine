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
	///初期化
	///</summary>
	void Initialize(Model* Model_);
	///< summary>
	///更新
	///</summary>
	void CrystalBAttack();

	///< summary>
	///更新
	///</summary>
	void Update();

	///< summary>
	///描画
	///</summary>
	void Draw();




};

