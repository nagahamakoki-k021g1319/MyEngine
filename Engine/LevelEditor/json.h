﻿/**
 * @file json.h
 * @brief json情報読み取り
 */
#pragma once
#include <vector>
#include <DirectXMath.h>
#include<string>
#include <map>

#pragma warning(disable: 4324)
// レベルデータ
struct LevelData {

	struct ObjectData {
		// ファイル名
		std::string fileName;
		// 平行移動
		DirectX::XMVECTOR translation;
		// 回転角
		DirectX::XMVECTOR rotation;
		// スケーリング
		DirectX::XMVECTOR scaling;
	};

	// オブジェクト配列
	std::vector<ObjectData> objects;
};
class JsonLoader {
public:// 定数
	// デフォルトの読み込みディレクトリ
	static const std::string kDefaultBaseDirectory;
	// ファイル拡張子
	static const std::string kExtension;

public:// メンバ関数

	/// <summary>
	/// レベルデータファイルの読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	static LevelData* LoadFile(const std::string& fileName);
};