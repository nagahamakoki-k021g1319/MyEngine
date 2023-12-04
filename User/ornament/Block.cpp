#include "Block.h"
#include "Player.h"

Block::Block()
{
}

Block::~Block()
{
	delete spriteCommon;
	//木箱
	for ( int i = 0; i < 3; i++ )
	{
		delete boxObj_[i];
		delete boxModel_[i];
	}
	//三角コーン
	for ( int i = 0; i < 2; i++ )
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
	for ( int i = 0; i < 3; i++ )
	{
		boxModel_[ i ] = Model::LoadFromOBJ("box");
		boxObj_[ i ] = Object3d::Create();
		boxObj_[ i ]->SetModel(boxModel_[ i ]);
		boxObj_[ i ]->wtf.scale = { 0.3f,0.3f,0.3f };
	}
	boxObj_[0]->wtf.position = { 0.0f,-1.5f,101.0f };
	boxObj_[1]->wtf.position = { 0.3f,-2.0f,101.0f };
	boxObj_[2]->wtf.position = { -0.3f,-2.0f,101.0f };

	//三角コーン
	for ( int i = 0; i < 2; i++ )
	{
		konModel_[i] = Model::LoadFromOBJ("kon");
		konObj_[i] = Object3d::Create();
		konObj_[i]->SetModel(konModel_[i]);
		konObj_[i]->wtf.scale = { 0.4f,0.4f,0.4f };
	}
	konObj_[0]->wtf.position = { 0.5f,-2.0f,100.0f };
	konObj_[1]->wtf.position = { -0.5f,-2.0f,100.0f };



	//自機の当たり判定
	collModel_ = Model::LoadFromOBJ("collboll");
	collObj_ = Object3d::Create();
	collObj_->SetModel(collModel_);
	collObj_->wtf.scale = { 1.0f,1.0f,1.0f };

}

void Block::Update(Vector3 playerPos)
{
	//木箱
	for ( int i = 0; i < 3; i++ ){boxObj_[i]->Update();}
	//三角コーン
	for ( int i = 0; i < 2; i++ ){konObj_[i]->Update();}

	collObj_->Update();
	collObj_->wtf.position = { playerPos.x,playerPos.y - 0.5f,playerPos.z + 2.0f };

	isGameStartTimer++;

	//ゲームが始まったら全て障害物が迫る
	if ( isGameStartTimer >= 240){
		//木箱
		for ( int i = 0; i < 3; i++ )
		{
			boxObj_[ i ]->wtf.position.z -= 0.3f;
		}
		//三角コーン
		for ( int i = 0; i < 2; i++ )
		{
			konObj_[ i ]->wtf.position.z -= 0.3f;
		}

	}

	//当たった時に障害物が散らばる
	if ( input_->TriggerKey(DIK_R) ){
		for ( int i = 0; i < 3; i++ )
		{
			isBoxScatterFlag_[i] = 1;
		}
		for ( int i = 0; i < 2; i++ )
		{
			isKonScatterFlag_[i] = 1;
		}
	}

	//木箱が散らばる
	for ( int i = 0; i < 3; i++ )
	{
		if ( isBoxScatterFlag_[i] == 1 )
		{
			boxScattertimer_[ i ]++;

			if ( boxScattertimer_[ i ] >= 1 && boxScattertimer_[ i ] <= 10 )
			{
				boxObj_[i]->wtf.position.z += 2.0f;
			}
			if ( boxScattertimer_[ i ] >= 11 )
			{
				boxObj_[i]->wtf.position.z -= 0.4f;
				boxObj_[ i ]->wtf.position.y += 0.02f;
			}
			boxObj_[i]->wtf.rotation.x += 0.03f;
			boxObj_[i]->wtf.rotation.z += 0.03f;

		}
	}
	//木箱の当たり判定
	for ( int i = 0; i < 3; i++ )
	{
		if ( coll.CircleCollision(playerPos,boxObj_[ i ]->wtf.position,0.5f,0.5f) )
		{
			isBoxScatterFlag_[ i ] = 1;
			player_->isDecelerationFlag = true;
		}
		
	}


	//三角コーンが散らばる
	for ( int i = 0; i < 2; i++ )
	{
		if ( isKonScatterFlag_[i] == 1 )
		{
			konScattertimer_[ i ]++;

			if ( konScattertimer_[ i ] >= 1 && konScattertimer_[ i ] <= 10 )
			{
				konObj_[i]->wtf.position.z += 2.0f;
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
	for ( int i = 0; i < 2; i++ ){
		if ( coll.CircleCollision(playerPos,konObj_[ i ]->wtf.position,0.5f,0.5f) )
		{
			isKonScatterFlag_[ i ] = 1;
			player_->isDecelerationFlag = true;
		}

	}



}

void Block::Draw()
{
	//木箱
	for ( int i = 0; i < 3; i++ )
	{
		boxObj_[i]->Draw();
	}
	//三角コーン
	for ( int i = 0; i < 2; i++ )
	{
		konObj_[i]->Draw();
	}

	/*collObj_->Draw();*/
}
