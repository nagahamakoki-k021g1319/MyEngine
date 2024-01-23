#include "Block.h"
#include "Player.h"
#include "Enemy/BikeEnemy.h"

Block::Block()
{
}

Block::~Block()
{
	delete spriteCommon;
	//木箱
	for ( int i = 0; i < 12; i++ )
	{
		delete boxObj_[i];
		delete boxModel_[i];
	}
	//三角コーン
	for ( int i = 0; i < 8; i++ )
	{
		delete konObj_[ i ];
		delete konModel_[ i ];
	}

	delete collObj_;
	delete collModel_;

}

void Block::Initialize(DirectXCommon* dxCommon,Input* input)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);

	this->dxCommon_ = dxCommon;
	input_ = input;
	//スプライト共通部分の初期化
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon);

	//木箱
	for ( int i = 0; i < 12; i++ )
	{
		boxModel_[ i ] = Model::LoadFromOBJ("box");
		boxObj_[ i ] = Object3d::Create();
		boxObj_[ i ]->SetModel(boxModel_[ i ]);
		boxObj_[ i ]->wtf.scale = { 0.3f,0.3f,0.3f };
	}
	

	//三角コーン
	for ( int i = 0; i < 8; i++ )
	{
		konModel_[i] = Model::LoadFromOBJ("kon");
		konObj_[i] = Object3d::Create();
		konObj_[i]->SetModel(konModel_[i]);
		konObj_[i]->wtf.scale = { 0.4f,0.4f,0.4f };
	}
	
	InitialPosition();

	//自機の当たり判定
	collModel_ = Model::LoadFromOBJ("collboll");
	collObj_ = Object3d::Create();
	collObj_->SetModel(collModel_);
	collObj_->wtf.scale = { 1.0f,1.0f,1.0f };

}

void Block::Update(Vector3 playerPos)
{

	//木箱
	for ( int i = 0; i < 12; i++ ){boxObj_[i]->Update();}
	//三角コーン
	for ( int i = 0; i < 8; i++ ){konObj_[i]->Update();}

	collObj_->Update();
	collObj_->wtf.position = {playerPos};



	isGameStartTimer++;

	//ゲームが始まったら全て障害物が迫る
	if ( isGameStartTimer >= 280 && player_->isRoundFlag >= 3 ){
		//木箱
		for ( int i = 0; i < 12; i++ )
		{
			boxObj_[ i ]->wtf.position.z -= 0.3f;
		}
		//三角コーン
		for ( int i = 0; i < 8; i++ )
		{
			konObj_[ i ]->wtf.position.z -= 0.3f;
		}

	}

	//木箱が散らばる
	for ( int i = 1; i < 12; i++ )
	{
		if ( isBoxScatterFlag_[i] == 1 )
		{
			boxScattertimer_[ i ]++;

			if ( boxScattertimer_[ i ] >= 1 && boxScattertimer_[ i ] <= 10 )
			{
				boxObj_[i]->wtf.position.z += 3.0f;
			}
			if ( boxScattertimer_[ i ] >= 11 )
			{
				boxObj_[i]->wtf.position.z -= 0.4f;
			}
			boxObj_[i]->wtf.rotation.x += 0.06f;
			boxObj_[i]->wtf.rotation.z += 0.1f;

		}
	}

	if ( isBoxScatterFlag_[0] == 1 )
	{
		boxScattertimer_[0]++;

		if ( boxScattertimer_[0] >= 1 && boxScattertimer_[0] <= 10 )
		{
			boxObj_[0]->wtf.position.z += 3.5f;
		}
		if ( boxScattertimer_[0] >= 11 )
		{
			boxObj_[0]->wtf.position.z -= 0.4f;
		}
		boxObj_[0]->wtf.rotation.x += 0.06f;
		boxObj_[0]->wtf.rotation.z += 0.1f;

	}

	//木箱の当たり判定
	for ( int i = 0; i < 12; i++ )
	{
		if ( player_->isDecelerationFlag == false)
		{
			//障害物と当たったらプレイヤーがノックバックする
			if ( coll.CircleCollision(playerPos,boxObj_[ i ]->wtf.position,0.5f,0.5f) )
			{
				player_->isDecelerationFlag = true;
			}
		}
		//障害物が吹っ飛ぶ
		if ( coll.CircleCollision(playerPos,boxObj_[ i ]->wtf.position,0.5f,0.5f) )
		{
			isBoxScatterFlag_[ i ] = 1;
		}
	}

	for ( int i = 0; i < 3; i++ )
	{
		if ( bikeEnemy_->isBoxFlag_[i] == 1 )
		{
			isBoxScatterFlag_[i] = 1;
		}
	}

	//三角コーンが散らばる
	for ( int i = 0; i < 8; i++ )
	{
		if ( isKonScatterFlag_[i] == 1 )
		{
			konScattertimer_[ i ]++;

			if ( konScattertimer_[ i ] >= 1 && konScattertimer_[ i ] <= 10 )
			{
				konObj_[i]->wtf.position.z += 3.5f;
			}
			if ( konScattertimer_[ i ] >= 11 )
			{
				konObj_[ i ]->wtf.position.z -= 0.4f;
				konObj_[ i ]->wtf.position.y += 0.02f;
				konObj_[ i ]->wtf.scale.x -= 0.002f;
				konObj_[ i ]->wtf.scale.y -= 0.002f;
				konObj_[ i ]->wtf.scale.z -= 0.002f;
			}
			konObj_[ i ]->wtf.rotation.x += 0.1f;
			konObj_[ i ]->wtf.rotation.z += 0.1f;


		}
	}
	//三角コーンの当たり判定
	for ( int i = 0; i < 8; i++ ){
		if ( isKonScatterFlag_[ i ] == 0 )
		{
			if ( player_->isDecelerationFlag == false )
			{
				if ( coll.CircleCollision(playerPos,konObj_[ i ]->wtf.position,0.5f,0.5f) )
				{
					player_->isDecelerationFlag = true;
				}
			}
			if ( coll.CircleCollision(playerPos,konObj_[ i ]->wtf.position,0.5f,0.5f) )
			{
				isKonScatterFlag_[ i ] = 1;
			}
		}
	}



}

void Block::Draw()
{
	if ( isGameStartTimer >= 280 && player_->isRoundFlag >= 3 )
	{
	//木箱
		for ( int i = 0; i < 12; i++ )
		{
			boxObj_[ i ]->Draw();
		}
		//三角コーン
		for ( int i = 0; i < 8; i++ )
		{
			konObj_[ i ]->Draw();
		}
	}

	/*collObj_->Draw();*/
}

void Block::InitialPosition()
{
	//1番目の障害物
	boxObj_[ 0 ]->wtf.position = { 0.0f,-1.5f,101.0f };
	boxObj_[ 1 ]->wtf.position = { boxObj_[ 0 ]->wtf.position.x + 0.3f,-2.0f,boxObj_[ 0 ]->wtf.position.z };
	boxObj_[ 2 ]->wtf.position = { boxObj_[ 0 ]->wtf.position.x -0.3f,-2.0f,boxObj_[ 0 ]->wtf.position.z };

	konObj_[ 0 ]->wtf.position = { boxObj_[ 0 ]->wtf.position.x + 0.5f, -2.0f,boxObj_[ 0 ]->wtf.position.z - 1.0f };
	konObj_[ 1 ]->wtf.position = { boxObj_[ 0 ]->wtf.position.x - 0.5f,-2.0f, boxObj_[ 0 ]->wtf.position.z - 1.0f };

	//2番目の障害物
	boxObj_[ 3 ]->wtf.position = { 1.5f,-1.5f,201.0f };
	boxObj_[ 4 ]->wtf.position = { boxObj_[ 3 ]->wtf.position.x + 0.3f,-2.0f,boxObj_[ 3 ]->wtf.position.z };
	boxObj_[ 5 ]->wtf.position = { boxObj_[ 3 ]->wtf.position.x -0.3f,-2.0f,boxObj_[ 3 ]->wtf.position.z };

	konObj_[ 2 ]->wtf.position = { boxObj_[ 3 ]->wtf.position.x + 0.5f,-2.0f,boxObj_[ 3 ]->wtf.position.z - 1.0f };
	konObj_[ 3 ]->wtf.position = { boxObj_[ 3 ]->wtf.position.x -0.5f,-2.0f,boxObj_[ 3 ]->wtf.position.z - 1.0f };

	//3番目の障害物
	boxObj_[ 6 ]->wtf.position = { 2.5f,-1.5f,251.0f };
	boxObj_[ 7 ]->wtf.position = { boxObj_[ 6 ]->wtf.position.x + 0.3f,-2.0f,boxObj_[ 6 ]->wtf.position.z };
	boxObj_[ 8 ]->wtf.position = { boxObj_[ 6 ]->wtf.position.x -0.3f,-2.0f,boxObj_[ 6 ]->wtf.position.z };

	konObj_[ 4 ]->wtf.position = { boxObj_[ 6 ]->wtf.position.x + 0.5f,-2.0f,boxObj_[ 6 ]->wtf.position.z -1.0f };
	konObj_[ 5 ]->wtf.position = { boxObj_[ 6 ]->wtf.position.x -0.5f,-2.0f,boxObj_[ 6 ]->wtf.position.z - 1.0f };

	//4番目の障害物
	boxObj_[ 9 ]->wtf.position = { -1.5f,-1.5f,351.0f };
	boxObj_[ 10 ]->wtf.position = { boxObj_[ 9 ]->wtf.position.x + 0.3f,-2.0f,boxObj_[ 9 ]->wtf.position.z };
	boxObj_[ 11 ]->wtf.position = { boxObj_[ 9 ]->wtf.position.x -0.3f,-2.0f,boxObj_[ 9 ]->wtf.position.z };

	konObj_[ 6 ]->wtf.position = { boxObj_[ 9 ]->wtf.position.x + 0.5f,-2.0f,boxObj_[ 9 ]->wtf.position.z - 1.0f };
	konObj_[ 7 ]->wtf.position = { boxObj_[ 9 ]->wtf.position.x -0.5f,-2.0f,boxObj_[ 9 ]->wtf.position.z - 1.0f };

}

Vector3 Block::GetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;

	boxObj_[ 0 ]->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	worldPos.x = boxObj_[ 0 ]->wtf.matWorld.m[ 3 ][ 0 ];
	worldPos.y = boxObj_[ 0 ]->wtf.matWorld.m[ 3 ][ 1 ];
	worldPos.z = boxObj_[ 0 ]->wtf.matWorld.m[ 3 ][ 2 ];

	return worldPos;
}
