#include "Block.h"
#include "Player/Player.h"
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

void Block::BlockReset()
{
	/*isGameStartTimer = 0;
	isBoxScatterFlag_[ 12 ] = { 0 };
	boxScattertimer_[ 12 ] = { 0 };
	isKonScatterFlag_[ 8 ] = { 0 };
	konScattertimer_[ 8 ] = { 0 };
	InitialPosition();*/
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
		const float posSpeed = 1.0f;
		//木箱
		for ( int i = 0; i < 12; i++ )
		{
			boxObj_[ i ]->wtf.position.z -= posSpeed;
		}
		//三角コーン
		for ( int i = 0; i < 8; i++ )
		{
			konObj_[ i ]->wtf.position.z -= posSpeed;
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
				const float posSpeed = 3.0f;
				boxObj_[i]->wtf.position.z += posSpeed;
			}
			if ( boxScattertimer_[ i ] >= 11 )
			{
				const float posSpeed = 0.4f;
				boxObj_[i]->wtf.position.z -= posSpeed;
			}
			const float RotposX = 0.06f;
			const float RotposZ = 0.1f;
			boxObj_[i]->wtf.rotation.x += RotposX;
			boxObj_[i]->wtf.rotation.z += RotposZ;

		}
	}

	if ( isBoxScatterFlag_[0] == 1 )
	{
		boxScattertimer_[0]++;

		if ( boxScattertimer_[0] >= 1 && boxScattertimer_[0] <= 10 )
		{
			const float speedPosZ = 3.5f;
			boxObj_[0]->wtf.position.z += speedPosZ;
		}
		if ( boxScattertimer_[0] >= 11 )
		{
			const float speedPosZ = 0.4f;
			boxObj_[0]->wtf.position.z -= speedPosZ;
		}
		const float rotSpeedX = 0.06f;
		const float rotSpeedZ = 0.1f;
		boxObj_[0]->wtf.rotation.x += rotSpeedX;
		boxObj_[0]->wtf.rotation.z += rotSpeedZ;

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

	

	//三角コーンが散らばる
	for ( int i = 0; i < 8; i++ )
	{
		if ( isKonScatterFlag_[i] == 1 )
		{
			konScattertimer_[ i ]++;

			if ( konScattertimer_[ i ] >= 1 && konScattertimer_[ i ] <= 10 )
			{
				const float posSpeedZ = 3.5f;
				konObj_[i]->wtf.position.z += posSpeedZ;
			}
			if ( konScattertimer_[ i ] >= 11 )
			{
				const float posSpeedZ = 0.4f;
				const float posSpeedY = 0.02f;
				const float scaledownPos = 0.002f;
				konObj_[ i ]->wtf.position.z -= posSpeedZ;
				konObj_[ i ]->wtf.position.y += posSpeedY;
				konObj_[ i ]->wtf.scale.x -= scaledownPos;
				konObj_[ i ]->wtf.scale.y -= scaledownPos;
				konObj_[ i ]->wtf.scale.z -= scaledownPos;
			}
			const float rotPos = 0.1f;
			konObj_[ i ]->wtf.rotation.x += rotPos;
			konObj_[ i ]->wtf.rotation.z += rotPos;


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

	//当たり判定
	for ( int i = 0; i < 12; i++ )
	{
		if ( bikeEnemy_->isBoxFlag_[ i ] == 1 )
		{
			isBoxScatterFlag_[ i ] = 1;
		}
	}

	for ( int i = 0; i < 8; i++ )
	{
		if ( bikeEnemy_->isKonFlag_[ i ] == 1 )
		{
			isKonScatterFlag_[ i ] = 1;
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
	//木箱
	const float box1WPosX = 0.0f;
	const float box1WPosY = -1.5f;
	const float box1WPosZ = 101.1f;
	const float boxaddPosX = 0.3f;
	const float boxaddPosY = -2.0f;
	//コーン
	const float konaddPosX = 0.5f;
	const float konaddPosZ = 1.0f;

	//1番目の障害物
	boxObj_[ 0 ]->wtf.position = { box1WPosX,box1WPosY,box1WPosZ };
	boxObj_[ 1 ]->wtf.position = { boxObj_[ 0 ]->wtf.position.x + boxaddPosX,boxaddPosY,boxObj_[ 0 ]->wtf.position.z };
	boxObj_[ 2 ]->wtf.position = { boxObj_[ 0 ]->wtf.position.x - boxaddPosX,boxaddPosY,boxObj_[ 0 ]->wtf.position.z };

	konObj_[ 0 ]->wtf.position = { boxObj_[ 0 ]->wtf.position.x + konaddPosX,boxaddPosY,boxObj_[ 0 ]->wtf.position.z - konaddPosZ };
	konObj_[ 1 ]->wtf.position = { boxObj_[ 0 ]->wtf.position.x - konaddPosX,boxaddPosY, boxObj_[ 0 ]->wtf.position.z - konaddPosZ };

	//2番目の障害物
	boxObj_[ 3 ]->wtf.position = { 1.5f,-1.5f,201.0f };
	boxObj_[ 4 ]->wtf.position = { boxObj_[ 3 ]->wtf.position.x + boxaddPosX,boxaddPosY,boxObj_[ 3 ]->wtf.position.z };
	boxObj_[ 5 ]->wtf.position = { boxObj_[ 3 ]->wtf.position.x - boxaddPosX,boxaddPosY,boxObj_[ 3 ]->wtf.position.z };

	konObj_[ 2 ]->wtf.position = { boxObj_[ 3 ]->wtf.position.x + konaddPosX,boxaddPosY,boxObj_[ 3 ]->wtf.position.z - konaddPosZ };
	konObj_[ 3 ]->wtf.position = { boxObj_[ 3 ]->wtf.position.x - konaddPosX,boxaddPosY,boxObj_[ 3 ]->wtf.position.z - konaddPosZ };

	//3番目の障害物
	boxObj_[ 6 ]->wtf.position = { 2.5f,-1.5f,251.0f };
	boxObj_[ 7 ]->wtf.position = { boxObj_[ 6 ]->wtf.position.x + boxaddPosX,boxaddPosY,boxObj_[ 6 ]->wtf.position.z };
	boxObj_[ 8 ]->wtf.position = { boxObj_[ 6 ]->wtf.position.x - boxaddPosX,boxaddPosY,boxObj_[ 6 ]->wtf.position.z };

	konObj_[ 4 ]->wtf.position = { boxObj_[ 6 ]->wtf.position.x + konaddPosX,boxaddPosY,boxObj_[ 6 ]->wtf.position.z - konaddPosZ };
	konObj_[ 5 ]->wtf.position = { boxObj_[ 6 ]->wtf.position.x - konaddPosX,boxaddPosY,boxObj_[ 6 ]->wtf.position.z - konaddPosZ };

	//4番目の障害物
	boxObj_[ 9 ]->wtf.position = { -1.5f,-1.5f,351.0f };
	boxObj_[ 10 ]->wtf.position = { boxObj_[ 9 ]->wtf.position.x + boxaddPosX,boxaddPosY,boxObj_[ 9 ]->wtf.position.z };
	boxObj_[ 11 ]->wtf.position = { boxObj_[ 9 ]->wtf.position.x - boxaddPosX,boxaddPosY,boxObj_[ 9 ]->wtf.position.z };

	konObj_[ 6 ]->wtf.position = { boxObj_[ 9 ]->wtf.position.x + konaddPosX,boxaddPosY,boxObj_[ 9 ]->wtf.position.z - konaddPosZ };
	konObj_[ 7 ]->wtf.position = { boxObj_[ 9 ]->wtf.position.x - konaddPosX,boxaddPosY,boxObj_[ 9 ]->wtf.position.z - konaddPosZ };

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

Vector3 Block::GetWorldPosition2()
{
	//ワールド座標を入れる変数
	Vector3 worldPos2;

	boxObj_[ 3 ]->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	worldPos2.x = boxObj_[ 3 ]->wtf.matWorld.m[ 3 ][ 0 ];
	worldPos2.y = boxObj_[ 3 ]->wtf.matWorld.m[ 3 ][ 1 ];
	worldPos2.z = boxObj_[ 3 ]->wtf.matWorld.m[ 3 ][ 2 ];

	return worldPos2;
}

Vector3 Block::GetWorldPosition3()
{
	//ワールド座標を入れる変数
	Vector3 worldPos3;

	boxObj_[ 6 ]->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	worldPos3.x = boxObj_[ 6 ]->wtf.matWorld.m[ 3 ][ 0 ];
	worldPos3.y = boxObj_[ 6 ]->wtf.matWorld.m[ 3 ][ 1 ];
	worldPos3.z = boxObj_[ 6 ]->wtf.matWorld.m[ 3 ][ 2 ];

	return worldPos3;
}

Vector3 Block::GetWorldPosition4()
{
	//ワールド座標を入れる変数
	Vector3 worldPos4;

	boxObj_[ 9 ]->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	worldPos4.x = boxObj_[ 9 ]->wtf.matWorld.m[ 3 ][ 0 ];
	worldPos4.y = boxObj_[ 9 ]->wtf.matWorld.m[ 3 ][ 1 ];
	worldPos4.z = boxObj_[ 9 ]->wtf.matWorld.m[ 3 ][ 2 ];

	return worldPos4;
}
