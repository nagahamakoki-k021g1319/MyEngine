#pragma once
#include <Vector3.h>
#include <memory>
#include <vector>

class SplinePosition {

public://基本関数
	/// <summary>
	/// Vector3を4つ入れるバージョン(vector型で渡す方もある)
	/// </summary>
	/// <param name="Start">スタート地点</param>
	/// <param name="p1">制御点その1</param>
	/// <param name="p2">制御点その2</param>
	/// <param name="end">ゴール地点</param>
	SplinePosition(Vector3& Start, Vector3& p1, Vector3& p2, Vector3& end);
	/// <summary>
	/// スタート地点,制御点,ゴール地点を好きに決められる方
	/// </summary>
	/// <param name="points">points{ start, start, p1, p2, p3, end, end };みたいにp3を追加したり</param>
	SplinePosition(const std::vector<Vector3>& points);
	~SplinePosition();

	/// <summary>
	/// スプライン曲線アップデート
	/// </summary>
	/// <param name="time">デフォMaxTime=1のうちのどこか</param>
	void Update(float& time);

	void Reset();

private://プライベート関数
	Vector3 SplinePositionUpdate(const std::vector<Vector3>& points, size_t& startIndex, float& t);

public://Setter
	void SetSplineMaxTime(const float& MaxTime) { this->MaxTime = MaxTime; }
	size_t GetIndex() { return startIndex; }

public://公開変数
	Vector3 NowPos;

private://プレイヤークラス変数

	//スプライン曲線に必要なメンバ変数
	size_t startIndex = 1;
	float timeRate_ = 0.0f;
	float MaxTime = 1.0f;
	std::vector<Vector3> points{};

};