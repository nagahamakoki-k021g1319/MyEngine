#include "SplinePosition.h"

SplinePosition::SplinePosition(Vector3& Start, Vector3& p1, Vector3& p2, Vector3& end)
{
	std::vector<Vector3> points{ Start, Start, p1, p2, end, end };
	this->points_ = points;
}

SplinePosition::SplinePosition(const std::vector<Vector3>& points)
{
	this->points_ = points;
}

SplinePosition::~SplinePosition(){}

void SplinePosition::Update(float& time)
{
	timeRate_ += time;
	if (timeRate_ >= MaxTime) {
		if (startIndex_ < points_.size() - 3) {

			startIndex_ += 1;
			timeRate_ -= MaxTime;
		}
		else {
			timeRate_ = MaxTime;
		}
	}
	NowPos = SplinePositionUpdate(points_, startIndex_, timeRate_);
}

void SplinePosition::Reset()
{
	startIndex_ = 1;
	timeRate_ = 0.0f;
}

Vector3 SplinePosition::SplinePositionUpdate(const std::vector<Vector3>& points, size_t& startIndex, float& t)
{
	//•âŠÔ‚·‚×‚«“_‚Ì”
	size_t n = points.size() - 2;

	if (startIndex > n)return points[n];
	if (startIndex < 1)return points[1];

	Vector3 p0 = points[startIndex - 1];
	Vector3 p1 = points[startIndex];
	Vector3 p2 = points[startIndex + 1];
	Vector3 p3 = points[startIndex + 2];

	Vector3 position = 0.5 * (2 * p1 + (-p0 + p2) * t + (2 * p0 - 5 * p1 + 4 * p2 - p3) * (t * t) + (-p0 + 3 * p1 - 3 * p2 + p3) * (t * t * t));

	return position;
}
